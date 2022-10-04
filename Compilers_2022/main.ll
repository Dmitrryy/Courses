
define dso_local i32 @main() {
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
  call void @sEngine_init(i32 1024, i32 576)
  br label %8

8:                                                ; preds = %37, %0
  %9 = call i32 (...) @sEngine_windowIsOpen()
  %10 = icmp ne i32 %9, 0
  br i1 %10, label %11, label %38

11:                                               ; preds = %8
  %12 = call i64 (...) @get_time_milliseconds()
  store i64 %12, i64* %4, align 8
  store i64 0, i64* %5, align 8
  br label %13

13:                                               ; preds = %34, %11
  %14 = load i64, i64* %5, align 8
  %15 = icmp slt i64 %14, 576
  br i1 %15, label %16, label %37

16:                                               ; preds = %13
  store i64 0, i64* %6, align 8
  br label %17

17:                                               ; preds = %30, %16
  %18 = load i64, i64* %6, align 8
  %19 = icmp slt i64 %18, 1024
  br i1 %19, label %20, label %33

20:                                               ; preds = %17
  %21 = load i64, i64* %6, align 8
  %22 = load i64, i64* %5, align 8
  %23 = load i64, i64* %4, align 8
  %24 = call i32 @shader_1(i64 %21, i64 %22, i64 1024, i64 576, i64 %23)
  store i32 %24, i32* %7, align 4
  %25 = load i64, i64* %6, align 8
  %26 = trunc i64 %25 to i32
  %27 = load i64, i64* %5, align 8
  %28 = trunc i64 %27 to i32
  %29 = load i32, i32* %7, align 4
  call void @put_pixel(i32 %26, i32 %28, i32 %29)
  br label %30

30:                                               ; preds = %20
  %31 = load i64, i64* %6, align 8
  %32 = add nsw i64 %31, 1
  store i64 %32, i64* %6, align 8
  br label %17

33:                                               ; preds = %17
  br label %34

34:                                               ; preds = %33
  %35 = load i64, i64* %5, align 8
  %36 = add nsw i64 %35, 1
  store i64 %36, i64* %5, align 8
  br label %13

37:                                               ; preds = %13
  call void (...) @flush()
  br label %8

38:                                               ; preds = %8
  ret i32 0
}

declare dso_local void @sEngine_init(i32, i32)

declare dso_local i32 @sEngine_windowIsOpen(...) 

declare dso_local i64 @get_time_milliseconds(...)

