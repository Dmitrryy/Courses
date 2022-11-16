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

    // Print
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

        auto&& str = builder.CreateGlobalStringPtr("%ld\n\0");
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
    auto&& sub_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& sub_F = llvm::Function::Create(sub_Ty, linkage, mangl_method_name(class_name, "Sub", {orig_int_name}), module);
    sub_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", sub_F);
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

        auto&& c = builder.CreateSub(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Mult(Integer)
    //=-------------
    auto&& mul_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& mul_F = llvm::Function::Create(mul_Ty, linkage, mangl_method_name(class_name, "Mult", {orig_int_name}), module);
    mul_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", mul_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(mul_F->getArg(0), a3);
        builder.CreateStore(mul_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateMul(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Minus()
    //=-------
    auto&& minus_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo()}, false);
    auto&& minus_F = llvm::Function::Create(minus_Ty, linkage, mangl_method_name(class_name, "Minus", {}), module);
    minus_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", minus_F);
        builder.SetInsertPoint(entryBB);

        auto&& a2 = builder.CreateAlloca(intTy->getPointerTo());
        builder.CreateStore(minus_F->getArg(0), a2);

        auto&& a3 = builder.CreateLoad(intTy->getPointerTo(), a2);
        auto&& a6 = builder.CreateStructGEP(intTy, a3, 0);

        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);
        auto&& b = builder.getInt64(-1);

        auto&& c = builder.CreateMul(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Div(Integer)
    //=------------
    auto&& div_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& div_F = llvm::Function::Create(div_Ty, linkage, mangl_method_name(class_name, "Div", {orig_int_name}), module);
    div_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", div_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(div_F->getArg(0), a3);
        builder.CreateStore(div_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateSDiv(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // Equal(Integer)
    //=--------------
    auto&& eq_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& eq_F = llvm::Function::Create(eq_Ty, linkage, mangl_method_name(class_name, "Equal", {orig_int_name}), module);
    eq_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", eq_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(eq_F->getArg(0), a3);
        builder.CreateStore(eq_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateICmpEQ(a, b);
        auto&& c_ext = builder.CreateZExt(c, builder.getInt64Ty());

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c_ext, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // NotEqual(Integer)
    //=-----------------
    auto&& ne_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& ne_F = llvm::Function::Create(ne_Ty, linkage, mangl_method_name(class_name, "NotEqual", {orig_int_name}), module);
    ne_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", ne_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(ne_F->getArg(0), a3);
        builder.CreateStore(ne_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateICmpNE(a, b);
        auto&& c_ext = builder.CreateZExt(c, builder.getInt64Ty());

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c_ext, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // Greater(Integer)
    //=----------------
    auto&& gt_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& gt_F = llvm::Function::Create(gt_Ty, linkage, mangl_method_name(class_name, "Greater", {orig_int_name}), module);
    gt_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", gt_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(gt_F->getArg(0), a3);
        builder.CreateStore(gt_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateICmpSGT(a, b);
        auto&& c_ext = builder.CreateZExt(c, builder.getInt64Ty());

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c_ext, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // GreaterEqual(Integer)
    //=---------------------
    auto&& ge_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& ge_F = llvm::Function::Create(ge_Ty, linkage, mangl_method_name(class_name, "GreaterEqual", {orig_int_name}), module);
    ge_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", ge_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(ge_F->getArg(0), a3);
        builder.CreateStore(ge_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateICmpSGE(a, b);
        auto&& c_ext = builder.CreateZExt(c, builder.getInt64Ty());

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c_ext, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // Less(Integer)
    //=-------------
    auto&& lt_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& lt_F = llvm::Function::Create(lt_Ty, linkage, mangl_method_name(class_name, "Less", {orig_int_name}), module);
    lt_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", lt_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(lt_F->getArg(0), a3);
        builder.CreateStore(lt_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateICmpSLT(a, b);
        auto&& c_ext = builder.CreateZExt(c, builder.getInt64Ty());

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c_ext, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }

    // LessEqual(Integer)
    //=------------------
    auto&& le_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& le_F = llvm::Function::Create(le_Ty, linkage, mangl_method_name(class_name, "Greater", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", le_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(le_F->getArg(0), a3);
        builder.CreateStore(le_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateICmpSLE(a, b);
        auto&& c_ext = builder.CreateZExt(c, builder.getInt64Ty());

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c_ext, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


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