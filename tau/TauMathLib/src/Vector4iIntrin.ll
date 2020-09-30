source_filename = "src\5CVector4iIntrin.ll"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.25.28614"

define <4 x i32> @vector4i_add(<4 x i32> %a, <4 x i32> %b) local_unnamed_addr #0
{
    %val = add <4 x i32> %a, %b
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_sub(<4 x i32> %a, <4 x i32> %b) local_unnamed_addr #0
{
    %val = sub <4 x i32> %a, %b
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_mul(<4 x i32> %a, <4 x i32> %b) local_unnamed_addr #0
{
    %val = mul <4 x i32> %a, %b
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_div(<4 x i32> %a, <4 x i32> %b) local_unnamed_addr #0
{
    %val = sdiv <4 x i32> %a, %b
    ret <4 x i32> %val
}

define <4 x float> @vector4i_divF(<4 x i32> %a, <4 x i32> %b) local_unnamed_addr #0
{
    %fa = sitofp <4 x i32> %a to <4 x float>
    %fb = sitofp <4 x i32> %b to <4 x float>
    %val = fdiv <4 x float> %fa, %fb
    ret <4 x float> %val
}

define <4 x i32> @vector4i_addScalar(<4 x i32> %a, i32 %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x i32>           undef, i32 %b, i32 0
    %scalingVector1 = insertelement <4 x i32> %scalingVector0, i32 %b, i32 1
    %scalingVector2 = insertelement <4 x i32> %scalingVector1, i32 %b, i32 2
    %scalingVector  = insertelement <4 x i32> %scalingVector2, i32 %b, i32 3

    %val = add <4 x i32> %a, %scalingVector
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_subScalar(<4 x i32> %a, i32 %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x i32>           undef, i32 %b, i32 0
    %scalingVector1 = insertelement <4 x i32> %scalingVector0, i32 %b, i32 1
    %scalingVector2 = insertelement <4 x i32> %scalingVector1, i32 %b, i32 2
    %scalingVector  = insertelement <4 x i32> %scalingVector2, i32 %b, i32 3

    %val = sub <4 x i32> %a, %scalingVector
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_mulScalar(<4 x i32> %a, i32 %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x i32>           undef, i32 %b, i32 0
    %scalingVector1 = insertelement <4 x i32> %scalingVector0, i32 %b, i32 1
    %scalingVector2 = insertelement <4 x i32> %scalingVector1, i32 %b, i32 2
    %scalingVector  = insertelement <4 x i32> %scalingVector2, i32 %b, i32 3

    %val = mul <4 x i32> %a, %scalingVector
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_divScalar(<4 x i32> %a, i32 %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x i32>           undef, i32 %b, i32 0
    %scalingVector1 = insertelement <4 x i32> %scalingVector0, i32 %b, i32 1
    %scalingVector2 = insertelement <4 x i32> %scalingVector1, i32 %b, i32 2
    %scalingVector  = insertelement <4 x i32> %scalingVector2, i32 %b, i32 3

    %val = sdiv <4 x i32> %a, %scalingVector
    ret <4 x i32> %val
}

define <4 x i32> @vector4i_divScalarInv(i32 %a, <4 x i32> %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x i32>           undef, i32 %a, i32 0
    %scalingVector1 = insertelement <4 x i32> %scalingVector0, i32 %a, i32 1
    %scalingVector2 = insertelement <4 x i32> %scalingVector1, i32 %a, i32 2
    %scalingVector  = insertelement <4 x i32> %scalingVector2, i32 %a, i32 3

    %val = sdiv <4 x i32> %scalingVector, %b
    ret <4 x i32> %val
}

define <4 x float> @vector4i_divScalarF(<4 x i32> %a, float %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %b, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %b, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %b, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %b, i32 3

    %fa = sitofp <4 x i32> %a to <4 x float>

    %val = fdiv <4 x float> %fa, %scalingVector
    ret <4 x float> %val
}

define <4 x float> @vector4i_divScalarInvF(float %a, <4 x i32> %b) local_unnamed_addr #0
{
    %scalingVector0 = insertelement <4 x float>           undef, float %a, i32 0
    %scalingVector1 = insertelement <4 x float> %scalingVector0, float %a, i32 1
    %scalingVector2 = insertelement <4 x float> %scalingVector1, float %a, i32 2
    %scalingVector  = insertelement <4 x float> %scalingVector2, float %a, i32 3

    %fb = sitofp <4 x i32> %b to <4 x float>

    %val = fdiv <4 x float> %scalingVector, %fb
    ret <4 x float> %val
}

define <4 x i32> @vector4i_neg(<4 x i32> %a) local_unnamed_addr #0
{
    %val = sub <4 x i32> <i32 0, i32 0, i32 0, i32 0>, %a
    ret <4 x i32> %val
}

define i32 @vector4i_magnitudeSquared(<4 x i32> %a) local_unnamed_addr #0
{
    %squared = mul <4 x i32> %a, %a
    %x = extractelement <4 x i32> %squared, i32 0
    %y = extractelement <4 x i32> %squared, i32 1
    %z = extractelement <4 x i32> %squared, i32 2
    %w = extractelement <4 x i32> %squared, i32 3

    %xy = add i32 %x, %y
    %zw = add i32 %z, %w
    %magSquared = add i32 %xy, %zw
    ret i32 %magSquared
}

define float @vector4i_magnitude(<4 x i32> %a) local_unnamed_addr #0
{
    %magSquared = call i32 @vector4i_magnitudeSquared(<4 x i32> %a)
    %magSquaredF = sitofp i32 %magSquared to float
    %mag = call float @llvm.sqrt.f32(float %magSquaredF)
    ret float %mag
}

define float @vector4i_inverseMagnitude(<4 x i32> %a) local_unnamed_addr #0
{
    %magSquared = call i32 @vector4i_magnitudeSquared(<4 x i32> %a)
    %magSquaredF = sitofp i32 %magSquared to float
    %inputVec = insertelement <4 x float> undef, float %magSquaredF, i32 0
    %rsqrtVec = call <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float> %inputVec)
    %val = extractelement <4 x float> %rsqrtVec, i32 0
    ret float %val
}

define <4 x float> @vector4i_normalizeFast(<4 x i32> %a) local_unnamed_addr #0
{
    %invMag = call float @vector4i_inverseMagnitude(<4 x i32> %a)
    %invMagVector0 = insertelement <4 x float>          undef, float %invMag, i32 0
    %invMagVector1 = insertelement <4 x float> %invMagVector0, float %invMag, i32 1
    %invMagVector2 = insertelement <4 x float> %invMagVector1, float %invMag, i32 2
    %invMagVector  = insertelement <4 x float> %invMagVector2, float %invMag, i32 3

    %fa = sitofp <4 x i32> %a to <4 x float>

    %normalized = fmul <4 x float> %fa, %invMagVector
    ret <4 x float> %normalized
}

define <4 x float> @vector4i_normalizeExact(<4 x i32> %a) local_unnamed_addr #0
{
    %mag = call float @vector4i_magnitude(<4 x i32> %a)
    %magVector0 = insertelement <4 x float>          undef, float %mag, i32 0
    %magVector1 = insertelement <4 x float> %magVector0, float %mag, i32 1
    %magVector2 = insertelement <4 x float> %magVector1, float %mag, i32 2
    %magVector  = insertelement <4 x float> %magVector2, float %mag, i32 3

    %fa = sitofp <4 x i32> %a to <4 x float>

    %normalized = fdiv <4 x float> %fa, %magVector
    ret <4 x float> %normalized
}

define i32 @vector4i_dot(<4 x i32> %a, <4 x i32> %b) #0
{
    %mul = mul <4 x i32> %a, %b
    %x = extractelement <4 x i32> %mul, i32 0
    %y = extractelement <4 x i32> %mul, i32 1
    %z = extractelement <4 x i32> %mul, i32 2
    %w = extractelement <4 x i32> %mul, i32 3

    %xy = add i32 %x, %y
    %zw = add i32 %z, %w
    %dot = add i32 %xy, %zw
    ret i32 %dot
}

declare <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float>)
declare float @llvm.sqrt.f32(float)

attributes #0 = { inlinehint nofree norecurse willreturn nosync nounwind optsize readnone }
