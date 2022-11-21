%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%define parse.error custom

%param {Driver* driver}

%locations
%debug


%code requires
{

	#include <algorithm>
	#include <string>
	#include <vector>

    #include "llvm/IR/Module.h"

	namespace yy { class Driver; }
}

%code
{

    #include "driver.h"
    //#include <stack>
    #include <cassert>
    #include <stack>
    #include <string>
    #include <unordered_set>
    #include <memory>
    #include <set>
    #include <map>

    #include "../Compiler/builder.hpp"

	namespace yy {

		parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* l, Driver* driver);

	}

    llvm::Module *g_module = nullptr;
    std::unique_ptr<llvm::IRBuilder<>> g_builder;

    std::string g_curClassName = "UNDEFINED";
    
    std::unordered_map< std::string, std::pair<size_t, std::string> > g_structFields;

    llvm::Value *g_this = nullptr;

    // Implements the logic of visibility of variables in a certain scope
    struct idInfo {
        llvm::Value *value = {};
        llvm::StructType *type = {};

        // set if we need create load before set as function's argument
        bool to_load = false;
    };
    ezg::ScopeTable< std::string, idInfo > g_nameTable;

    // Problem: when a falseBB is created before opening an scope, 
    // but it must be put in the IRBuilder.SetInsertPoint after the if-scope, 
    // we need to save this BB
    std::stack< llvm::BasicBlock * > ifFalseBBs;
    // Same problem as with if
    std::stack< llvm::BasicBlock * > whileCondBBs;
    std::stack< llvm::BasicBlock * > whileFalseBBs;
}

%token
  PLUS 			"+"
  MINUS			"-"
  MUL			"*"
  DIV			"/"
  SCOLON  		";"
  COLON         ":"
  COMMA         ","
  LPARENTHESES		"("
  RPARENTHESES		")"
  LBRACE        	"{"
  RBRACE        	"}"
  BIT_SHIFT_LEFT        "<<"
  BIT_SHIFT_RIGHT       ">>"
  BIT_AND           "&"
  BIT_OR            "|"
  BIT_XOR           "^"
  BIT_NOT           "~"
  PERSENT           "%"
  GREATER       	">"
  LESS          	"<"
  LLESS			"<="
  LGREATER		">="
  EQUAL         	"=="
  NONEQUAL      	"!="
  ASSIGN        	"="
  KW_WHILE		"while"
  KW_IF			"if"
  KW_RETURN     "return"
  KW_CLASS      "class"
  KW_THIS       "this"
  KW_METHOD     "method"
  KW_BREAK      "break"
  DOT           "."
  HEX_INTEGER_LITERAL "hex number"
  ERROR
;

%token < int64_t >     INTEGER_LITERAL
%token < std::string > IDENTIFIER

%nterm < std::vector< llvm::Type* > > class_variables
%nterm < llvm::StructType *> class_field_declaration
%nterm < llvm::StructType *> type

// pair<type, var_name>
%nterm < std::vector<std::pair<llvm::StructType *, std::string>> > ne_parameters
%nterm < std::vector<std::pair<llvm::StructType *, std::string>> > parameters
%nterm < std::pair<llvm::StructType *, std::string>> parameter_declaration

// pair<llvm_val, type_name>
%nterm < std::pair<llvm::Value *, std::string> > variable_access
//%nterm < std::pair<llvm::Value *, std::string> > expression
%nterm < std::pair<llvm::Value *, std::string> > exprLvl0
%nterm < std::pair<llvm::Value *, std::string> > exprLvl1
%nterm < std::pair<llvm::Value *, std::string> > exprLvl2
%nterm < std::pair<llvm::Value *, std::string> > exprLvl3
%nterm < std::pair<llvm::Value *, std::string> > exprLvl4
%nterm < std::pair<llvm::Value *, std::string> > exprLvl5

%nterm < std::vector<std::pair<llvm::Value *, std::string>> > ne_arguments
%nterm < std::vector<std::pair<llvm::Value *, std::string>> > arguments

%left '+' '-'
%left '*' '/'

%start program

%%


program 
: {
    g_module = driver->module;
    g_builder = std::move(std::make_unique<llvm::IRBuilder<>>(driver->context));
} class_declarations { }
;


class_declarations
: /* empty */ {}
| class_declarations class_declaration {}
;