define dso_local i32 @shader_1(i64 %0, i64 %1, i64 %2, i64 %3, i64 %4)  {
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
  %20 = alloca i32, align 4
  %21 = alloca i64, align 8
  store i64 %0, i64* %6, align 8
  store i64 %1, i64* %7, align 8
  store i64 %2, i64* %8, align 8
  store i64 %3, i64* %9, align 8
  store i64 %4, i64* %10, align 8
  store i32 255, i32* %11, align 4
  %22 = load i64, i64* %6, align 8
  %23 = mul nsw i64 %22, 1000
  %24 = load i64, i64* %8, align 8
  %25 = sdiv i64 %23, %24
  store i64 %25, i64* %12, align 8
  %26 = load i64, i64* %7, align 8
  %27 = mul nsw i64 %26, 1000
  %28 = load i64, i64* %9, align 8
  %29 = sdiv i64 %27, %28
  store i64 %29, i64* %13, align 8
  %30 = load i64, i64* %10, align 8
  %31 = sdiv i64 %30, 2
  store i64 %31, i64* %10, align 8
  store i32 75, i32* %14, align 4
  %32 = load i64, i64* %10, align 8
  %33 = sdiv i64 %32, 3
  %34 = sub nsw i64 %33, 2000
  %35 = call i64 @int_cos(i64 %34)
  %36 = sdiv i64 %35, 5
  %37 = add nsw i64 %36, 850
  %38 = mul nsw i64 788, %37
  %39 = sdiv i64 %38, 1000
  store i64 %39, i64* %15, align 8
  %40 = load i64, i64* %15, align 8
  %41 = load i64, i64* %10, align 8
  %42 = sdiv i64 %41, 3
  %43 = call i64 @int_cos(i64 %42)
  %44 = mul nsw i64 %40, %43
  %45 = sdiv i64 %44, 1000
  store i64 %45, i64* %16, align 8
  %46 = load i64, i64* %15, align 8
  %47 = load i64, i64* %10, align 8
  %48 = sdiv i64 %47, 3
  %49 = add nsw i64 %48, 3000
  %50 = call i64 @int_cos(i64 %49)
  %51 = mul nsw i64 %46, %50
  %52 = sdiv i64 %51, 1000
  store i64 %52, i64* %17, align 8
  %53 = load i64, i64* %12, align 8
  %54 = sub nsw i64 %53, 500
  %55 = mul nsw i64 3, %54
  store i64 %55, i64* %18, align 8
  %56 = load i64, i64* %13, align 8
  %57 = sub nsw i64 %56, 500
  %58 = mul nsw i64 2, %57
  store i64 %58, i64* %19, align 8
  store i32 0, i32* %20, align 4
  store i32 0, i32* %20, align 4
  br label %59

59:                                               ; preds = %96, %5
  %60 = load i32, i32* %20, align 4
  %61 = load i32, i32* %14, align 4
  %62 = icmp ult i32 %60, %61
  br i1 %62, label %63, label %99

63:                                               ; preds = %59
  %64 = load i64, i64* %18, align 8
  %65 = load i64, i64* %18, align 8
  %66 = mul nsw i64 %64, %65
  %67 = load i64, i64* %19, align 8
  %68 = load i64, i64* %19, align 8
  %69 = mul nsw i64 %67, %68
  %70 = sub nsw i64 %66, %69
  %71 = sdiv i64 %70, 1000
  %72 = load i64, i64* %16, align 8
  %73 = add nsw i64 %71, %72
  store i64 %73, i64* %21, align 8
  %74 = load i64, i64* %19, align 8
  %75 = load i64, i64* %18, align 8
  %76 = mul nsw i64 %74, %75
  %77 = load i64, i64* %18, align 8
  %78 = load i64, i64* %19, align 8
  %79 = mul nsw i64 %77, %78
  %80 = add nsw i64 %76, %79
  %81 = sdiv i64 %80, 1000
  %82 = load i64, i64* %17, align 8
  %83 = add nsw i64 %81, %82
  store i64 %83, i64* %19, align 8
  %84 = load i64, i64* %21, align 8
  store i64 %84, i64* %18, align 8
  %85 = load i64, i64* %18, align 8
  %86 = load i64, i64* %18, align 8
  %87 = mul nsw i64 %85, %86
  %88 = load i64, i64* %19, align 8
  %89 = load i64, i64* %19, align 8
  %90 = mul nsw i64 %88, %89
  %91 = add nsw i64 %87, %90
  %92 = sdiv i64 %91, 10000
  %93 = icmp sgt i64 %92, 4000
  br i1 %93, label %94, label %95

94:                                               ; preds = %63
  br label %99

95:                                               ; preds = %63
  br label %96

96:                                               ; preds = %95
  %97 = load i32, i32* %20, align 4
  %98 = add i32 %97, 1
  store i32 %98, i32* %20, align 4
  br label %59

99:                                               ; preds = %94, %59
  %100 = load i32, i32* %20, align 4
  %101 = load i32, i32* %14, align 4
  %102 = icmp ne i32 %100, %101
  br i1 %102, label %103, label %128

103:                                              ; preds = %99
  %104 = load i32, i32* %20, align 4
  %105 = mul i32 %104, 1000
  %106 = load i32, i32* %14, align 4
  %107 = udiv i32 %105, %106
  %108 = zext i32 %107 to i64
  %109 = load i64, i64* %10, align 8
  %110 = add nsw i64 %109, 3000
  %111 = call i64 @int_cos(i64 %110)
  %112 = mul nsw i64 %111, 2
  %113 = sdiv i64 %112, 5
  %114 = add nsw i64 300, %113
  %115 = load i64, i64* %10, align 8
  %116 = add nsw i64 %115, 3600
  %117 = call i64 @int_cos(i64 %116)
  %118 = mul nsw i64 %117, 3
  %119 = sdiv i64 %118, 10
  %120 = add nsw i64 200, %119
  %121 = load i64, i64* %10, align 8
  %122 = add nsw i64 %121, 4400
  %123 = call i64 @int_cos(i64 %122)
  %124 = mul nsw i64 %123, 3
  %125 = sdiv i64 %124, 10
  %126 = add nsw i64 200, %125
  %127 = call i32 @palette_int(i64 %108, i64 %114, i64 %120, i64 %126)
  store i32 %127, i32* %11, align 4
  br label %128

128:                                              ; preds = %103, %99
  %129 = load i32, i32* %11, align 4
  ret i32 %129
}

