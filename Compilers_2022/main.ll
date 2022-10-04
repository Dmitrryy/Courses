; ModuleID = './main.cpp'
source_filename = "./main.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

$_ZSt3absl = comdat any

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i64 1024, i64* %2, align 8
  store i64 576, i64* %3, align 8
  call void @_Z12sEngine_initjj(i32 1024, i32 576)
  br label %8

8:                                                ; preds = %43, %0
  %9 = call zeroext i1 @_Z20sEngine_windowIsOpenv()
  br i1 %9, label %10, label %44

10:                                               ; preds = %8
  %11 = call i64 @get_time_milliseconds()
  store i64 %11, i64* %4, align 8
  store i64 0, i64* %5, align 8
  br label %12

12:                                               ; preds = %40, %10
  %13 = load i64, i64* %5, align 8
  %14 = icmp slt i64 %13, 576
  br i1 %14, label %15, label %43

15:                                               ; preds = %12
  store i64 0, i64* %6, align 8
  br label %16

16:                                               ; preds = %36, %15
  %17 = load i64, i64* %6, align 8
  %18 = icmp slt i64 %17, 1024
  br i1 %18, label %19, label %39

19:                                               ; preds = %16
  %20 = load i64, i64* %6, align 8
  %21 = load i64, i64* %5, align 8
  %22 = load i64, i64* %4, align 8
  %23 = call i32 @_Z8shader_1lllll(i64 %20, i64 %21, i64 1024, i64 576, i64 %22)
  store i32 %23, i32* %7, align 4
  %24 = load i64, i64* %6, align 8
  %25 = trunc i64 %24 to i32
  %26 = load i64, i64* %5, align 8
  %27 = trunc i64 %26 to i32
  %28 = load i32, i32* %7, align 4
  %29 = call zeroext i8 @_Z13unpack_4ui8_rj(i32 %28)
  %30 = load i32, i32* %7, align 4
  %31 = call zeroext i8 @_Z13unpack_4ui8_gj(i32 %30)
  %32 = load i32, i32* %7, align 4
  %33 = call zeroext i8 @_Z13unpack_4ui8_bj(i32 %32)
  %34 = load i32, i32* %7, align 4
  %35 = call zeroext i8 @_Z13unpack_4ui8_aj(i32 %34)
  call void @put_pixel(i32 %25, i32 %27, i8 zeroext %29, i8 zeroext %31, i8 zeroext %33, i8 zeroext %35)
  br label %36

36:                                               ; preds = %19
  %37 = load i64, i64* %6, align 8
  %38 = add nsw i64 %37, 1
  store i64 %38, i64* %6, align 8
  br label %16

39:                                               ; preds = %16
  br label %40

40:                                               ; preds = %39
  %41 = load i64, i64* %5, align 8
  %42 = add nsw i64 %41, 1
  store i64 %42, i64* %5, align 8
  br label %12

43:                                               ; preds = %12
  call void @flush()
  br label %8

44:                                               ; preds = %8
  ret i32 0
}

declare dso_local void @_Z12sEngine_initjj(i32, i32) #1

declare dso_local zeroext i1 @_Z20sEngine_windowIsOpenv() #1

declare dso_local i64 @get_time_milliseconds() #1