class_declaration
: KW_CLASS IDENTIFIER 
{   
    g_curClassName = $2;
    //std::cout << @2 << ": class declaration: " << $2 << std::endl;
} LBRACE class_members RBRACE {
     g_structFields.clear();
}
;


// only var first declare. than only methods.
class_members
: class_variables {
    // firstly define struct type.
    llvm::StructType::create(driver->context, $1, ezg::mangl_class_name(g_curClassName));
}   // than we can use struct type in methods
class_methods { }
;


class_variables
: class_field_declaration {
    $$.clear();
    $$.push_back($1);
}
| class_variables class_field_declaration { 
    $1.push_back($2);
    $$ = $1;
    }
;

class_field_declaration
: IDENTIFIER COLON type SCOLON {
    g_structFields[$1] = std::make_pair(g_structFields.size(), $3->getName());
    $$ = $3;
    //std::cout << "Class field declaration: " << $1 << std::endl; 
}
;

class_methods
: method_declaration { }
| class_methods method_declaration { }
;


// defining new function with mangled name
method_declaration
: KW_METHOD IDENTIFIER LPARENTHESES parameters RPARENTHESES {
    // create csope
    //=------------
    size_t table_id = g_nameTable.createTable();
    g_nameTable.entryScope(table_id);

    // parameters
    //=----------
    std::vector<std::string> param_types_names;
    for (auto&& param: $4) {
        param_types_names.push_back(param.first->getName().str());
    }
    auto&& mangl_name = ezg::mangl_method_name(ezg::mangl_class_name(g_curClassName), $2, {param_types_names});
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name(g_curClassName));
    
    if(g_module->getFunction(mangl_name)) {
        throw syntax_error(@2, "redefinition method: $2");
    }

    // create function
    //=---------------
    std::vector<llvm::Type *> param_types = {struct_type->getPointerTo()};
    for(auto&& param: $4) {
        param_types.push_back(param.first->getPointerTo());
    }
    auto&& func_Ty = llvm::FunctionType::get(g_builder->getVoidTy(), param_types, false);
    auto&& func_F = llvm::Function::Create(func_Ty, llvm::Function::ExternalLinkage, mangl_name, g_module);
    func_F->setDSOLocal(true);

    auto&& entryBB = llvm::BasicBlock::Create(driver->context, "", func_F);
    g_builder->SetInsertPoint(entryBB);

    // Allocas
    //=-------
    g_this = g_builder->CreateAlloca(struct_type->getPointerTo());
    g_builder->CreateStore(func_F->getArg(0), g_this)->setAlignment(llvm::Align(8));

    size_t arg_id = 1;
    for(auto&& param: $4) {
        auto&& val = g_builder->CreateAlloca(param.first->getPointerTo());
        g_builder->CreateStore(func_F->getArg(arg_id), val);

        idInfo info {
            .value = val,
            .type = param.first,
            .to_load = true
        };

        g_nameTable.declare(g_nameTable.getCurTableId(), param.second, info);

        arg_id++;
    }

} LBRACE scope_body RBRACE {
    g_builder->CreateRetVoid();

    // close function scope
    //=--------------------
    g_nameTable.exitCurScope();
}
| KW_METHOD IDENTIFIER LPARENTHESES parameters RPARENTHESES COLON type {
    // create csope
    //=------------
    size_t table_id = g_nameTable.createTable();
    g_nameTable.entryScope(table_id);

    // parameters
    //=----------
    std::vector<std::string> param_types_names;
    for (auto&& param: $4) {
        param_types_names.push_back(param.first->getName().str());
    }
    auto&& mangl_name = ezg::mangl_method_name(ezg::mangl_class_name(g_curClassName), $2, {param_types_names});
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name(g_curClassName));
    
    if(g_module->getFunction(mangl_name)) {
        throw syntax_error(@2, "redefinition method: $2");
    }

    // create function
    //=---------------
    std::vector<llvm::Type *> param_types = {struct_type->getPointerTo()};
    for(auto&& param: $4) {
        param_types.push_back(param.first->getPointerTo());
    }
    auto&& func_Ty = llvm::FunctionType::get($7, param_types, false);
    auto&& func_F = llvm::Function::Create(func_Ty, llvm::Function::ExternalLinkage, mangl_name, g_module);
    func_F->setDSOLocal(true);

    auto&& entryBB = llvm::BasicBlock::Create(driver->context, "", func_F);
    g_builder->SetInsertPoint(entryBB);

    // Allocas
    //=-------
    g_this = g_builder->CreateAlloca(struct_type->getPointerTo());
    g_builder->CreateStore(func_F->getArg(0), g_this)->setAlignment(llvm::Align(8));

    size_t arg_id = 1;
    for(auto&& param: $4) {
        auto&& val = g_builder->CreateAlloca(param.first->getPointerTo());
        g_builder->CreateStore(func_F->getArg(arg_id), val);

        idInfo info {
            .value = val,
            .type = param.first,
            .to_load = true
        };

        g_nameTable.declare(g_nameTable.getCurTableId(), param.second, info);

        arg_id++;
    }
} LBRACE scope_body RBRACE {
    // close function scope
    //=--------------------
    g_nameTable.exitCurScope();
}
;