declare dso_local void @put_pixel(i32, i32, i32) #1

declare dso_local void @flush(...) #1

define dso_local i32 @pack_4ui8_to_ui32(i8 zeroext %0, i8 zeroext %1, i8 zeroext %2, i8 zeroext %3)  {
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

define dso_local i64 @abs_dummy(i64 %0)  {
  %2 = alloca i64, align 8
  store i64 %0, i64* %2, align 8
  %3 = load i64, i64* %2, align 8
  %4 = icmp slt i64 %3, 0
  br i1 %4, label %5, label %8

5:                                                ; preds = %1
  %6 = load i64, i64* %2, align 8
  %7 = sub nsw i64 0, %6
  store i64 %7, i64* %2, align 8
  br label %8

8:                                                ; preds = %5, %1
  %9 = load i64, i64* %2, align 8
  ret i64 %9
}

define dso_local i64 @int_cos(i64 %0)  {
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
  %10 = call i64 @abs_dummy(i64 %9)
  %11 = srem i64 %10, 4000
  %12 = sdiv i64 %11, 1000
  store i64 %12, i64* %4, align 8
  %13 = load i64, i64* %4, align 8
  %14 = srem i64 %13, 2
  %15 = icmp ne i64 %14, 0
  br i1 %15, label %16, label %19

16:                                               ; preds = %1
  %17 = load i64, i64* %3, align 8
  %18 = sub nsw i64 1000, %17
  store i64 %18, i64* %3, align 8
  br label %19

19:                                               ; preds = %16, %1
  %20 = load i64, i64* %4, align 8
  %21 = icmp eq i64 %20, 1
  br i1 %21, label %25, label %22

22:                                               ; preds = %19
  %23 = load i64, i64* %4, align 8
  %24 = icmp eq i64 %23, 2
  br label %25

25:                                               ; preds = %22, %19
  %26 = phi i1 [ true, %19 ], [ %24, %22 ]
  %27 = zext i1 %26 to i64
  %28 = select i1 %26, i32 1, i32 -1
  %29 = sext i32 %28 to i64
  store i64 %29, i64* %5, align 8
  %30 = load i64, i64* %3, align 8
  %31 = load i64, i64* %3, align 8
  %32 = mul nsw i64 %30, %31
  %33 = sdiv i64 %32, 1000
  %34 = sub nsw i64 %33, 1000
  store i64 %34, i64* %6, align 8
  %35 = load i64, i64* %6, align 8
  %36 = load i64, i64* %5, align 8
  %37 = mul nsw i64 %35, %36
  ret i64 %37
}

define dso_local i32 @palette_int(i64 %0, i64 %1, i64 %2, i64 %3)  {
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
  %19 = call i64 @int_cos(i64 %18)
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
  %32 = call i64 @int_cos(i64 %31)
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
  %45 = call i64 @int_cos(i64 %44)
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
  %57 = call i32 @pack_4ui8_to_ui32(i8 zeroext %51, i8 zeroext %52, i8 zeroext %53, i8 zeroext %56)
  ret i32 %57
}