; Function Attrs: noinline optnone uwtable
define dso_local i32 @_Z8shader_1lllll(i64 %0, i64 %1, i64 %2, i64 %3, i64 %4) #2 {
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i32, align 4
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %14 = alloca i32, align 4
  %15 = alloca i64, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64, align 8
  %19 = alloca i64, align 8
  %20 = alloca i8, align 1
  %21 = alloca i32, align 4
  %22 = alloca i64, align 8
  %23 = alloca i32, align 4
  store i64 %0, i64* %6, align 8
  store i64 %1, i64* %7, align 8
  store i64 %2, i64* %8, align 8
  store i64 %3, i64* %9, align 8
  store i64 %4, i64* %10, align 8
  store i32 255, i32* %11, align 4
  %24 = load i64, i64* %6, align 8
  %25 = mul nsw i64 %24, 1000
  %26 = load i64, i64* %8, align 8
  %27 = sdiv i64 %25, %26
  store i64 %27, i64* %12, align 8
  %28 = load i64, i64* %7, align 8
  %29 = mul nsw i64 %28, 1000
  %30 = load i64, i64* %9, align 8
  %31 = sdiv i64 %29, %30
  store i64 %31, i64* %13, align 8
  %32 = load i64, i64* %10, align 8
  %33 = sdiv i64 %32, 2
  store i64 %33, i64* %10, align 8
  store i32 75, i32* %14, align 4
  %34 = load i64, i64* %10, align 8
  %35 = sdiv i64 %34, 3
  %36 = sub nsw i64 %35, 2000
  %37 = call i64 @_Z7int_cosl(i64 %36)
  %38 = sdiv i64 %37, 5
  %39 = add nsw i64 %38, 850
  %40 = mul nsw i64 788, %39
  %41 = sdiv i64 %40, 1000
  store i64 %41, i64* %15, align 8
  %42 = load i64, i64* %15, align 8
  %43 = load i64, i64* %10, align 8
  %44 = sdiv i64 %43, 3
  %45 = call i64 @_Z7int_cosl(i64 %44)
  %46 = mul nsw i64 %42, %45
  %47 = sdiv i64 %46, 1000
  store i64 %47, i64* %16, align 8
  %48 = load i64, i64* %15, align 8
  %49 = load i64, i64* %10, align 8
  %50 = sdiv i64 %49, 3
  %51 = add nsw i64 %50, 3000
  %52 = call i64 @_Z7int_cosl(i64 %51)
  %53 = mul nsw i64 %48, %52
  %54 = sdiv i64 %53, 1000
  store i64 %54, i64* %17, align 8
  %55 = load i64, i64* %12, align 8
  %56 = sub nsw i64 %55, 500
  %57 = mul nsw i64 3, %56
  store i64 %57, i64* %18, align 8
  %58 = load i64, i64* %13, align 8
  %59 = sub nsw i64 %58, 500
  %60 = mul nsw i64 2, %59
  store i64 %60, i64* %19, align 8
  store i8 0, i8* %20, align 1
  store i32 0, i32* %21, align 4
  br label %61

61:                                               ; preds = %98, %5
  %62 = load i32, i32* %21, align 4
  %63 = load i32, i32* %14, align 4
  %64 = icmp ult i32 %62, %63
  br i1 %64, label %65, label %101

65:                                               ; preds = %61
  %66 = load i64, i64* %18, align 8
  %67 = load i64, i64* %18, align 8
  %68 = mul nsw i64 %66, %67
  %69 = load i64, i64* %19, align 8
  %70 = load i64, i64* %19, align 8
  %71 = mul nsw i64 %69, %70
  %72 = sub nsw i64 %68, %71
  %73 = sdiv i64 %72, 1000
  %74 = load i64, i64* %16, align 8
  %75 = add nsw i64 %73, %74
  store i64 %75, i64* %22, align 8
  %76 = load i64, i64* %19, align 8
  %77 = load i64, i64* %18, align 8
  %78 = mul nsw i64 %76, %77
  %79 = load i64, i64* %18, align 8
  %80 = load i64, i64* %19, align 8
  %81 = mul nsw i64 %79, %80
  %82 = add nsw i64 %78, %81
  %83 = sdiv i64 %82, 1000
  %84 = load i64, i64* %17, align 8
  %85 = add nsw i64 %83, %84
  store i64 %85, i64* %19, align 8
  %86 = load i64, i64* %22, align 8
  store i64 %86, i64* %18, align 8
  %87 = load i64, i64* %18, align 8
  %88 = load i64, i64* %18, align 8
  %89 = mul nsw i64 %87, %88
  %90 = load i64, i64* %19, align 8
  %91 = load i64, i64* %19, align 8
  %92 = mul nsw i64 %90, %91
  %93 = add nsw i64 %89, %92
  %94 = sdiv i64 %93, 10000
  %95 = icmp sgt i64 %94, 4000
  br i1 %95, label %96, label %97

96:                                               ; preds = %65
  store i8 1, i8* %20, align 1
  br label %101

97:                                               ; preds = %65
  br label %98

98:                                               ; preds = %97
  %99 = load i32, i32* %21, align 4
  %100 = add i32 %99, 1
  store i32 %100, i32* %21, align 4
  br label %61

