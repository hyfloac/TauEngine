source_filename = "src\5CVector3fIntrin.ll"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.25.28614"

define <3 x float> @vector3f_add(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %val = fadd <3 x float> %a, %b
    ret <3 x float> %val
}

define <3 x float> @vector3f_sub(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %val = fsub <3 x float> %a, %b
    ret <3 x float> %val
}

define <3 x float> @vector3f_mul(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %val = fmul <3 x float> %a, %b
    ret <3 x float> %val
}

define <3 x float> @vector3f_div(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %val = fdiv <3 x float> %a, %b
    ret <3 x float> %val
}

define <3 x float> @vector3f_addScalar(<3 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <3 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <3 x float> %scalingVector0, float %b, i32 1
    %scalingVector  = insertelement <3 x float> %scalingVector1, float %b, i32 2

    %val = fadd <3 x float> %a, %scalingVector
    ret <3 x float> %val
}

define <3 x float> @vector3f_subScalar(<3 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <3 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <3 x float> %scalingVector0, float %b, i32 1
    %scalingVector  = insertelement <3 x float> %scalingVector1, float %b, i32 2

    %val = fsub <3 x float> %a, %scalingVector
    ret <3 x float> %val
}

define <3 x float> @vector3f_mulScalar(<3 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <3 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <3 x float> %scalingVector0, float %b, i32 1
    %scalingVector  = insertelement <3 x float> %scalingVector1, float %b, i32 2

    %val = fmul <3 x float> %a, %scalingVector
    ret <3 x float> %val
}

define <3 x float> @vector3f_divScalar(<3 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <3 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <3 x float> %scalingVector0, float %b, i32 1
    %scalingVector  = insertelement <3 x float> %scalingVector1, float %b, i32 2

    %val = fdiv <3 x float> %a, %scalingVector
    ret <3 x float> %val
}

define <3 x float> @vector3f_divScalarInv(float %a, <3 x float> %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <3 x float>           undef, float %a, i32 0
    %scalingVector1 = insertelement <3 x float> %scalingVector0, float %a, i32 1
    %scalingVector  = insertelement <3 x float> %scalingVector1, float %a, i32 2

    %val = fadd <3 x float> %scalingVector, %b
    ret <3 x float> %val
}

define <3 x float> @vector3f_neg(<3 x float> %a) local_unnamed_addr #0
{
    %val = fneg <3 x float> %a
    ret <3 x float> %val
}

define float @vector3f_magnitudeSquared(<3 x float> %a) local_unnamed_addr #0
{
    %squared = fmul <3 x float> %a, %a
    %x = extractelement <3 x float> %squared, i32 0
    %y = extractelement <3 x float> %squared, i32 1
    %z = extractelement <3 x float> %squared, i32 2

    %xy = fadd float %x, %y
    %magSquared = fadd float %xy, %z
    ret float %magSquared
}

define float @vector3f_magnitude(<3 x float> %a) local_unnamed_addr #0
{
    %magSquared = call float @vector3f_magnitudeSquared(<3 x float> %a)
    %mag = call float @llvm.sqrt.f32(float %magSquared)
    ret float %mag
}

define float @vector3f_inverseMagnitude(<3 x float> %a) local_unnamed_addr #0
{
    %magSquared = call float @vector3f_magnitudeSquared(<3 x float> %a)
    %inputVec = insertelement <4 x float> undef, float %magSquared, i32 0
    %rsqrtVec = call <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float> %inputVec)
    %val = extractelement <4 x float> %rsqrtVec, i32 0
    ret float %val
}

define <3 x float> @vector3f_normalizeFast(<3 x float> %a) local_unnamed_addr #0
{
    %invMag = call float @vector3f_inverseMagnitude(<3 x float> %a)
    %invMagVector0 = insertelement <3 x float>          undef, float %invMag, i32 0
    %invMagVector1 = insertelement <3 x float> %invMagVector0, float %invMag, i32 1
    %invMagVector  = insertelement <3 x float> %invMagVector1, float %invMag, i32 2

    %normalized = fmul <3 x float> %a, %invMagVector
    ret <3 x float> %normalized
}

define <3 x float> @vector3f_normalizeExact(<3 x float> %a) local_unnamed_addr #0
{
    %mag = call float @vector3f_magnitude(<3 x float> %a)
    %invMag = fdiv float 1.0, %mag
    %invMagVector0 = insertelement <3 x float>          undef, float %invMag, i32 0
    %invMagVector1 = insertelement <3 x float> %invMagVector0, float %invMag, i32 1
    %invMagVector  = insertelement <3 x float> %invMagVector1, float %invMag, i32 2

    %normalized = fmul <3 x float> %a, %invMagVector
    ret <3 x float> %normalized
}

define float @vector3f_dot(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %mul = fmul <3 x float> %a, %b
    %x = extractelement <3 x float> %mul, i32 0
    %y = extractelement <3 x float> %mul, i32 1
    %z = extractelement <3 x float> %mul, i32 2

    %xy = fadd float %x, %y
    %dot = fadd float %xy, %z
    ret float %dot
}

define <3 x float> @vector3f_cross(<3 x float> %a, <3 x float> %b) local_unnamed_addr #0
{
    %ax = extractelement <3 x float> %a, i32 0
    %ay = extractelement <3 x float> %a, i32 1
    %az = extractelement <3 x float> %a, i32 2

    %bx = extractelement <3 x float> %b, i32 0
    %by = extractelement <3 x float> %b, i32 1
    %bz = extractelement <3 x float> %b, i32 2

    %a0_0 = insertelement <3 x float> undef, float %ay, i32 0
    %a0_1 = insertelement <3 x float> %a0_0, float %az, i32 1
    %a0   = insertelement <3 x float> %a0_1, float %ax, i32 2

    %a1_0 = insertelement <3 x float> undef, float %az, i32 0
    %a1_1 = insertelement <3 x float> %a1_0, float %ax, i32 1
    %a1   = insertelement <3 x float> %a1_1, float %ay, i32 2

    %b0_0 = insertelement <3 x float> undef, float %bz, i32 0
    %b0_1 = insertelement <3 x float> %b0_0, float %bx, i32 1
    %b0   = insertelement <3 x float> %b0_1, float %by, i32 2

    %b1_0 = insertelement <3 x float> undef, float %by, i32 0
    %b1_1 = insertelement <3 x float> %b1_0, float %bz, i32 1
    %b1   = insertelement <3 x float> %b1_1, float %bx, i32 2

    %c0 = fmul <3 x float> %a0, %b0
    %c1 = fmul <3 x float> %a1, %b1

    %cross = fsub <3 x float> %c0, %c1
    ret <3 x float> %cross
}

declare <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float>)
declare float @llvm.sqrt.f32(float)

attributes #0 = { inlinehint nofree norecurse willreturn nosync nounwind optsize readnone }
