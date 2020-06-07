source_filename = "src\5CVector4fIntrin.ll"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.25.28614"

define <4 x float> @vector4f_add(<4 x float> %a, <4 x float> %b) local_unnamed_addr #0
{
    %val = fadd <4 x float> %a, %b
    ret <4 x float> %val
}

define <4 x float> @vector4f_sub(<4 x float> %a, <4 x float> %b) local_unnamed_addr #0
{
    %val = fsub <4 x float> %a, %b
    ret <4 x float> %val
}

define <4 x float> @vector4f_mul(<4 x float> %a, <4 x float> %b) local_unnamed_addr #0
{
    %val = fmul <4 x float> %a, %b
    ret <4 x float> %val
}

define <4 x float> @vector4f_div(<4 x float> %a, <4 x float> %b) local_unnamed_addr #0
{
    %val = fdiv <4 x float> %a, %b
    ret <4 x float> %val
}

define <4 x float> @vector4f_addScalar(<4 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %b, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %b, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %b, i32 3

    %val = fadd <4 x float> %a, %scalingVector
    ret <4 x float> %val
}

define <4 x float> @vector4f_subScalar(<4 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %b, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %b, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %b, i32 3

    %val = fsub <4 x float> %a, %scalingVector
    ret <4 x float> %val
}

define <4 x float> @vector4f_mulScalar(<4 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %b, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %b, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %b, i32 3

    %val = fmul <4 x float> %a, %scalingVector
    ret <4 x float> %val
}

define <4 x float> @vector4f_divScalar(<4 x float> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %b, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %b, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %b, i32 3

    %val = fdiv <4 x float> %a, %scalingVector
    ret <4 x float> %val
}

define <4 x float> @vector4f_divScalarInv(float %a, <4 x float> %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %a, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %a, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %a, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %a, i32 3

    %val = fadd <4 x float> %scalingVector, %b
    ret <4 x float> %val
}

define <4 x float> @vector4f_neg(<4 x float> %a) local_unnamed_addr #0
{
    %val = fneg <4 x float> %a
    ret <4 x float> %val
}

define float @vector4f_magnitudeSquared(<4 x float> %a) local_unnamed_addr #0
{
    %squared = fmul <4 x float> %a, %a
    %x = extractelement <4 x float> %squared, i32 0
    %y = extractelement <4 x float> %squared, i32 1
    %z = extractelement <4 x float> %squared, i32 2
    %w = extractelement <4 x float> %squared, i32 3

    %xy = fadd float %x, %y
    %zw = fadd float %z, %w
    %magSquared = fadd float %xy, %zw
    ret float %magSquared
}

define float @vector4f_magnitude(<4 x float> %a) local_unnamed_addr #0
{
    %magSquared = call float @vector4f_magnitudeSquared(<4 x float> %a)
    %mag = call float @llvm.sqrt.f32(float %magSquared)
    ret float %mag
}

define float @vector4f_inverseMagnitude(<4 x float> %a) local_unnamed_addr #0
{
    %magSquared = call float @vector4f_magnitudeSquared(<4 x float> %a)
    %inputVec = insertelement <4 x float> undef, float %magSquared, i32 0
    %rsqrtVec = call <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float> %inputVec)
    %val = extractelement <4 x float> %rsqrtVec, i32 0
    ret float %val
}

define <4 x float> @vector4f_normalizeFast(<4 x float> %a) local_unnamed_addr #0
{
    %invMag = call float @vector4f_inverseMagnitude(<4 x float> %a)
    %invMagVector0 = insertelement <4 x float>          undef, float %invMag, i32 0
    %invMagVector1 = insertelement <4 x float> %invMagVector0, float %invMag, i32 1
    %invMagVector2 = insertelement <4 x float> %invMagVector1, float %invMag, i32 2
    %invMagVector  = insertelement <4 x float> %invMagVector2, float %invMag, i32 3

    %normalized = fmul <4 x float> %a, %invMagVector
    ret <4 x float> %normalized
}

define <4 x float> @vector4f_normalizeExact(<4 x float> %a) local_unnamed_addr #0
{
    %mag = call float @vector4f_magnitude(<4 x float> %a)
    %invMag = fdiv float 1.0, %mag
    %invMagVector0 = insertelement <4 x float>          undef, float %invMag, i32 0
    %invMagVector1 = insertelement <4 x float> %invMagVector0, float %invMag, i32 1
    %invMagVector2 = insertelement <4 x float> %invMagVector1, float %invMag, i32 2
    %invMagVector  = insertelement <4 x float> %invMagVector2, float %invMag, i32 3

    %normalized = fmul <4 x float> %a, %invMagVector
    ret <4 x float> %normalized
}

define float @vector4f_dot(<4 x float> %a, <4 x float> %b)
{
    %mul = fmul <4 x float> %a, %b
    %x = extractelement <4 x float> %mul, i32 0
    %y = extractelement <4 x float> %mul, i32 1
    %z = extractelement <4 x float> %mul, i32 2
    %w = extractelement <4 x float> %mul, i32 3

    %xy = fadd float %x, %y
    %zw = fadd float %z, %w
    %dot = fadd float %xy, %zw
    ret float %dot
}

declare <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float>)
declare float @llvm.sqrt.f32(float)

attributes #0 = { inlinehint nofree norecurse willreturn nosync nounwind optsize readnone }