parameters
: /* empty */ { $$.clear(); }
| ne_parameters { $$ = $1; }
;


ne_parameters
: parameter_declaration {
    $$.clear();
    $$.push_back($1);
}
| ne_parameters COMMA parameter_declaration {
    $1.push_back($3);
    $$ = $1;
}
;


parameter_declaration
: IDENTIFIER COLON type {
    $$ = std::make_pair($3, $1);
}
;


scope_body
: /* empty */ {} 
| scope_body variable_declaration {}
| scope_body exprLvl0 SCOLON {} 
| scope_body statement { }
| scope_body {
    // create csope
    //=------------
    size_t table_id = g_nameTable.createTable(g_nameTable.getCurTableId());
    g_nameTable.entryScope(table_id);
} LBRACE scope_body RBRACE {
    // close scope
    //=-----------
    g_nameTable.exitCurScope();
}
;

// only inside function body!
variable_declaration
: IDENTIFIER COLON type SCOLON {
    auto&& a = g_builder->CreateAlloca($3);

    idInfo info {
        .value = a,
        .type = $3,
        .to_load = false
    };
    g_nameTable.declare(g_nameTable.getCurTableId(), $1, info);
}
;


statement
: assignment {}
| {
    // create condition BB
    //=-------------------
    auto&& func = g_builder->GetInsertBlock()->getParent();
    auto&& condBB = llvm::BasicBlock::Create(driver->context, "", func);
    g_builder->CreateBr(condBB);
    g_builder->SetInsertPoint(condBB);
    whileCondBBs.push(condBB);
} while_loop {}
| if_statement {}
| return_statement {}
| break_statement {}
;


break_statement
: KW_BREAK SCOLON {
    // jump to false BB of the while loop
    g_builder->CreateBr(whileFalseBBs.top());
}
;


assignment
: variable_access ASSIGN exprLvl0 SCOLON {
    // Call $1.Set
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Set", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Set' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});
}
;


while_loop
: KW_WHILE LPARENTHESES exprLvl0 RPARENTHESES {
    // create csope
    //=------------
    size_t table_id = g_nameTable.createTable(g_nameTable.getCurTableId());
    g_nameTable.entryScope(table_id);

    if($3.second != "Integer") {
        throw syntax_error(@3, "condition at 'while' can be only 'Integer' type. Receive: '" + $3.second + "'");
    }

    // converting Integer to bool
    //=--------------------------
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name($3.second));
    auto&& valPtr = g_builder->CreateStructGEP(struct_type, $3.first, 0);
    auto&& val = g_builder->CreateLoad(g_builder->getInt64Ty(), valPtr);
    auto&& cmp = g_builder->CreateICmpNE(val, g_builder->getInt64(0));

    // create BBs and br
    //=-----------------
    auto&& func = g_builder->GetInsertBlock()->getParent();
    auto&& trueBB = llvm::BasicBlock::Create(driver->context, "", func);
    auto&& falseBB = llvm::BasicBlock::Create(driver->context, "", func);

    g_builder->CreateCondBr(cmp, trueBB, falseBB);
    g_builder->SetInsertPoint(trueBB);

    whileFalseBBs.push(falseBB);

} LBRACE scope_body RBRACE {
    // close scope
    //=-----------
    g_nameTable.exitCurScope();
    if(!(g_builder->GetInsertBlock()->getTerminator())) {
        g_builder->CreateBr(whileCondBBs.top());
    }
    whileCondBBs.pop();

    g_builder->SetInsertPoint(whileFalseBBs.top());
    whileFalseBBs.pop();
}
;

