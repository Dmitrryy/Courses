#include "builtins.h"
#include "utils.h"

#include "llvm/IR/IRBuilder.h"

namespace ezg
{

const std::string orig_int_name = "Integer";
const std::string orig_graphics_name = "Graphics";

llvm::StructType *declareIntegerBuiltin(llvm::Module *module) {
    auto&& context = module->getContext();
    llvm::IRBuilder<> builder(context);
    const auto linkage = llvm::GlobalValue::LinkageTypes::ExternalLinkage;
    const std::string int_class_name = mangl_class_name(orig_int_name);

    // Integer type
    //=------------
    auto&& intTy = llvm::StructType::create({builder.getInt64Ty()}, int_class_name, false);

    // Print
    //=-----
    auto&& printf_Ty = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8Ty()->getPointerTo()}, true);
    auto&& printf_F = llvm::Function::Create(printf_Ty, linkage, "printf", module);
    printf_F->setDSOLocal(true);

    auto&& print_Ty = llvm::FunctionType::get(builder.getVoidTy(), {intTy->getPointerTo()},false);
    auto&& print_F = llvm::Function::Create(print_Ty, linkage, mangl_method_name(int_class_name, "Print", {}), module);
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
    auto&& set_F = llvm::Function::Create(set_Ty, linkage, mangl_method_name(int_class_name, "Set", {orig_int_name}), module);
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
    auto&& add_F = llvm::Function::Create(add_Ty, linkage, mangl_method_name(int_class_name, "Add", {orig_int_name}), module);
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
    auto&& sub_F = llvm::Function::Create(sub_Ty, linkage, mangl_method_name(int_class_name, "Sub", {orig_int_name}), module);
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
    auto&& mul_F = llvm::Function::Create(mul_Ty, linkage, mangl_method_name(int_class_name, "Mult", {orig_int_name}), module);
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
    auto&& minus_F = llvm::Function::Create(minus_Ty, linkage, mangl_method_name(int_class_name, "Minus", {}), module);
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
    auto&& div_F = llvm::Function::Create(div_Ty, linkage, mangl_method_name(int_class_name, "Div", {orig_int_name}), module);
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
    auto&& eq_F = llvm::Function::Create(eq_Ty, linkage, mangl_method_name(int_class_name, "Equal", {orig_int_name}), module);
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
    auto&& ne_F = llvm::Function::Create(ne_Ty, linkage, mangl_method_name(int_class_name, "NotEqual", {orig_int_name}), module);
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
    auto&& gt_F = llvm::Function::Create(gt_Ty, linkage, mangl_method_name(int_class_name, "Greater", {orig_int_name}), module);
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
    auto&& ge_F = llvm::Function::Create(ge_Ty, linkage, mangl_method_name(int_class_name, "GreaterEqual", {orig_int_name}), module);
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
    auto&& lt_F = llvm::Function::Create(lt_Ty, linkage, mangl_method_name(int_class_name, "Less", {orig_int_name}), module);
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
    auto&& le_F = llvm::Function::Create(le_Ty, linkage, mangl_method_name(int_class_name, "Greater", {orig_int_name}), module);
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


    // Or(Integer) : Integer
    //=-----------
    auto&& or_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& or_F = llvm::Function::Create(or_Ty, linkage, mangl_method_name(int_class_name, "Or", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", or_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(or_F->getArg(0), a3);
        builder.CreateStore(or_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateOr(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Xor(Integer) : Integer
    //=----------------------
    auto&& xor_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& xor_F = llvm::Function::Create(xor_Ty, linkage, mangl_method_name(int_class_name, "Xor", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", xor_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(xor_F->getArg(0), a3);
        builder.CreateStore(xor_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateXor(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Not() : Integer
    //=----------------------
    auto&& not_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo()}, false);
    auto&& not_F = llvm::Function::Create(not_Ty, linkage, mangl_method_name(int_class_name, "Not", {}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", not_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(not_F->getArg(0), a3);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& c = builder.CreateNot(a);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // And(Integer) : Integer
    //=----------------------
    auto&& and_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& and_F = llvm::Function::Create(or_Ty, linkage, mangl_method_name(int_class_name, "And", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", and_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(and_F->getArg(0), a3);
        builder.CreateStore(and_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateAnd(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Shl(Integer) : Integer
    //=----------------------
    auto&& shl_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& shl_F = llvm::Function::Create(shl_Ty, linkage, mangl_method_name(int_class_name, "Shl", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", shl_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(shl_F->getArg(0), a3);
        builder.CreateStore(shl_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateShl(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // AShr(Integer) : Integer
    //=-----------------------
    auto&& ashr_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& ashr_F = llvm::Function::Create(ashr_Ty, linkage, mangl_method_name(int_class_name, "AShr", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", ashr_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(ashr_F->getArg(0), a3);
        builder.CreateStore(ashr_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateAShr(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    // Srem(Integer) : Integer
    //=-----------------------
    auto&& srem_Ty = llvm::FunctionType::get(intTy, {intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& srem_F = llvm::Function::Create(srem_Ty, linkage, mangl_method_name(int_class_name, "SRem", {orig_int_name}), module);
    le_F->setDSOLocal(true);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", srem_F);
        builder.SetInsertPoint(entryBB);

        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(srem_F->getArg(0), a3);
        builder.CreateStore(srem_F->getArg(1), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a6 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a = builder.CreateLoad(builder.getInt64Ty(), a6);

        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);
        auto&& a8 = builder.CreateStructGEP(intTy, a7, 0);
        auto&& b = builder.CreateLoad(builder.getInt64Ty(), a8);

        auto&& c = builder.CreateSRem(a, b);

        auto&& res = builder.CreateAlloca(intTy);
        auto&& res_f = builder.CreateStructGEP(intTy, res, 0);
        builder.CreateStore(c, res_f);

        auto&& lres = builder.CreateLoad(intTy, res);

        builder.CreateRet(lres);
    }


    //=-------------------------------------------------------------------------------
    // Graphisc type
    //=-------------
    const std::string graph_class_name = mangl_class_name(orig_graphics_name);

    // Now struct is ampty. In future we can contain handle to window.
    auto&& grTy = llvm::StructType::create({builder.getInt64Ty()}, graph_class_name, false);

    // glib interface
    //=--------------
    auto &&sEngine_initTy = llvm::FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty(), builder.getInt32Ty()}, false);
    auto &&sEngine_initF = llvm::Function::Create(sEngine_initTy, linkage, "sEngine_init", module);
    sEngine_initF->setDSOLocal(true);

    auto &&sEngine_windowIsOpenTy = llvm::FunctionType::get(builder.getInt32Ty(), true);
    auto &&sEngine_windowIsOpenF = llvm::Function::Create(sEngine_windowIsOpenTy, linkage, "sEngine_windowIsOpen", module);
    sEngine_windowIsOpenF->setDSOLocal(true);

    auto &&get_time_millisecondsTy = llvm::FunctionType::get(builder.getInt64Ty(), true);
    auto &&get_time_millisecondsF = llvm::Function::Create(get_time_millisecondsTy, linkage, "get_time_milliseconds", module);
    get_time_millisecondsF->setDSOLocal(true);

    auto &&put_pixelTy = llvm::FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()}, false);
    auto&& put_pixelF = llvm::Function::Create(put_pixelTy, linkage, "put_pixel", module);
    put_pixelF->setDSOLocal(true);

    auto &&flushTy = llvm::FunctionType::get(builder.getVoidTy(), true);
    auto &&flushF = llvm::Function::Create(flushTy, linkage, "flush", module);
    flushF->setDSOLocal(true);
    
    // Init(width: Integer, heigth: Integer)
    //=-------------------------------------
    auto&& init_Ty = llvm::FunctionType::get(builder.getVoidTy(), {grTy->getPointerTo(), intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& init_F = llvm::Function::Create(init_Ty, linkage, mangl_method_name(graph_class_name, "Init", {orig_int_name, orig_int_name}), module);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", init_F);
        builder.SetInsertPoint(entryBB);

        auto&& a1 = builder.CreateAlloca(grTy->getPointerTo());
        auto&& a2 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(init_F->getArg(0), a1);
        builder.CreateStore(init_F->getArg(1), a2);
        builder.CreateStore(init_F->getArg(2), a3);

        auto&& a4 = builder.CreateLoad(intTy->getPointerTo(), a2);
        auto&& w_ptr = builder.CreateStructGEP(intTy, a4, 0);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& h_ptr = builder.CreateStructGEP(intTy, a5, 0);

        auto&& w = builder.CreateLoad(builder.getInt64Ty(), w_ptr);
        auto&& h = builder.CreateLoad(builder.getInt64Ty(), h_ptr);
        auto&& w_t = builder.CreateTrunc(w, builder.getInt32Ty());
        auto&& h_t = builder.CreateTrunc(h, builder.getInt32Ty());
        builder.CreateCall(sEngine_initF, {w_t, h_t});

        builder.CreateRetVoid();
    }

    // WindowIsOpen()
    //=--------------
    auto&& windowIsOpen_Ty = llvm::FunctionType::get(intTy, {grTy->getPointerTo()}, false);
    auto&& windowIsOpen_F = llvm::Function::Create(windowIsOpen_Ty, linkage, mangl_method_name(graph_class_name, "WindowIsOpen", {}), module);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", windowIsOpen_F);
        builder.SetInsertPoint(entryBB);

        auto&& a = builder.CreateAlloca(grTy->getPointerTo());
        auto&& res = builder.CreateAlloca(intTy);
        builder.CreateStore(windowIsOpen_F->getArg(0), a);

        auto&& ptr = builder.CreateStructGEP(intTy, res, 0);

        auto&& call = builder.CreateCall(sEngine_windowIsOpenF, {});
        auto&& call_e = builder.CreateZExt(call, builder.getInt64Ty());

        builder.CreateStore(call_e, ptr);

        builder.CreateRet(builder.CreateLoad(intTy, res));
    }

    // Flush()
    //=-------
    auto&& Flush_Ty = llvm::FunctionType::get(builder.getVoidTy(), {grTy->getPointerTo()}, false);
    auto&& Flush_F = llvm::Function::Create(Flush_Ty, linkage, mangl_method_name(graph_class_name, "Flush", {}), module);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", Flush_F);
        builder.SetInsertPoint(entryBB);

        auto&& a = builder.CreateAlloca(grTy->getPointerTo());
        builder.CreateStore(Flush_F->getArg(0), a);

        builder.CreateCall(flushF, {});
        builder.CreateRetVoid();
    }

    // PutPixel(x: Integer, y: Integer, rgba: Integer)
    //=-----------------------------------------------
    auto&& PutPixel_Ty = llvm::FunctionType::get(builder.getVoidTy(), {grTy->getPointerTo(), intTy->getPointerTo(), intTy->getPointerTo(), intTy->getPointerTo()}, false);
    auto&& PutPixel_F = llvm::Function::Create(PutPixel_Ty, linkage, mangl_method_name(graph_class_name, "PutPixel", {orig_int_name, orig_int_name, orig_int_name}), module);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", PutPixel_F);
        builder.SetInsertPoint(entryBB);

        auto&& a1 = builder.CreateAlloca(grTy->getPointerTo());
        auto&& a2 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a3 = builder.CreateAlloca(intTy->getPointerTo());
        auto&& a4 = builder.CreateAlloca(intTy->getPointerTo());

        builder.CreateStore(PutPixel_F->getArg(0), a1);
        builder.CreateStore(PutPixel_F->getArg(1), a2);
        builder.CreateStore(PutPixel_F->getArg(2), a3);
        builder.CreateStore(PutPixel_F->getArg(3), a4);

        auto&& a5 = builder.CreateLoad(intTy->getPointerTo(), a2);
        auto&& a6 = builder.CreateLoad(intTy->getPointerTo(), a3);
        auto&& a7 = builder.CreateLoad(intTy->getPointerTo(), a4);

        auto&& a8 = builder.CreateStructGEP(intTy, a5, 0);
        auto&& a9 = builder.CreateStructGEP(intTy, a6, 0);
        auto&& a10 = builder.CreateStructGEP(intTy, a7, 0);

        auto&& a11 = builder.CreateLoad(builder.getInt64Ty(), a8);
        auto&& a12 = builder.CreateLoad(builder.getInt64Ty(), a9);
        auto&& a13 = builder.CreateLoad(builder.getInt64Ty(), a10);

        auto&& x = builder.CreateTrunc(a11, builder.getInt32Ty());
        auto&& y = builder.CreateTrunc(a12, builder.getInt32Ty());
        auto&& rgba = builder.CreateTrunc(a13, builder.getInt32Ty());

        builder.CreateCall(put_pixelF, {x, y, rgba});

        builder.CreateRetVoid();
    }

    // GetTime() : Integer
    //=-------------------
    auto&& get_time_Ty = llvm::FunctionType::get(intTy, {grTy->getPointerTo()}, false);
    auto&& get_time_F = llvm::Function::Create(get_time_Ty, linkage, mangl_method_name(graph_class_name, "GetTime", {}), module);
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", get_time_F);
        builder.SetInsertPoint(entryBB);

        auto&& a = builder.CreateAlloca(grTy->getPointerTo());
        auto&& res = builder.CreateAlloca(intTy);
        builder.CreateStore(get_time_F->getArg(0), a);

        auto&& ptr = builder.CreateStructGEP(intTy, res, 0);

        auto&& call = builder.CreateCall(get_time_millisecondsF, {});
        auto&& call_e = builder.CreateZExt(call, builder.getInt64Ty());

        builder.CreateStore(call_e, ptr);

        builder.CreateRet(builder.CreateLoad(intTy, res));
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