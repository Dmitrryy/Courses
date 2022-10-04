
#include <iostream>
#include <fstream>
#include "llvm-10/llvm/Support/raw_ostream.h"
#include "llvm-10/llvm/IR/LLVMContext.h"
#include "llvm-10/llvm/IR/Module.h"
#include "llvm-10/llvm/IR/IRBuilder.h"

int main()
{
    llvm::LLVMContext context;
    // ; ModuleID = 'main.c'
    // source_filename = "main.c"
    llvm::Module *module = new llvm::Module("main.c", context);
    llvm::IRBuilder<> builder(context);

    //=----------------
    // Create functions
    auto &&mainTy = llvm::FunctionType::get(builder.getInt32Ty(), false);
    auto &&mainFunc = llvm::Function::Create(mainTy, llvm::Function::ExternalLinkage, "main", module);
    mainFunc->setDSOLocal(true);

    auto &&shader_1Ty = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt64Ty(), builder.getInt64Ty(), builder.getInt64Ty(), builder.getInt64Ty(), builder.getInt64Ty()}, false);
    auto &&shader_1F = llvm::Function::Create(shader_1Ty, llvm::GlobalValue::LinkageTypes::ExternalLinkage, "shader_1", module);
    shader_1F->setDSOLocal(true);

    auto &&sEngine_windowIsOpenTy = llvm::FunctionType::get(builder.getInt32Ty(), true);
    auto &&sEngine_windowIsOpenF = llvm::Function::Create(sEngine_windowIsOpenTy, llvm::Function::ExternalLinkage, "sEngine_windowIsOpen", module);
    sEngine_windowIsOpenF->setDSOLocal(true);

    auto &&sEngine_initTy = llvm::FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty(), builder.getInt32Ty()}, false);
    auto &&sEngine_initF = llvm::Function::Create(sEngine_initTy, llvm::Function::ExternalLinkage, "sEngine_init", module);
    sEngine_initF->setDSOLocal(true);

    auto &&get_time_millisecondsTy = llvm::FunctionType::get(builder.getInt64Ty(), true);
    auto &&get_time_millisecondsF = llvm::Function::Create(get_time_millisecondsTy, llvm::Function::ExternalLinkage, "get_time_milliseconds", module);
    get_time_millisecondsF->setDSOLocal(true);

    auto &&int_cosTy = llvm::FunctionType::get(builder.getInt64Ty(), {builder.getInt64Ty()}, false);
    auto &&int_cosF = llvm::Function::Create(int_cosTy, llvm::Function::ExternalLinkage, "int_cos", module);
    int_cosF->setDSOLocal(true);

    auto &&palette_intTy = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt64Ty(), builder.getInt64Ty(), builder.getInt64Ty(), builder.getInt64Ty()}, false);
    auto &&palette_intF = llvm::Function::Create(palette_intTy, llvm::Function::ExternalLinkage, "palette_int", module);
    palette_intF->setDSOLocal(true);

    auto &&flushTy = llvm::FunctionType::get(builder.getVoidTy(), true);
    auto &&flushF = llvm::Function::Create(flushTy, llvm::Function::ExternalLinkage, "flush", module);
    flushF->setDSOLocal(true);

    auto &&pack_4ui8_to_ui32Ty = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8Ty(), builder.getInt8Ty(), builder.getInt8Ty(), builder.getInt8Ty()}, false);
    auto &&pack_4ui8_to_ui32F = llvm::Function::Create(pack_4ui8_to_ui32Ty, llvm::Function::ExternalLinkage, "pack_4ui8_to_ui32", module);
    pack_4ui8_to_ui32F->setDSOLocal(true);

    auto &&abs_dummyTy = llvm::FunctionType::get(builder.getInt64Ty(), {builder.getInt64Ty()}, false);
    auto &&abs_dummyF = llvm::Function::Create(abs_dummyTy, llvm::Function::ExternalLinkage, "abs_dummy", module);
    abs_dummyF->setDSOLocal(true);

    //=------------------------------
    // define dso_local i32 @main() {
    {
        llvm::BasicBlock *mainEntryBB = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB8 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB11 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB13 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB16 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB17 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB20 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB30 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB33 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB34 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB37 = llvm::BasicBlock::Create(context, "", mainFunc);
        llvm::BasicBlock *mainBB38 = llvm::BasicBlock::Create(context, "", mainFunc);

        builder.SetInsertPoint(mainEntryBB);

        //   %1 = alloca i32, align 4
        auto a1 = builder.CreateAlloca(builder.getInt32Ty());
        a1->setAlignment(llvm::MaybeAlign(4));

        //   %2 = alloca i64, align 8
        auto &&a2 = builder.CreateAlloca(builder.getInt64Ty());
        a2->setAlignment(llvm::MaybeAlign(8));

        //   %3 = alloca i64, align 8
        auto &&a3 = builder.CreateAlloca(builder.getInt64Ty());
        a3->setAlignment(llvm::MaybeAlign(8));

        //   %4 = alloca i64, align 8
        auto &&a4 = builder.CreateAlloca(builder.getInt64Ty());
        a4->setAlignment(llvm::MaybeAlign(8));

        //   %5 = alloca i64, align 8
        auto &&a5 = builder.CreateAlloca(builder.getInt64Ty());
        a5->setAlignment(llvm::MaybeAlign(8));

        //   %6 = alloca i64, align 8
        auto &&a6 = builder.CreateAlloca(builder.getInt64Ty());
        a6->setAlignment(llvm::MaybeAlign(8));

        //   %7 = alloca i32, align 4
        auto &&a7 = builder.CreateAlloca(builder.getInt32Ty());
        a7->setAlignment(llvm::MaybeAlign(4));

        //   store i32 0, i32* %1, align 4
        builder.CreateStore(builder.getInt32(0), a1)->setAlignment(llvm::MaybeAlign(4));

        //   store i64 1024, i64* %2, align 8
        builder.CreateStore(builder.getInt64(1024), a2)->setAlignment(llvm::MaybeAlign(8));

        //   store i64 576, i64* %3, align 8
        builder.CreateStore(builder.getInt64(576), a3)->setAlignment(llvm::MaybeAlign(8));

        //   call void @sEngine_init(i32 1024, i32 576)
        builder.CreateCall(sEngine_initF, {builder.getInt32(1024), builder.getInt32(576)});

        //   br label %8
        // 8:                                                ; preds = %37, %0
        builder.CreateBr(mainBB8);
        builder.SetInsertPoint(mainBB8);

        //   %9 = call i32 (...) @sEngine_windowIsOpen()
        auto &&a9 = builder.CreateCall(sEngine_windowIsOpenF);

        //   %10 = icmp ne i32 %9, 0
        auto &&a10 = builder.CreateICmpNE(a9, builder.getInt32(0));

        //   br i1 %10, label %11, label %38
        builder.CreateCondBr(a10, mainBB11, mainBB38);

        // 11:                                               ; preds = %8
        //   %12 = call i64 (...) @get_time_milliseconds()
        builder.SetInsertPoint(mainBB11);
        auto &&a12 = builder.CreateCall(get_time_millisecondsF);

        //   store i64 %12, i64* %4, align 8
        builder.CreateStore(a12, a4)->setAlignment(llvm::MaybeAlign(8));

        //   store i64 0, i64* %5, align 8
        builder.CreateStore(builder.getInt64(0), a5)->setAlignment(llvm::MaybeAlign(8));

        //   br label %13
        builder.CreateBr(mainBB13);
        //
        // 13:                                               ; preds = %34, %11
        builder.SetInsertPoint(mainBB13);
        //   %14 = load i64, i64* %5, align 8
        auto &&a14 = builder.CreateLoad(a5);
        //   %15 = icmp slt i64 %14, 576
        auto &&a15 = builder.CreateICmpSLT(a14, builder.getInt64(576));
        //   br i1 %15, label %16, label %37
        builder.CreateCondBr(a15, mainBB16, mainBB37);

        // 16:                                               ; preds = %13
        builder.SetInsertPoint(mainBB16);
        //   store i64 0, i64* %6, align 8
        builder.CreateStore(builder.getInt64(9), a6)->setAlignment(llvm::MaybeAlign(8));
        //   br label %17

        // 17:                                               ; preds = %30, %16
        builder.SetInsertPoint(mainBB17);
        //   %18 = load i64, i64* %6, align 8
        auto &&a18 = builder.CreateLoad(a6);
        a18->setAlignment(llvm::MaybeAlign(8));
        //   %19 = icmp slt i64 %18, 1024
        auto &&a19 = builder.CreateICmpSLT(a18, builder.getInt64(1024));
        //   br i1 %19, label %20, label %33
        builder.CreateCondBr(a19, mainBB20, mainBB33);
        //
        // 20:                                               ; preds = %17
        builder.SetInsertPoint(mainBB20);
        //   %21 = load i64, i64* %6, align 8
        auto &&a21 = builder.CreateLoad(a6);
        a21->setAlignment(llvm::MaybeAlign(8));
        //   %22 = load i64, i64* %5, align 8
        auto &&a22 = builder.CreateLoad(a5);
        a22->setAlignment(llvm::MaybeAlign(8));
        //   %23 = load i64, i64* %4, align 8
        auto &&a23 = builder.CreateLoad(a4);
        a23->setAlignment(llvm::MaybeAlign(8));
        //   %24 = call i32 @shader_1(i64 %21, i64 %22, i64 1024, i64 576, i64 %23)
        auto &&a24 = builder.CreateCall(shader_1F, {a21, a22, builder.getInt64(1024), builder.getInt64(576), a23});
        //   store i32 %24, i32* %7, align 4
        builder.CreateStore(a24, a7)->setAlignment(llvm::MaybeAlign{4});
        //   %25 = load i64, i64* %6, align 8
        auto &&a25 = builder.CreateLoad(a6);
        a25->setAlignment(llvm::MaybeAlign(8));
        //   %26 = trunc i64 %25 to i32
        auto &&a26 = builder.CreateTrunc(a25, builder.getInt32Ty());
        //   %27 = load i64, i64* %5, align 8
        auto &&a27 = builder.CreateLoad(a5);
        a27->setAlignment(llvm::MaybeAlign(8));
        //   %28 = trunc i64 %27 to i32
        auto &&a28 = builder.CreateTrunc(a27, builder.getInt32Ty());
        //   %29 = load i32, i32* %7, align 4
        auto &&a29 = builder.CreateLoad(a7);
        a29->setAlignment(llvm::MaybeAlign(4));
        //   call void @put_pixel(i32 %26, i32 %28, i32 %29)
        auto &&put_pixelTy = llvm::FunctionType::get(builder.getVoidTy(), {builder.getInt32Ty(), builder.getInt32Ty(), builder.getInt32Ty()}, false);
        //   br label %30
        builder.CreateBr(mainBB30);

        // 30:                                               ; preds = %20
        builder.SetInsertPoint(mainBB30);
        //   %31 = load i64, i64* %6, align 8
        auto &&a31 = builder.CreateLoad(a6);
        a31->setAlignment(llvm::MaybeAlign(8));
        //   %32 = add nsw i64 %31, 1
        auto &&a32 = builder.CreateAdd(a31, builder.getInt64(1), "", false, true);
        //   store i64 %32, i64* %6, align 8
        builder.CreateStore(a32, a6)->setAlignment(llvm::MaybeAlign(8));
        //   br label %17
        builder.CreateBr(mainBB17);
        //
        // 33:                                               ; preds = %17
        builder.SetInsertPoint(mainBB33);
        builder.CreateBr(mainBB34);
        //   br label %34

        // 34:                                               ; preds = %33
        builder.SetInsertPoint(mainBB34);
        //   %35 = load i64, i64* %5, align 8
        auto &&a35 = builder.CreateLoad(a5);
        a35->setAlignment(llvm::MaybeAlign(8));
        //   %36 = add nsw i64 %35, 1
        auto &&a36 = builder.CreateAdd(a35, builder.getInt64(1), "", false, true);
        //   store i64 %36, i64* %5, align 8
        builder.CreateStore(a36, a5)->setAlignment(llvm::MaybeAlign(8));
        //   br label %13
        builder.CreateBr(mainBB13);

        // 37:                                               ; preds = %13
        builder.SetInsertPoint(mainBB37);
        //   call void (...) @flush()
        builder.CreateCall(flushF);
        //   br label %8
        builder.CreateBr(mainBB8);

        // 38:                                               ; preds = %8
        builder.SetInsertPoint(mainBB38);
        builder.CreateRet(builder.getInt32(0));
        //   ret i32 0
        // } ; main end
    }

    // declare dso_local void @sEngine_init(i32, i32)
    //
    // declare dso_local i32 @sEngine_windowIsOpen(...)
    //
    // declare dso_local i64 @get_time_milliseconds(...)

    // define dso_local i32 @shader_1(i64 %0, i64 %1, i64 %2, i64 %3, i64 %4)  {
    {
        llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB59 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB63 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB94 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB95 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB96 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB99 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB103 = llvm::BasicBlock::Create(context, "", shader_1F);
        llvm::BasicBlock *BB128 = llvm::BasicBlock::Create(context, "", shader_1F);

        builder.SetInsertPoint(entryBB);
        //   %6 = alloca i64, align 8
        auto &&a6 = builder.CreateAlloca(builder.getInt64Ty());
        a6->setAlignment(llvm::MaybeAlign(8));
        //   %7 = alloca i64, align 8
        auto &&a7 = builder.CreateAlloca(builder.getInt64Ty());
        a7->setAlignment(llvm::MaybeAlign(8));
        //   %8 = alloca i64, align 8
        auto &&a8 = builder.CreateAlloca(builder.getInt64Ty());
        a8->setAlignment(llvm::MaybeAlign(8));
        //   %9 = alloca i64, align 8
        auto &&a9 = builder.CreateAlloca(builder.getInt64Ty());
        a9->setAlignment(llvm::MaybeAlign(8));
        //   %10 = alloca i64, align 8
        auto &&a10 = builder.CreateAlloca(builder.getInt64Ty());
        a10->setAlignment(llvm::MaybeAlign(8));
        //   %11 = alloca i32, align 4
        auto &&a11 = builder.CreateAlloca(builder.getInt32Ty());
        a11->setAlignment(llvm::MaybeAlign(4));
        //   %12 = alloca i64, align 8
        auto &&a12 = builder.CreateAlloca(builder.getInt64Ty());
        a12->setAlignment(llvm::MaybeAlign(8));
        //   %13 = alloca i64, align 8
        auto &&a13 = builder.CreateAlloca(builder.getInt64Ty());
        a13->setAlignment(llvm::MaybeAlign(8));
        //   %14 = alloca i32, align 4
        auto &&a14 = builder.CreateAlloca(builder.getInt32Ty());
        a14->setAlignment(llvm::MaybeAlign(4));
        //   %15 = alloca i64, align 8
        auto &&a15 = builder.CreateAlloca(builder.getInt64Ty());
        a15->setAlignment(llvm::MaybeAlign(8));
        //   %16 = alloca i64, align 8
        auto &&a16 = builder.CreateAlloca(builder.getInt64Ty());
        a16->setAlignment(llvm::MaybeAlign(8));
        //   %17 = alloca i64, align 8
        auto &&a17 = builder.CreateAlloca(builder.getInt64Ty());
        a17->setAlignment(llvm::MaybeAlign(8));
        //   %18 = alloca i64, align 8
        auto &&a18 = builder.CreateAlloca(builder.getInt64Ty());
        a18->setAlignment(llvm::MaybeAlign(8));
        //   %19 = alloca i64, align 8
        auto &&a19 = builder.CreateAlloca(builder.getInt64Ty());
        a19->setAlignment(llvm::MaybeAlign(8));
        //   %20 = alloca i32, align 4
        auto &&a20 = builder.CreateAlloca(builder.getInt32Ty());
        a20->setAlignment(llvm::MaybeAlign(4));
        //   %21 = alloca i64, align 8
        auto &&a21 = builder.CreateAlloca(builder.getInt64Ty());
        a21->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %0, i64* %6, align 8
        builder.CreateStore(shader_1F->getArg(0), a6)->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %1, i64* %7, align 8
        builder.CreateStore(shader_1F->getArg(1), a7)->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %2, i64* %8, align 8
        builder.CreateStore(shader_1F->getArg(2), a8)->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %3, i64* %9, align 8
        builder.CreateStore(shader_1F->getArg(3), a9)->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %4, i64* %10, align 8
        builder.CreateStore(shader_1F->getArg(4), a10)->setAlignment(llvm::MaybeAlign(8));
        //   store i32 255, i32* %11, align 4
        builder.CreateStore(builder.getInt32(255), a11)->setAlignment(llvm::MaybeAlign(4));
        //   %22 = load i64, i64* %6, align 8
        auto &&a22 = builder.CreateLoad(a6);
        a22->setAlignment(llvm::MaybeAlign(8));
        //   %23 = mul nsw i64 %22, 1000
        auto &&a23 = builder.CreateNSWMul(a22, builder.getInt64(1000));
        //   %24 = load i64, i64* %8, align 8
        auto &&a24 = builder.CreateLoad(a8);
        a24->setAlignment(llvm::MaybeAlign(8));
        //   %25 = sdiv i64 %23, %24
        auto &&a25 = builder.CreateSDiv(a23, a24);
        //   store i64 %25, i64* %12, align 8
        builder.CreateStore(a25, a12)->setAlignment(llvm::MaybeAlign(8));
        //   %26 = load i64, i64* %7, align 8
        auto &&a26 = builder.CreateLoad(a7);
        a26->setAlignment(llvm::MaybeAlign(8));
        //   %27 = mul nsw i64 %26, 1000
        auto &&a27 = builder.CreateNSWMul(a26, builder.getInt64(1000));
        //   %28 = load i64, i64* %9, align 8
        auto &&a28 = builder.CreateLoad(a9);
        a28->setAlignment(llvm::MaybeAlign(8));
        //   %29 = sdiv i64 %27, %28
        auto &&a29 = builder.CreateSDiv(a27, a28);
        //   store i64 %29, i64* %13, align 8
        builder.CreateStore(a29, a13)->setAlignment(llvm::MaybeAlign(8));
        //   %30 = load i64, i64* %10, align 8
        auto &&a30 = builder.CreateLoad(a10);
        a30->setAlignment(llvm::MaybeAlign(0));
        //   %31 = sdiv i64 %30, 2
        auto &&a31 = builder.CreateSDiv(a30, builder.getInt64(2));
        //   store i64 %31, i64* %10, align 8
        builder.CreateStore(a31, a10)->setAlignment(llvm::MaybeAlign(8));
        //   store i32 75, i32* %14, align 4
        builder.CreateStore(builder.getInt32(75), a14)->setAlignment(llvm::MaybeAlign(4));
        //   %32 = load i64, i64* %10, align 8
        auto &&a32 = builder.CreateLoad(a10);
        a32->setAlignment(llvm::MaybeAlign(8));
        //   %33 = sdiv i64 %32, 3
        auto &&a33 = builder.CreateSDiv(a32, builder.getInt64(3));
        //   %34 = sub nsw i64 %33, 2000
        auto &&a34 = builder.CreateNSWSub(a33, builder.getInt64(2000));
        //   %35 = call i64 @int_cos(i64 %34)
        auto &&a35 = builder.CreateCall(int_cosF, {a34});
        //   %36 = sdiv i64 %35, 5
        auto &&a36 = builder.CreateSDiv(a35, builder.getInt64(5));
        //   %37 = add nsw i64 %36, 850
        auto &&a37 = builder.CreateNSWAdd(a36, builder.getInt64(850));
        //   %38 = mul nsw i64 788, %37
        auto &&a38 = builder.CreateNSWMul(builder.getInt64(788), a37);
        //   %39 = sdiv i64 %38, 1000
        auto &&a39 = builder.CreateSDiv(a38, builder.getInt64(1000));
        //   store i64 %39, i64* %15, align 8
        builder.CreateStore(a39, a15)->setAlignment(llvm::MaybeAlign(8));
        //   %40 = load i64, i64* %15, align 8
        auto &&a40 = builder.CreateLoad(a15);
        a40->setAlignment(llvm::MaybeAlign(8));
        //   %41 = load i64, i64* %10, align 8
        auto &&a41 = builder.CreateLoad(a10);
        a41->setAlignment(llvm::MaybeAlign(8));
        //   %42 = sdiv i64 %41, 3
        auto &&a42 = builder.CreateSDiv(a41, builder.getInt64(3));
        //   %43 = call i64 @int_cos(i64 %42)
        auto &&a43 = builder.CreateCall(int_cosF, {a42});
        //   %44 = mul nsw i64 %40, %43
        auto &&a44 = builder.CreateNSWMul(a40, a43);
        //   %45 = sdiv i64 %44, 1000
        auto &&a45 = builder.CreateSDiv(a44, builder.getInt64(1000));
        //   store i64 %45, i64* %16, align 8
        builder.CreateStore(a45, a16)->setAlignment(llvm::MaybeAlign(8));
        //   %46 = load i64, i64* %15, align 8
        auto &&a46 = builder.CreateLoad(a15);
        a46->setAlignment(llvm::MaybeAlign(8));
        //   %47 = load i64, i64* %10, align 8
        auto &&a47 = builder.CreateLoad(a10);
        a47->setAlignment(llvm::MaybeAlign(8));
        //   %48 = sdiv i64 %47, 3
        auto &&a48 = builder.CreateSDiv(a47, builder.getInt64(3));
        //   %49 = add nsw i64 %48, 3000
        auto &&a49 = builder.CreateNSWAdd(a48, builder.getInt64(3000));
        //   %50 = call i64 @int_cos(i64 %49)
        auto &&a50 = builder.CreateCall(int_cosF, {a49});
        //   %51 = mul nsw i64 %46, %50
        auto &&a51 = builder.CreateNSWMul(a46, a50);
        //   %52 = sdiv i64 %51, 1000
        auto &&a52 = builder.CreateSDiv(a51, builder.getInt64(1000));
        //   store i64 %52, i64* %17, align 8
        builder.CreateStore(a52, a17)->setAlignment(llvm::MaybeAlign(8));
        //   %53 = load i64, i64* %12, align 8
        auto &&a53 = builder.CreateLoad(a12);
        a53->setAlignment(llvm::MaybeAlign(8));
        //   %54 = sub nsw i64 %53, 500
        auto &&a54 = builder.CreateNSWMul(a53, builder.getInt64(500));
        //   %55 = mul nsw i64 3, %54
        auto &&a55 = builder.CreateNSWMul(builder.getInt64(3), a54);
        //   store i64 %55, i64* %18, align 8
        builder.CreateStore(a55, a18)->setAlignment(llvm::MaybeAlign(8));
        //   %56 = load i64, i64* %13, align 8
        auto &&a56 = builder.CreateLoad(a13);
        a56->setAlignment(llvm::MaybeAlign(8));
        //   %57 = sub nsw i64 %56, 500
        auto &&a57 = builder.CreateNSWSub(a56, builder.getInt64(500));
        //   %58 = mul nsw i64 2, %57
        auto &&a58 = builder.CreateNSWMul(builder.getInt64(2), a57);
        //   store i64 %58, i64* %19, align 8
        builder.CreateStore(a58, a19)->setAlignment(llvm::MaybeAlign(8));
        //   store i32 0, i32* %20, align 4
        builder.CreateStore(builder.getInt32(0), a20)->setAlignment(llvm::MaybeAlign(4));
        //   store i32 0, i32* %20, align 4
        builder.CreateStore(builder.getInt32(0), a20)->setAlignment(llvm::MaybeAlign(4));
        //   br label %59
        builder.CreateBr(BB59);

        // 59:                                               ; preds = %96, %5
        builder.SetInsertPoint(BB59);
        //   %60 = load i32, i32* %20, align 4
        auto &&a60 = builder.CreateLoad(a20);
        a60->setAlignment(llvm::MaybeAlign(4));
        //   %61 = load i32, i32* %14, align 4
        auto &&a61 = builder.CreateLoad(a14);
        a61->setAlignment(llvm::MaybeAlign(4));
        //   %62 = icmp ult i32 %60, %61
        auto &&a62 = builder.CreateICmpULT(a60, a61);
        //   br i1 %62, label %63, label %99
        builder.CreateCondBr(a62, BB63, BB99);
        //
        // 63:                                               ; preds = %59
        builder.SetInsertPoint(BB63);
        //   %64 = load i64, i64* %18, align 8
        auto &&a64 = builder.CreateLoad(a18);
        a64->setAlignment(llvm::MaybeAlign(8));
        //   %65 = load i64, i64* %18, align 8
        auto &&a65 = builder.CreateLoad(a18);
        a65->setAlignment(llvm::MaybeAlign(8));
        //   %66 = mul nsw i64 %64, %65
        auto &&a66 = builder.CreateNSWMul(a64, a65);
        //   %67 = load i64, i64* %19, align 8
        auto &&a67 = builder.CreateLoad(a19);
        a67->setAlignment(llvm::MaybeAlign(8));
        //   %68 = load i64, i64* %19, align 8
        auto &&a68 = builder.CreateLoad(a19);
        a68->setAlignment(llvm::MaybeAlign(8));
        //   %69 = mul nsw i64 %67, %68
        auto &&a69 = builder.CreateNSWMul(a67, a68);
        //   %70 = sub nsw i64 %66, %69
        auto &&a70 = builder.CreateNSWSub(a66, a69);
        //   %71 = sdiv i64 %70, 1000
        auto &&a71 = builder.CreateSDiv(a70, builder.getInt64(1000));
        //   %72 = load i64, i64* %16, align 8
        auto &&a72 = builder.CreateLoad(a16);
        a72->setAlignment(llvm::MaybeAlign(8));
        //   %73 = add nsw i64 %71, %72
        auto &&a73 = builder.CreateNSWAdd(a71, a72);
        //   store i64 %73, i64* %21, align 8
        builder.CreateStore(a73, a21)->setAlignment(llvm::MaybeAlign(8));
        //   %74 = load i64, i64* %19, align 8
        auto &&a74 = builder.CreateLoad(a19);
        a74->setAlignment(llvm::MaybeAlign(8));
        //   %75 = load i64, i64* %18, align 8
        auto &&a75 = builder.CreateLoad(a18);
        a75->setAlignment(llvm::MaybeAlign(8));
        //   %76 = mul nsw i64 %74, %75
        auto &&a76 = builder.CreateNSWMul(a74, a75);
        //   %77 = load i64, i64* %18, align 8
        auto &&a77 = builder.CreateLoad(a18);
        a77->setAlignment(llvm::MaybeAlign(8));
        //   %78 = load i64, i64* %19, align 8
        auto &&a78 = builder.CreateLoad(a19);
        a78->setAlignment(llvm::MaybeAlign(8));
        //   %79 = mul nsw i64 %77, %78
        auto &&a79 = builder.CreateNSWMul(a77, a78);
        //   %80 = add nsw i64 %76, %79
        auto &&a80 = builder.CreateNSWAdd(a76, a79);
        //   %81 = sdiv i64 %80, 1000
        auto &&a81 = builder.CreateSDiv(a80, builder.getInt64(1000));
        //   %82 = load i64, i64* %17, align 8
        auto &&a82 = builder.CreateLoad(a17);
        a82->setAlignment(llvm::MaybeAlign(8));
        //   %83 = add nsw i64 %81, %82
        auto &&a83 = builder.CreateNSWAdd(a81, a82);
        //   store i64 %83, i64* %19, align 8
        builder.CreateStore(a83, a19)->setAlignment(llvm::MaybeAlign(8));
        //   %84 = load i64, i64* %21, align 8
        auto &&a84 = builder.CreateLoad(a21);
        a84->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %84, i64* %18, align 8
        builder.CreateStore(a84, a18)->setAlignment(llvm::MaybeAlign(8));
        //   %85 = load i64, i64* %18, align 8
        auto &&a85 = builder.CreateLoad(a18);
        a85->setAlignment(llvm::MaybeAlign(8));
        //   %86 = load i64, i64* %18, align 8
        auto &&a86 = builder.CreateLoad(a18);
        a86->setAlignment(llvm::MaybeAlign(8));
        //   %87 = mul nsw i64 %85, %86
        auto &&a87 = builder.CreateNSWMul(a85, a86);
        //   %88 = load i64, i64* %19, align 8
        auto &&a88 = builder.CreateLoad(a19);
        a88->setAlignment(llvm::MaybeAlign(8));
        //   %89 = load i64, i64* %19, align 8
        auto &&a89 = builder.CreateLoad(a19);
        a89->setAlignment(llvm::MaybeAlign(8));
        //   %90 = mul nsw i64 %88, %89
        auto &&a90 = builder.CreateNSWMul(a88, a89);
        //   %91 = add nsw i64 %87, %90
        auto &&a91 = builder.CreateNSWAdd(a87, a90);
        //   %92 = sdiv i64 %91, 10000
        auto &&a92 = builder.CreateSDiv(a91, builder.getInt64(10000));
        //   %93 = icmp sgt i64 %92, 4000
        auto &&a93 = builder.CreateICmpSGT(a92, builder.getInt64(4000));
        //   br i1 %93, label %94, label %95
        builder.CreateCondBr(a93, BB94, BB95);

        // 94:                                               ; preds = %63
        builder.SetInsertPoint(BB94);
        //   br label %99
        builder.CreateBr(BB99);

        // 95:                                               ; preds = %63
        builder.SetInsertPoint(BB95);
        //   br label %96
        builder.CreateBr(BB96);

        // 96:                                               ; preds = %95
        builder.SetInsertPoint(BB96);
        //   %97 = load i32, i32* %20, align 4
        auto &&a97 = builder.CreateLoad(a20);
        a97->setAlignment(llvm::MaybeAlign(4));
        //   %98 = add i32 %97, 1
        auto &&a98 = builder.CreateAdd(a97, builder.getInt32(1));
        //   store i32 %98, i32* %20, align 4
        builder.CreateStore(a98, a20)->setAlignment(llvm::MaybeAlign(4));
        //   br label %59
        builder.CreateBr(BB59);

        // 99:                                               ; preds = %94, %59
        builder.SetInsertPoint(BB99);
        //   %100 = load i32, i32* %20, align 4
        auto &&a100 = builder.CreateLoad(a20);
        a100->setAlignment(llvm::MaybeAlign(4));
        //   %101 = load i32, i32* %14, align 4
        auto &&a101 = builder.CreateLoad(a14);
        a101->setAlignment(llvm::MaybeAlign(4));
        //   %102 = icmp ne i32 %100, %101
        auto &&a102 = builder.CreateICmpNE(a100, a101);
        //   br i1 %102, label %103, label %128
        builder.CreateCondBr(a102, BB103, BB128);

        // 103:                                              ; preds = %99
        builder.SetInsertPoint(BB103);
        //   %104 = load i32, i32* %20, align 4
        auto &&a104 = builder.CreateLoad(a20);
        a104->setAlignment(llvm::MaybeAlign(4));
        //   %105 = mul i32 %104, 1000
        auto &&a105 = builder.CreateMul(a104, builder.getInt32(1000));
        //   %106 = load i32, i32* %14, align 4
        auto &&a106 = builder.CreateLoad(a14);
        a106->setAlignment(llvm::MaybeAlign(4));
        //   %107 = udiv i32 %105, %106
        auto &&a107 = builder.CreateUDiv(a105, a106);
        //   %108 = zext i32 %107 to i64
        auto &&a108 = builder.CreateZExt(a107, builder.getInt64Ty());
        //   %109 = load i64, i64* %10, align 8
        auto &&a109 = builder.CreateLoad(a10);
        a10->setAlignment(llvm::MaybeAlign(8));
        //   %110 = add nsw i64 %109, 3000
        auto &&a110 = builder.CreateNSWAdd(a109, builder.getInt64(3000));
        //   %111 = call i64 @int_cos(i64 %110)
        auto &&a111 = builder.CreateCall(int_cosF, {a110});
        //   %112 = mul nsw i64 %111, 2
        auto &&a112 = builder.CreateNSWMul(a111, builder.getInt64(2));
        //   %113 = sdiv i64 %112, 5
        auto &&a113 = builder.CreateSDiv(a112, builder.getInt64(5));
        //   %114 = add nsw i64 300, %113
        auto &&a114 = builder.CreateNSWAdd(builder.getInt64(300), a113);
        //   %115 = load i64, i64* %10, align 8
        auto &&a115 = builder.CreateLoad(a10);
        a115->setAlignment(llvm::MaybeAlign(8));
        //   %116 = add nsw i64 %115, 3600
        auto &&a116 = builder.CreateNSWAdd(a115, builder.getInt64(3600));
        //   %117 = call i64 @int_cos(i64 %116)
        auto &&a117 = builder.CreateCall(int_cosF, {a116});
        //   %118 = mul nsw i64 %117, 3
        auto &&a118 = builder.CreateNSWMul(a117, builder.getInt64(3));
        //   %119 = sdiv i64 %118, 10
        auto &&a119 = builder.CreateSDiv(a118, builder.getInt64(10));
        //   %120 = add nsw i64 200, %119
        auto &&a120 = builder.CreateNSWAdd(builder.getInt64(200), a119);
        //   %121 = load i64, i64* %10, align 8
        auto &&a121 = builder.CreateLoad(a10);
        a121->setAlignment(llvm::MaybeAlign(8));
        //   %122 = add nsw i64 %121, 4400
        auto &&a122 = builder.CreateNSWAdd(a121, builder.getInt64(4400));
        //   %123 = call i64 @int_cos(i64 %122)
        auto &&a123 = builder.CreateCall(int_cosF, {a122});
        //   %124 = mul nsw i64 %123, 3
        auto &&a124 = builder.CreateNSWMul(a123, builder.getInt64(3));
        //   %125 = sdiv i64 %124, 10
        auto &&a125 = builder.CreateSDiv(a124, builder.getInt64(10));
        //   %126 = add nsw i64 200, %125
        auto &&a126 = builder.CreateNSWAdd(builder.getInt64(200), a125);
        //   %127 = call i32 @palette_int(i64 %108, i64 %114, i64 %120, i64 %126)
        auto &&a127 = builder.CreateCall(palette_intF, {a108, a114, a120, a126});
        //   store i32 %127, i32* %11, align 4
        builder.CreateStore(a127, a11)->setAlignment(llvm::MaybeAlign(4));
        //   br label %128
        builder.CreateBr(BB128);

        // 128:                                              ; preds = %103, %99
        builder.SetInsertPoint(BB128);
        //   %129 = load i32, i32* %11, align 4
        auto &&a129 = builder.CreateLoad(a11);
        a129->setAlignment(llvm::MaybeAlign(4));
        //   ret i32 %129
        builder.CreateRet(a129);
        // }
    }

    // declare dso_local void @put_pixel(i32, i32, i32) #1
    //
    // declare dso_local void @flush(...) #1
    //
    {
        // define dso_local i32 @pack_4ui8_to_ui32(i8 zeroext %0, i8 zeroext %1, i8 zeroext %2, i8 zeroext %3)  {
        llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(context, "", pack_4ui8_to_ui32F);

        builder.SetInsertPoint(EntryBB);
        //   %5 = alloca i8, align 1
        auto &&a5 = builder.CreateAlloca(builder.getInt8Ty());
        a5->setAlignment(llvm::MaybeAlign(1));
        //   %6 = alloca i8, align 1
        auto &&a6 = builder.CreateAlloca(builder.getInt8Ty());
        a6->setAlignment(llvm::MaybeAlign(1));
        //   %7 = alloca i8, align 1
        auto &&a7 = builder.CreateAlloca(builder.getInt8Ty());
        a7->setAlignment(llvm::MaybeAlign(1));
        //   %8 = alloca i8, align 1
        auto &&a8 = builder.CreateAlloca(builder.getInt8Ty());
        a8->setAlignment(llvm::MaybeAlign(1));
        //   %9 = alloca i32, align 4
        auto &&a9 = builder.CreateAlloca(builder.getInt32Ty());
        a9->setAlignment(llvm::MaybeAlign(4));
        //   store i8 %0, i8* %5, align 1
        builder.CreateStore(pack_4ui8_to_ui32F->getArg(0), a5)->setAlignment(llvm::MaybeAlign(1));
        //   store i8 %1, i8* %6, align 1
        builder.CreateStore(pack_4ui8_to_ui32F->getArg(1), a6)->setAlignment(llvm::MaybeAlign(1));
        //   store i8 %2, i8* %7, align 1
        builder.CreateStore(pack_4ui8_to_ui32F->getArg(2), a7)->setAlignment(llvm::MaybeAlign(1));
        //   store i8 %3, i8* %8, align 1
        builder.CreateStore(pack_4ui8_to_ui32F->getArg(3), a8)->setAlignment(llvm::MaybeAlign(1));
        //   store i32 0, i32* %9, align 4
        builder.CreateStore(builder.getInt32(0), a9)->setAlignment(llvm::MaybeAlign(4));
        //   %10 = load i32, i32* %9, align 4
        auto &&a10 = builder.CreateLoad(a9);
        a9->setAlignment(llvm::MaybeAlign(4));
        //   %11 = load i8, i8* %8, align 1
        auto &&a11 = builder.CreateLoad(a8);
        a11->setAlignment(llvm::MaybeAlign(1));
        //   %12 = zext i8 %11 to i32
        auto &&a12 = builder.CreateZExt(a11, builder.getInt32Ty());
        //   %13 = or i32 %10, %12
        auto &&a13 = builder.CreateOr(a10, a12);
        //   store i32 %13, i32* %9, align 4
        builder.CreateStore(a13, a9)->setAlignment(llvm::MaybeAlign(4));
        //   %14 = load i32, i32* %9, align 4
        auto &&a14 = builder.CreateLoad(a9);
        a14->setAlignment(llvm::MaybeAlign(4));
        //   %15 = load i8, i8* %7, align 1
        auto &&a15 = builder.CreateLoad(a7);
        a15->setAlignment(llvm::MaybeAlign(1));
        //   %16 = zext i8 %15 to i32
        auto &&a16 = builder.CreateZExt(a15, builder.getInt32Ty());
        //   %17 = shl i32 %16, 8
        auto &&a17 = builder.CreateShl(a16, 8);
        //   %18 = or i32 %14, %17
        auto &&a18 = builder.CreateOr(a14, a17);
        //   store i32 %18, i32* %9, align 4
        builder.CreateStore(a18, a9)->setAlignment(llvm::MaybeAlign(4));
        //   %19 = load i32, i32* %9, align 4
        auto &&a19 = builder.CreateLoad(a9);
        a19->setAlignment(llvm::MaybeAlign(4));
        //   %20 = load i8, i8* %6, align 1
        auto &&a20 = builder.CreateLoad(a6);
        a20->setAlignment(llvm::MaybeAlign(4));
        //   %21 = zext i8 %20 to i32
        auto &&a21 = builder.CreateZExt(a20, builder.getInt32Ty());
        //   %22 = shl i32 %21, 16
        auto &&a22 = builder.CreateShl(a21, 16);
        //   %23 = or i32 %19, %22
        auto &&a23 = builder.CreateOr(a19, a22);
        //   store i32 %23, i32* %9, align 4
        builder.CreateStore(a23, a9)->setAlignment(llvm::MaybeAlign(4));
        //   %24 = load i32, i32* %9, align 4
        auto &&a24 = builder.CreateLoad(a9);
        a24->setAlignment(llvm::MaybeAlign(4));
        //   %25 = load i8, i8* %5, align 1
        auto &&a25 = builder.CreateLoad(a5);
        a25->setAlignment(llvm::MaybeAlign(1));
        //   %26 = zext i8 %25 to i32
        auto &&a26 = builder.CreateZExt(a25, builder.getInt32Ty());
        //   %27 = shl i32 %26, 24
        auto &&a27 = builder.CreateShl(a26, 24);
        //   %28 = or i32 %24, %27
        auto &&a28 = builder.CreateOr(a24, a27);
        //   store i32 %28, i32* %9, align 4
        builder.CreateStore(a28, a9)->setAlignment(llvm::MaybeAlign(4));
        //   %29 = load i32, i32* %9, align 4
        auto &&a29 = builder.CreateLoad(a9);
        a29->setAlignment(llvm::MaybeAlign(4));
        //   ret i32 %29
        builder.CreateRet(a29);
        // }
    }

    {
        // define dso_local i64 @abs_dummy(i64 %0)  {
        llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(context, "", abs_dummyF);
        llvm::BasicBlock *BB5 = llvm::BasicBlock::Create(context, "", abs_dummyF);
        llvm::BasicBlock *BB8 = llvm::BasicBlock::Create(context, "", abs_dummyF);

        builder.SetInsertPoint(EntryBB);
        //   %2 = alloca i64, align 8
        auto &&a2 = builder.CreateAlloca(builder.getInt64Ty());
        a2->setAlignment(llvm::MaybeAlign(8));
        //   store i64 %0, i64* %2, align 8
        builder.CreateStore(abs_dummyF->getArg(0), a2)->setAlignment(llvm::MaybeAlign(8));
        //   %3 = load i64, i64* %2, align 8
        auto &&a3 = builder.CreateLoad(a2);
        a2->setAlignment(llvm::MaybeAlign(8));
        //   %4 = icmp slt i64 %3, 0
        auto &&a4 = builder.CreateICmpSLT(a3, builder.getInt64(0));
        //   br i1 %4, label %5, label %8
        builder.CreateCondBr(a4, BB5, BB8);

        // 5:                                                ; preds = %1
        builder.SetInsertPoint(BB5);
        //   %6 = load i64, i64* %2, align 8
        auto &&a6 = builder.CreateLoad(a2);
        a6->setAlignment(llvm::MaybeAlign(8));
        //   %7 = sub nsw i64 0, %6
        auto &&a7 = builder.CreateNSWSub(builder.getInt64(0), a6);
        //   store i64 %7, i64* %2, align 8
        builder.CreateStore(a7, a2)->setAlignment(llvm::MaybeAlign(8));
        //   br label %8
        builder.CreateBr(BB8);
        //
        // 8:                                                ; preds = %5, %1
        builder.SetInsertPoint(BB8);
        //   %9 = load i64, i64* %2, align 8
        auto &&a9 = builder.CreateLoad(a2);
        a9->setAlignment(llvm::MaybeAlign(8));
        //   ret i64 %9
        builder.CreateRet(a9);
        // }
    }

    {
        llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(context, "", int_cosF);
        llvm::BasicBlock *BB16 = llvm::BasicBlock::Create(context, "", int_cosF);
        llvm::BasicBlock *BB19 = llvm::BasicBlock::Create(context, "", int_cosF);
        llvm::BasicBlock *BB22 = llvm::BasicBlock::Create(context, "", int_cosF);
        llvm::BasicBlock *BB25 = llvm::BasicBlock::Create(context, "", int_cosF);

        // define dso_local i64 @int_cos(i64 %0)  {
        builder.SetInsertPoint(EntryBB);
        //   %2 = alloca i64, align 8
        //   %3 = alloca i64, align 8
        //   %4 = alloca i64, align 8
        //   %5 = alloca i64, align 8
        //   %6 = alloca i64, align 8
        //   store i64 %0, i64* %2, align 8
        //   %7 = load i64, i64* %2, align 8
        //   %8 = srem i64 %7, 1000
        //   store i64 %8, i64* %3, align 8
        //   %9 = load i64, i64* %2, align 8
        //   %10 = call i64 @abs_dummy(i64 %9)
        //   %11 = srem i64 %10, 4000
        //   %12 = sdiv i64 %11, 1000
        //   store i64 %12, i64* %4, align 8
        //   %13 = load i64, i64* %4, align 8
        //   %14 = srem i64 %13, 2
        //   %15 = icmp ne i64 %14, 0
        //   br i1 %15, label %16, label %19
        //
        // 16:                                               ; preds = %1
        //   %17 = load i64, i64* %3, align 8
        //   %18 = sub nsw i64 1000, %17
        //   store i64 %18, i64* %3, align 8
        //   br label %19
        //
        // 19:                                               ; preds = %16, %1
        //   %20 = load i64, i64* %4, align 8
        //   %21 = icmp eq i64 %20, 1
        //   br i1 %21, label %25, label %22
        //
        // 22:                                               ; preds = %19
        //   %23 = load i64, i64* %4, align 8
        //   %24 = icmp eq i64 %23, 2
        //   br label %25
        //
        // 25:                                               ; preds = %22, %19
        //   %26 = phi i1 [ true, %19 ], [ %24, %22 ]
        //   %27 = zext i1 %26 to i64
        //   %28 = select i1 %26, i32 1, i32 -1
        //   %29 = sext i32 %28 to i64
        //   store i64 %29, i64* %5, align 8
        //   %30 = load i64, i64* %3, align 8
        //   %31 = load i64, i64* %3, align 8
        //   %32 = mul nsw i64 %30, %31
        //   %33 = sdiv i64 %32, 1000
        //   %34 = sub nsw i64 %33, 1000
        //   store i64 %34, i64* %6, align 8
        //   %35 = load i64, i64* %6, align 8
        //   %36 = load i64, i64* %5, align 8
        //   %37 = mul nsw i64 %35, %36
        //   ret i64 %37
        // }
    }
    //
    // define dso_local i32 @palette_int(i64 %0, i64 %1, i64 %2, i64 %3)  {
    //   %5 = alloca i64, align 8
    //   %6 = alloca i64, align 8
    //   %7 = alloca i64, align 8
    //   %8 = alloca i64, align 8
    //   %9 = alloca i8, align 1
    //   %10 = alloca i8, align 1
    //   %11 = alloca i8, align 1
    //   store i64 %0, i64* %5, align 8
    //   store i64 %1, i64* %6, align 8
    //   store i64 %2, i64* %7, align 8
    //   store i64 %3, i64* %8, align 8
    //   %12 = load i64, i64* %5, align 8
    //   %13 = mul nsw i64 1000, %12
    //   %14 = sdiv i64 %13, 1000
    //   %15 = load i64, i64* %6, align 8
    //   %16 = add nsw i64 %14, %15
    //   %17 = mul nsw i64 628318, %16
    //   %18 = sdiv i64 %17, 100000
    //   %19 = call i64 @int_cos(i64 %18)
    //   %20 = mul nsw i64 500, %19
    //   %21 = sdiv i64 %20, 1000
    //   %22 = add nsw i64 500, %21
    //   %23 = sdiv i64 %22, 5
    //   %24 = trunc i64 %23 to i8
    //   store i8 %24, i8* %9, align 1
    //   %25 = load i64, i64* %5, align 8
    //   %26 = mul nsw i64 1000, %25
    //   %27 = sdiv i64 %26, 1000
    //   %28 = load i64, i64* %7, align 8
    //   %29 = add nsw i64 %27, %28
    //   %30 = mul nsw i64 628318, %29
    //   %31 = sdiv i64 %30, 100000
    //   %32 = call i64 @int_cos(i64 %31)
    //   %33 = mul nsw i64 500, %32
    //   %34 = sdiv i64 %33, 1000
    //   %35 = add nsw i64 500, %34
    //   %36 = sdiv i64 %35, 5
    //   %37 = trunc i64 %36 to i8
    //   store i8 %37, i8* %10, align 1
    //   %38 = load i64, i64* %5, align 8
    //   %39 = mul nsw i64 0, %38
    //   %40 = sdiv i64 %39, 1000
    //   %41 = load i64, i64* %8, align 8
    //   %42 = add nsw i64 %40, %41
    //   %43 = mul nsw i64 628318, %42
    //   %44 = sdiv i64 %43, 100000
    //   %45 = call i64 @int_cos(i64 %44)
    //   %46 = mul nsw i64 500, %45
    //   %47 = sdiv i64 %46, 1000
    //   %48 = add nsw i64 500, %47
    //   %49 = sdiv i64 %48, 5
    //   %50 = trunc i64 %49 to i8
    //   store i8 %50, i8* %11, align 1
    //   %51 = load i8, i8* %9, align 1
    //   %52 = load i8, i8* %10, align 1
    //   %53 = load i8, i8* %11, align 1
    //   %54 = load i64, i64* %5, align 8
    //   %55 = sdiv i64 %54, 4
    //   %56 = trunc i64 %55 to i8
    //   %57 = call i32 @pack_4ui8_to_ui32(i8 zeroext %51, i8 zeroext %52, i8 zeroext %53, i8 zeroext %56)
    //   ret i32 %57
    // }

    // Dump LLVM IR
    //=------------
    std::string foutName = "main_gen.ll";
    std::cout << "Dump to: " << foutName << std::endl;
    std::error_code EC;
    llvm::raw_fd_ostream file(foutName, EC);
    std::cout << "Dump status: " << EC.message() << std::endl;
    module->print(file, nullptr);
    file.close();

    return 0;
}
