; ModuleID = 'test.ll'
source_filename = "test.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-darwin19.0.0"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@gSig = global i32 0
; Function Attrs: noinline nounwind optnone
define i32 @add(i32 %a, i32 %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %1 = load i32, i32* %b.addr, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, i32* %res, align 4
  %2 = load i32, i32* %res, align 4
  store i32 1, i32* @gSig
  ret i32 %2
}
; Function Attrs: noinline nounwind optnone
define i32 @sub(i32 %a, i32 %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %1 = load i32, i32* %b.addr, align 4
  %sub = sub nsw i32 %0, %1
  store i32 %sub, i32* %res, align 4
  %2 = load i32, i32* %res, align 4
  store i32 1, i32* @gSig
  ret i32 %2
}
; Function Attrs: noinline nounwind optnone
define i32 @mul(i32 %a, i32 %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  store i32 %b, i32* %b.addr, align 4
  %0 = load i32, i32* %a.addr, align 4
  %1 = load i32, i32* %b.addr, align 4
  %mul = mul nsw i32 %0, %1
  store i32 %mul, i32* %res, align 4
  %2 = load i32, i32* %res, align 4
  store i32 1, i32* @gSig
  ret i32 %2
}
; Function Attrs: noinline nounwind optnone
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32* %a)
  %0 = load i32, i32* %a, align 4
  %cmp = icmp sgt i32 %0, 10
  store i32 1, i32* @gSig
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @gSig
  %2 = xor i32 %1, 3
  %3 = icmp ne i32 %2, 2
  %call1 = call i32 @add(i32 2, i32 3)
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %call1)
  store i32 %2, i32* @gSig
  br label %if.end9

if.else:                                          ; preds = %entry
  %4 = load i32, i32* @gSig
  %5 = xor i32 %4, 2
  %6 = icmp ne i32 %5, 3
  %7 = load i32, i32* %a, align 4
  %cmp3 = icmp slt i32 %7, 9
  store i32 %5, i32* @gSig
  br i1 %cmp3, label %if.then4, label %if.else7

if.then4:                                         ; preds = %if.else
  %8 = load i32, i32* @gSig
  %9 = xor i32 %8, 7
  %10 = icmp ne i32 %9, 4
  %call5 = call i32 @sub(i32 2, i32 3)
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %call5)
  store i32 %9, i32* @gSig
  br label %if.end

if.else7:                                         ; preds = %if.else
  %11 = load i32, i32* @gSig
  %12 = xor i32 %11, 6
  %13 = icmp ne i32 %12, 5
  %call8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 10)
  store i32 %12, i32* @gSig
  br label %if.end

if.end:                                           ; preds = %if.else7, %if.then4
  %D1 = phi i32 [ 1, %if.then4 ], [ 0, %if.else7 ]
  %14 = load i32, i32* @gSig
  %15 = xor i32 %14, 3
  %16 = xor i32 %15, %D1
  %17 = icmp ne i32 %16, 6
  br label %if.end9

if.end9:                                          ; preds = %if.end, %if.then
  %D = phi i32 [ 4, %if.then ], [ 0, %if.end ]
  %18 = load i32, i32* @gSig
  %19 = xor i32 %18, 1
  %20 = xor i32 %19, %D
  %21 = icmp ne i32 %20, 7
  %call10 = call i32 @mul(i32 2, i32 3)
  %call11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i32 0, i32 0), i32 %call10)
  ret i32 0
}
declare i32 @scanf(i8*, ...) #1
declare i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-features"="+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-features"="+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Apple clang version 11.0.0 (clang-1100.0.33.12)"}