101:                                              ; preds = %96, %61
  %102 = load i32, i32* %21, align 4
  %103 = mul i32 %102, 1000
  %104 = load i32, i32* %14, align 4
  %105 = udiv i32 %103, %104
  %106 = zext i32 %105 to i64
  %107 = load i64, i64* %10, align 8
  %108 = add nsw i64 %107, 3000
  %109 = call i64 @_Z7int_cosl(i64 %108)
  %110 = mul nsw i64 %109, 2
  %111 = sdiv i64 %110, 5
  %112 = add nsw i64 300, %111
  %113 = load i64, i64* %10, align 8
  %114 = add nsw i64 %113, 3600
  %115 = call i64 @_Z7int_cosl(i64 %114)
  %116 = mul nsw i64 %115, 3
  %117 = sdiv i64 %116, 10
  %118 = add nsw i64 200, %117
  %119 = load i64, i64* %10, align 8
  %120 = add nsw i64 %119, 4400
  %121 = call i64 @_Z7int_cosl(i64 %120)
  %122 = mul nsw i64 %121, 3
  %123 = sdiv i64 %122, 10
  %124 = add nsw i64 200, %123
  %125 = call i32 @_Z11palette_intllll(i64 %106, i64 %112, i64 %118, i64 %124)
  store i32 %125, i32* %23, align 4
  %126 = load i8, i8* %20, align 1
  %127 = trunc i8 %126 to i1
  br i1 %127, label %128, label %130

128:                                              ; preds = %101
  %129 = load i32, i32* %23, align 4
  store i32 %129, i32* %11, align 4
  br label %130

130:                                              ; preds = %128, %101
  %131 = load i32, i32* %11, align 4
  ret i32 %131
}

declare dso_local void @put_pixel(i32, i32, i8 zeroext, i8 zeroext, i8 zeroext, i8 zeroext) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i8 @_Z13unpack_4ui8_rj(i32 %0) #3 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = and i32 %3, -16777216
  %5 = lshr i32 %4, 24
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i8 @_Z13unpack_4ui8_gj(i32 %0) #3 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = and i32 %3, 16711680
  %5 = lshr i32 %4, 16
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i8 @_Z13unpack_4ui8_bj(i32 %0) #3 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = and i32 %3, 65280
  %5 = lshr i32 %4, 8
  %6 = trunc i32 %5 to i8
  ret i8 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i8 @_Z13unpack_4ui8_aj(i32 %0) #3 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = and i32 %3, 255
  %5 = trunc i32 %4 to i8
  ret i8 %5
}

declare dso_local void @flush() #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @_Z17pack_4ui8_to_ui32hhhh(i8 zeroext %0, i8 zeroext %1, i8 zeroext %2, i8 zeroext %3) #3 {
  %5 = alloca i8, align 1
  %6 = alloca i8, align 1
  %7 = alloca i8, align 1
  %8 = alloca i8, align 1
  %9 = alloca i32, align 4
  store i8 %0, i8* %5, align 1
  store i8 %1, i8* %6, align 1
  store i8 %2, i8* %7, align 1
  store i8 %3, i8* %8, align 1
  store i32 0, i32* %9, align 4
  %10 = load i32, i32* %9, align 4
  %11 = load i8, i8* %8, align 1
  %12 = zext i8 %11 to i32
  %13 = or i32 %10, %12
  store i32 %13, i32* %9, align 4
  %14 = load i32, i32* %9, align 4
  %15 = load i8, i8* %7, align 1
  %16 = zext i8 %15 to i32
  %17 = shl i32 %16, 8
  %18 = or i32 %14, %17
  store i32 %18, i32* %9, align 4
  %19 = load i32, i32* %9, align 4
  %20 = load i8, i8* %6, align 1
  %21 = zext i8 %20 to i32
  %22 = shl i32 %21, 16
  %23 = or i32 %19, %22
  store i32 %23, i32* %9, align 4
  %24 = load i32, i32* %9, align 4
  %25 = load i8, i8* %5, align 1
  %26 = zext i8 %25 to i32
  %27 = shl i32 %26, 24
  %28 = or i32 %24, %27
  store i32 %28, i32* %9, align 4
  %29 = load i32, i32* %9, align 4
  ret i32 %29
}

; Function Attrs: noinline optnone uwtable
define dso_local i64 @_Z7int_cosl(i64 %0) #2 {
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca i64, align 8
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %7 = load i64, i64* %2, align 8
  %8 = srem i64 %7, 1000
  store i64 %8, i64* %3, align 8
  %9 = load i64, i64* %2, align 8
  %10 = call i64 @_ZSt3absl(i64 %9)
  %11 = srem i64 %10, 4000
  %12 = sdiv i64 %11, 1000
  store i64 %12, i64* %4, align 8
  %13 = load i64, i64* %4, align 8
  %14 = icmp eq i64 %13, 1
  br i1 %14, label %18, label %15

15:                                               ; preds = %1
  %16 = load i64, i64* %4, align 8
  %17 = icmp eq i64 %16, 3
  br i1 %17, label %18, label %21