//TODO else
if_statement
: KW_IF LPARENTHESES exprLvl0 RPARENTHESES {
    // create csope
    //=------------
    size_t table_id = g_nameTable.createTable(g_nameTable.getCurTableId());
    g_nameTable.entryScope(table_id);

    if($3.second != "Integer") {
        throw syntax_error(@3, "condition at 'if' can be only 'Integer' type. Receive: '" + $3.second + "'");
    }
    
    // converting Integer to bool
    //=--------------------------
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name($3.second));
    auto&& valPtr = g_builder->CreateStructGEP(struct_type, $3.first, 0);
    auto&& val = g_builder->CreateLoad(g_builder->getInt64Ty(), valPtr);
    auto&& cmp = g_builder->CreateICmpNE(val, g_builder->getInt64(0));
    
    // create BBs and br
    //=-----------------
    auto&& func = g_builder->GetInsertBlock()->getParent();
    auto&& trueBB = llvm::BasicBlock::Create(driver->context, "", func);
    auto&& falseBB = llvm::BasicBlock::Create(driver->context, "", func);

    g_builder->CreateCondBr(cmp, trueBB, falseBB);
    g_builder->SetInsertPoint(trueBB);

    ifFalseBBs.push(falseBB);

} LBRACE scope_body RBRACE {
    // close scope
    //=-----------
    g_nameTable.exitCurScope();
    if(!(g_builder->GetInsertBlock()->getTerminator())) {
        g_builder->CreateBr(ifFalseBBs.top());
    }
    g_builder->SetInsertPoint(ifFalseBBs.top());
    ifFalseBBs.pop();
}
;

return_statement
: KW_RETURN exprLvl0 SCOLON {
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name($2.second));
    auto&& load = g_builder->CreateLoad(struct_type, $2.first);
    g_builder->CreateRet(load);
}
;



