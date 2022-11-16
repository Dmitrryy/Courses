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

	// forward declaration of argument to parser
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

    #include "../Compiler/VarTable.h"

	namespace yy {

		parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* l, Driver* driver);

	}

    llvm::Module *g_module = nullptr;
    std::unique_ptr<llvm::IRBuilder<>> g_builder;

    std::string g_curClassName = "UNDEFINED";
    
    std::unordered_map< std::string, std::pair<size_t, std::string> > g_structFields;

    llvm::Value *g_this = nullptr;
    size_t g_fieldCounter = 0;
    llvm::StructType *g_curStruct_Ty = nullptr;

    struct idInfo {
        llvm::Value *value = {};
        llvm::StructType *type = {};

        // set if we need create load before set as function's argument
        bool to_load = false;
    };
    ezg::ScopeTable< std::string, idInfo > g_nameTable;

    std::stack< llvm::BasicBlock * > ifFalseBBs;
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
  GREATER       	">"
  LESS          	"<"
  LLESS			"<="
  LGREATER		">="
  EQUAL         	"=="
  NONEQUAL      	"!="
  ASSIGN        	"="
  //QMARK         	"?(scanf)"
  KW_WHILE		"while"
  KW_IF			"if"
  KW_RETURN     "return"
  KW_CLASS      "class"
  KW_THIS       "this"
  KW_METHOD     "method"
  DOT           "."
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
%nterm < std::pair<llvm::Value *, std::string> > expression
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
    std::cout << @2 << ": class declaration: " << $2 << std::endl;
} LBRACE class_members RBRACE {
     g_structFields.clear();
     g_fieldCounter = 0;
}
;


// only var first declare. than only methods.
class_members
: class_variables {
    // firstly define struct type.
    g_curStruct_Ty = llvm::StructType::create(driver->context, $1, 
    ezg::mangl_class_name(g_curClassName));
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
    g_structFields[$1] = std::make_pair(g_fieldCounter++, $3->getName());
    $$ = $3;
    std::cout << "Class field declaration: " << $1 << std::endl; 
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
| scope_body expression SCOLON {} 
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

    std::cout << "Var declaration: " << $1 << std::endl; 
}
;


statement
: assignment {assert(0 && "not implemented");}
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
;

//TODO
assignment
: IDENTIFIER ASSIGN expression SCOLON {}
;


while_loop
: KW_WHILE LPARENTHESES expression RPARENTHESES {
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

    g_builder->CreateBr(whileCondBBs.top());
    whileCondBBs.pop();

    g_builder->SetInsertPoint(whileFalseBBs.top());
    whileFalseBBs.pop();
}
;

//TODO else
if_statement
: KW_IF LPARENTHESES expression RPARENTHESES {
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
    
    g_builder->CreateBr(ifFalseBBs.top());
    g_builder->SetInsertPoint(ifFalseBBs.top());
    ifFalseBBs.pop();
}
;

return_statement
: KW_RETURN expression SCOLON {
    auto&& struct_type = llvm::StructType::getTypeByName(driver->context, ezg::mangl_class_name($2.second));
    auto&& load = g_builder->CreateLoad(struct_type, $2.first);
    g_builder->CreateRet(load);
}
;

expression
: variable_access { $$ = $1; }
| INTEGER_LITERAL {
    auto&& cur_func = g_builder->GetInsertBlock()->getParent();
    $$ = std::make_pair(ezg::createIntegerByLiteral(cur_func, $1), std::string("Integer"));
}
 // TODO construct variable
| type LPARENTHESES arguments RPARENTHESES { assert(0 && "not implemented"); }
| expression DOT IDENTIFIER LPARENTHESES arguments RPARENTHESES {
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
: expression { $$.clear(); $$.push_back($1); }
| ne_arguments COMMA expression { $1.push_back($3); $$ = $1; }
;


type
: IDENTIFIER { 
    std::cout << "type ID: " << $1 << std::endl; 
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