18:                                               ; preds = %15, %1
  %19 = load i64, i64* %3, align 8
  %20 = sub nsw i64 1000, %19
  store i64 %20, i64* %3, align 8
  br label %21

21:                                               ; preds = %18, %15
  %22 = load i64, i64* %4, align 8
  %23 = icmp eq i64 %22, 1
  br i1 %23, label %27, label %24

24:                                               ; preds = %21
  %25 = load i64, i64* %4, align 8
  %26 = icmp eq i64 %25, 2
  br label %27

27:                                               ; preds = %24, %21
  %28 = phi i1 [ true, %21 ], [ %26, %24 ]
  %29 = zext i1 %28 to i64
  %30 = select i1 %28, i32 1, i32 -1
  %31 = sext i32 %30 to i64
  store i64 %31, i64* %5, align 8
  %32 = load i64, i64* %3, align 8
  %33 = load i64, i64* %3, align 8
  %34 = mul nsw i64 %32, %33
  %35 = sdiv i64 %34, 1000
  %36 = sub nsw i64 %35, 1000
  store i64 %36, i64* %6, align 8
  %37 = load i64, i64* %6, align 8
  %38 = load i64, i64* %5, align 8
  %39 = mul nsw i64 %37, %38
  ret i64 %39
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i64 @_ZSt3absl(i64 %0) #3 comdat {
  %2 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %3 = load i64, i64* %2, align 8
  %4 = sub nsw i64 0, %3
  %5 = icmp slt i64 %3, 0
  %6 = select i1 %5, i64 %4, i64 %3
  ret i64 %6
}

; Function Attrs: noinline optnone uwtable
define dso_local i32 @_Z11palette_intllll(i64 %0, i64 %1, i64 %2, i64 %3) #2 {
  %5 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i8, align 1
  %10 = alloca i8, align 1
  %11 = alloca i8, align 1
  store i64 %0, i64* %5, align 8
  store i64 %1, i64* %6, align 8
  store i64 %2, i64* %7, align 8
  store i64 %3, i64* %8, align 8
  %12 = load i64, i64* %5, align 8
  %13 = mul nsw i64 1000, %12
  %14 = sdiv i64 %13, 1000
  %15 = load i64, i64* %6, align 8
  %16 = add nsw i64 %14, %15
  %17 = mul nsw i64 628318, %16
  %18 = sdiv i64 %17, 100000
  %19 = call i64 @_Z7int_cosl(i64 %18)
  %20 = mul nsw i64 500, %19
  %21 = sdiv i64 %20, 1000
  %22 = add nsw i64 500, %21
  %23 = sdiv i64 %22, 5
  %24 = trunc i64 %23 to i8
  store i8 %24, i8* %9, align 1
  %25 = load i64, i64* %5, align 8
  %26 = mul nsw i64 1000, %25
  %27 = sdiv i64 %26, 1000
  %28 = load i64, i64* %7, align 8
  %29 = add nsw i64 %27, %28
  %30 = mul nsw i64 628318, %29
  %31 = sdiv i64 %30, 100000
  %32 = call i64 @_Z7int_cosl(i64 %31)
  %33 = mul nsw i64 500, %32
  %34 = sdiv i64 %33, 1000
  %35 = add nsw i64 500, %34
  %36 = sdiv i64 %35, 5
  %37 = trunc i64 %36 to i8
  store i8 %37, i8* %10, align 1
  %38 = load i64, i64* %5, align 8
  %39 = mul nsw i64 0, %38
  %40 = sdiv i64 %39, 1000
  %41 = load i64, i64* %8, align 8
  %42 = add nsw i64 %40, %41
  %43 = mul nsw i64 628318, %42
  %44 = sdiv i64 %43, 100000
  %45 = call i64 @_Z7int_cosl(i64 %44)
  %46 = mul nsw i64 500, %45
  %47 = sdiv i64 %46, 1000
  %48 = add nsw i64 500, %47
  %49 = sdiv i64 %48, 5
  %50 = trunc i64 %49 to i8
  store i8 %50, i8* %11, align 1
  %51 = load i8, i8* %9, align 1
  %52 = load i8, i8* %10, align 1
  %53 = load i8, i8* %11, align 1
  %54 = load i64, i64* %5, align 8
  %55 = sdiv i64 %54, 4
  %56 = trunc i64 %55 to i8
  %57 = call i32 @_Z17pack_4ui8_to_ui32hhhh(i8 zeroext %51, i8 zeroext %52, i8 zeroext %53, i8 zeroext %56)
  ret i32 %57
}

attributes #0 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1 "}
