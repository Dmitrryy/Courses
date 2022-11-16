#include "builtins.h"
#include "utils.h"

#include "llvm/IR/IRBuilder.h"

namespace ezg
{

const std::string orig_int_name = "Integer";

llvm::StructType *declareIntegerBuiltin(llvm::Module *module) {
    auto&& context = module->getContext();
    llvm::IRBuilder<> builder(context);
    const auto linkage = llvm::GlobalValue::LinkageTypes::ExternalLinkage;
    const std::string class_name = mangl_class_name(orig_int_name);

    // Integer type
    //=------------
    auto&& intTy = llvm::StructType::create({builder.getInt64Ty()}, class_name, false);

    // print
    //=-----
    auto&& printf_Ty = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8Ty()->getPointerTo()}, true);
    auto&& printf_F = llvm::Function::Create(printf_Ty, linkage, "printf", module);
    printf_F->setDSOLocal(true);

    auto&& print_Ty = llvm::FunctionType::get(builder.getVoidTy(), {intTy->getPointerTo()},false);
    auto&& print_F = llvm::Function::Create(print_Ty, linkage, mangl_method_name(class_name, "Print", {}), module);
    print_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", print_F);
        builder.SetInsertPoint(entryBB);

        auto&& a2 = builder.CreateAlloca(intTy->getPointerTo());
        a2->setAlignment(llvm::Align(8));

        builder.CreateStore(print_F->getArg(0), a2)->setAlignment(llvm::Align(8));

        auto&& a3 = builder.CreateLoad(intTy->getPointerTo(), a2);
        auto&& a4 = builder.CreateStructGEP(intTy, a3, 0);

        auto&& a5 = builder.CreateLoad(builder.getInt64Ty(), a4);

        auto&& str = builder.CreateGlobalStringPtr("%lu\n\0");
        //auto&& strPtr = builder.CreateConstGEP2_64(str->getType(), str, 0, 0);
        builder.CreateCall(printf_F, {str, a5});

        builder.CreateRetVoid();
    }

    // set(Integer)
    //=----------------
    auto&& set_Ty = llvm::FunctionType::get(builder.getVoidTy(), {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& set_F = llvm::Function::Create(set_Ty, linkage, mangl_method_name(class_name, "Set", {orig_int_name}), module);
    set_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", set_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(set_F->getArg(0), a3);
        builder.CreateStore(set_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a6 = builder.CreateLoad(intTy, a5);
        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a3);
        builder.CreateStore(a6, a7);

        builder.CreateRetVoid();
    }

    // Add()
    //=-----
    auto&& add_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& add_F = llvm::Function::Create(add_Ty, linkage, mangl_method_name(class_name, "Add", {orig_int_name}), module);
    add_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", add_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(set_F->getArg(0), a3);
        builder.CreateStore(set_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateAdd(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // Sub(Integer)
    //=------------

    // Mult(Integer)
    //=-------------

    // Div(Integer)
    //=------------
    

    // Equal(Integer)
    //=--------------

    // NotEqual(Integer)
    //=-----------------

    // Greater(Integer)
    //=----------------

    // GreaterEqual(Integer)
    //=---------------------

    // Less(Integer)
    //=-------------

    // LessEqual(Integer)
    //=------------------



    return nullptr;
}

llvm::Value *createIntegerByLiteral(llvm::Function *func, int64_t val) {
    auto&& context = func->getContext();
    llvm::IRBuilder<> builder(context);
    auto&& BB = func->getEntryBlock();
    builder.SetInsertPoint(&BB, BB.begin());

    auto&& struct_type = llvm::StructType::getTypeByName(context, ezg::mangl_class_name("Integer"));
    auto&& st = builder.CreateAlloca(struct_type);

    auto&& gep = builder.CreateStructGEP(struct_type, st, 0);
    builder.CreateStore(builder.getInt64(val), gep);
    return st;
}

}