// Expresions
// FIXME: mnogo copipasta
//=------------------------------------------------
exprLvl0
: exprLvl1 { $$ = $1; }
| exprLvl1 BIT_SHIFT_LEFT exprLvl0 {
    // Call $1.Shl
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Shl", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Shl' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl1 BIT_SHIFT_RIGHT exprLvl0 {
    // Call $1.AShr
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "AShr", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'AShr' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl1 BIT_AND exprLvl0 {
    // Call $1.And
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "And", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'And' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl1 BIT_OR exprLvl0 {
    // Call $1.Or
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Or", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Or' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl1 BIT_XOR exprLvl0 {
    // Call $1.Xor
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Xor", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Xor' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
;

exprLvl1
: exprLvl2 { $$ = $1; }
| exprLvl2 GREATER exprLvl1 {
    // Call $1.Greater
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Greater", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Greater' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl2 LESS exprLvl1 {
    // Call $1.Less
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Less", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Less' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl2 LLESS exprLvl1 {
    // Call $1.LessEqual
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "LessEqual", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'LessEqual' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl2 LGREATER exprLvl1 {
    // Call $1.GreaterEqual
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "GreaterEqual", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'GreaterEqual' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl2 EQUAL exprLvl1 {
    // Call $1.Equal
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Equal", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Equal' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl2 NONEQUAL exprLvl1 {
    // Call $1.NotEqual
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "NotEqual", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'NotEqual' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
; 

exprLvl2
: exprLvl3 { $$ = $1; }
| exprLvl2 PLUS exprLvl3 {
    // Call $1.Add
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Add", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Add' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl2 MINUS exprLvl3 {
    // Call $1.Sub
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Sub", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Sub' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
;

exprLvl3
: exprLvl3 MUL exprLvl4 {
    // Call $1.Mult
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Mult", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Mult' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl3 DIV exprLvl4 {
    // Call $1.Div
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Div", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Div' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl3 PERSENT exprLvl4 {
    // Call $1.SRem
    const std::string class_name = $1.second;
    const std::string arg_type = $3.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "SRem", {arg_type});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'SRem' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$1.first, $3.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl4 { $$ = $1; }
;

// unminus or unplus
exprLvl4
: MINUS exprLvl5 {
    // Call $2.Minus
    const std::string class_name = $2.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Minus", {});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Minus' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$2.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
//| PLUS exprLvl5 {assert(0 && "not implemented");}
| BIT_NOT exprLvl5 {
    // Call $2.Not
    const std::string class_name = $2.second;
    const std::string method_name = ezg::mangl_method_name(class_name, "Not", {});

    auto&& callee = g_module->getFunction(method_name);
    if(!callee) {
        throw syntax_error(@2, "no method named 'Not' in '" + class_name +"'");
    }
    auto&& call = g_builder->CreateCall(callee, {$2.first});

    // check if we should save result
    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(call, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| exprLvl5 { $$ = $1; }
;


exprLvl5
: exprLvl5 DOT IDENTIFIER LPARENTHESES arguments RPARENTHESES {
    auto&& class_name =  $1.second;
    std::vector< std::string > args_type;
    std::vector< llvm::Value* > args_val = {$1.first};
    args_type.reserve($5.size());
    args_val.reserve($5.size() + 1);
    for(auto&& arg: $5) {
        args_type.push_back(arg.second);
        args_val.push_back(arg.first);
    }
    auto&& callee = g_module->getFunction(ezg::mangl_method_name(class_name, $3, {args_type}));
    if (!callee) {
        throw syntax_error(@3, "no method named '" + $3 + "' in '" + class_name +"'");
    }
    auto&& a6 = g_builder->CreateCall(callee, args_val);

    auto&& ret_Ty = dyn_cast<llvm::StructType>(callee->getReturnType());
    $$ = {};
    if(ret_Ty) {
        // alloca to allow take pointer to callInst result
        auto&& a = g_builder->CreateAlloca(ret_Ty);
        g_builder->CreateStore(a6, a);
        $$ = std::make_pair(a, ezg::demangl_class_name(ret_Ty->getName().str()));
    }
}
| LPARENTHESES exprLvl0 RPARENTHESES { $$ = $2; }
| variable_access { $$ = $1; }
| INTEGER_LITERAL {
    auto&& cur_func = g_builder->GetInsertBlock()->getParent();
    $$ = std::make_pair(ezg::createIntegerByLiteral(cur_func, $1), std::string("Integer"));
}
;




variable_access
: IDENTIFIER {
    auto&& var_id = g_structFields.find($1);
    if (var_id != g_structFields.end()) 
    {
        auto&& field_id = var_id->second.first;
        auto&& type_name = ezg::demangl_class_name(var_id->second.second);
        auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name(g_curClassName));

        auto&& a4 = g_builder->CreateLoad(struct_type->getPointerTo(), g_this);
        auto&& a5 = g_builder->CreateStructGEP(struct_type, a4, field_id);
        $$ = std::make_pair(a5, type_name);
    } 
    else if(g_nameTable.isDeclared(g_nameTable.getCurTableId(), $1)) 
    {
        auto&& info = g_nameTable.access($1);

        auto* val = info.value;
        if(info.to_load) {
            val = g_builder->CreateLoad(info.type->getPointerTo(), info.value);
        }
        $$ = std::make_pair(val, ezg::demangl_class_name(info.type->getName().str()));
    }
    else {
        throw syntax_error(@1, "undefined identifier '" + $1 + "'");
    }
}
| KW_THIS {
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name(g_curClassName));

    auto&& a1 = g_builder->CreateLoad(struct_type->getPointerTo(), g_this);
    $$ = std::make_pair(a1, g_curClassName);
}
;


arguments
: /* empty */ { $$.clear(); }
| ne_arguments { $$ = $1; }
;


ne_arguments
: exprLvl0 { $$.clear(); $$.push_back($1); }
| ne_arguments COMMA exprLvl0 { $1.push_back($3); $$ = $1; }
;


type
: IDENTIFIER { 
    //std::cout << "type ID: " << $1 << std::endl; 
    $$ = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name($1));
    if (!$$) {
        throw syntax_error(@1, "undefined identifier '" + $1 + "'");
    }
    }
;

%%

namespace yy {

	parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* l, Driver* driver) {
		return driver->yylex (l, yylval);
	}

	void parser::error (const parser::location_type& l, const std::string& msg) {
		auto tmp = symbol_type(0, l);
		context ctx(*this, tmp);
		driver->error(ctx, msg);
	}

	void parser::report_syntax_error(parser::context const& ctx) const
	{
		driver->error(ctx, "syntax error");
	}
}