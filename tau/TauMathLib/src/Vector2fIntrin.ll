source_filename = "src\5CVector2fIntrin.ll"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.25.28614"

define void @vector2f_add(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture readonly %pB, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %b = load <2 x float>, <2 x float>* %pB

    %val = fadd <2 x float> %a, %b
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_sub(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture readonly %pB, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %b = load <2 x float>, <2 x float>* %pB

    %val = fsub <2 x float> %a, %b
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_mul(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture readonly %pB, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %b = load <2 x float>, <2 x float>* %pB

    %val = fmul <2 x float> %a, %b
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_div(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture readonly %pB, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %b = load <2 x float>, <2 x float>* %pB

    %val = fdiv <2 x float> %a, %b
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_addScalar(<2 x float>* nocapture readonly %pA, float %b, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA

    %scalingVector0 = insertelement <2 x float>           undef, float %b, i32 0
    %scalingVector  = insertelement <2 x float> %scalingVector0, float %b, i32 1

    %val = fadd <2 x float> %a, %scalingVector
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_subScalar(<2 x float>* nocapture readonly %pA, float %b, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    
    %scalingVector0 = insertelement <2 x float>           undef, float %b, i32 0
    %scalingVector  = insertelement <2 x float> %scalingVector0, float %b, i32 1

    %val = fsub <2 x float> %a, %scalingVector
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_mulScalar(<2 x float>* nocapture readonly %pA, float %b, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    
    %scalingVector0 = insertelement <2 x float>           undef, float %b, i32 0
    %scalingVector  = insertelement <2 x float> %scalingVector0, float %b, i32 1

    %val = fmul <2 x float> %a, %scalingVector
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_divScalar(<2 x float>* nocapture readonly %pA, float %b, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    
    %scalingVector0 = insertelement <2 x float>           undef, float %b, i32 0
    %scalingVector  = insertelement <2 x float> %scalingVector0, float %b, i32 1

    %val = fdiv <2 x float> %a, %scalingVector
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_divScalarInv(float %a, <2 x float>* nocapture readonly %pB, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %b = load <2 x float>, <2 x float>* %pB
    
    %scalingVector0 = insertelement <2 x float>           undef, float %a, i32 0
    %scalingVector  = insertelement <2 x float> %scalingVector0, float %a, i32 1

    %val = fadd <2 x float> %scalingVector, %b
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define void @vector2f_neg(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    
    %val = fneg <2 x float> %a
    store <2 x float> %val, <2 x float>* %pStore
    ret void
}

define float @vector2f_internal_magnitudeSquared(<2 x float> %a) local_unnamed_addr #0
{
    %squared = fmul <2 x float> %a, %a
    %x = extractelement <2 x float> %squared, i32 0
    %y = extractelement <2 x float> %squared, i32 1

    %magSquared = fadd float %x, %y
    ret float %magSquared
}

define float @vector2f_magnitudeSquared(<2 x float>* nocapture readonly %pA) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %magSquared = call float @vector2f_internal_magnitudeSquared(<2 x float> %a)
    ret float %magSquared
}

define float @vector2f_internal_magnitude(<2 x float> %a) local_unnamed_addr #0
{
    %magSquared = call float @vector2f_internal_magnitudeSquared(<2 x float> %a)
    %mag = call float @llvm.sqrt.f32(float %magSquared)
    ret float %mag
}

define float @vector2f_magnitude(<2 x float>* nocapture readonly %pA) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %mag = call float @vector2f_internal_magnitude(<2 x float> %a)
    ret float %mag
}

define float @vector2f_internal_inverseMagnitude(<2 x float> %a) local_unnamed_addr #0
{
    %magSquared = call float @vector2f_internal_magnitudeSquared(<2 x float> %a)
    %inputVec = insertelement <4 x float> undef, float %magSquared, i32 0
    %rsqrtVec = call <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float> %inputVec)
    %val = extractelement <4 x float> %rsqrtVec, i32 0
    ret float %val
}

define float @vector2f_inverseMagnitude(<2 x float>* nocapture readonly %pA) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %val = call float @vector2f_internal_inverseMagnitude(<2 x float> %a)
    ret float %val
}

define void @vector2f_normalizeFast(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA

    %invMag = call float @vector2f_internal_inverseMagnitude(<2 x float> %a)
    %invMagVector0 = insertelement <2 x float>          undef, float %invMag, i32 0
    %invMagVector  = insertelement <2 x float> %invMagVector0, float %invMag, i32 1

    %normalized = fmul <2 x float> %a, %invMagVector
    store <2 x float> %normalized, <2 x float>* %pStore
    ret void
}

define void @vector2f_normalizeExact(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <2 x float>, <2 x float>* %pA

    %mag = call float @vector2f_internal_magnitude(<2 x float> %a)
    %invMag = fdiv float 1.0, %mag
    %invMagVector0 = insertelement <2 x float>          undef, float %invMag, i32 0
    %invMagVector  = insertelement <2 x float> %invMagVector0, float %invMag, i32 1

    %normalized = fmul <2 x float> %a, %invMagVector
    store <2 x float> %normalized, <2 x float>* %pStore
    ret void
}

define float @vector2f_dot(<2 x float>* nocapture readonly %pA, <2 x float>* nocapture readonly %pB) #0
{
    %a = load <2 x float>, <2 x float>* %pA
    %b = load <2 x float>, <2 x float>* %pB

    %mul = fmul <2 x float> %a, %b
    %x = extractelement <2 x float> %mul, i32 0
    %y = extractelement <2 x float> %mul, i32 1

    %dot = fadd float %x, %y
    ret float %dot
}

declare <4 x float> @llvm.x86.sse.rsqrt.ss(<4 x float>)
declare float @llvm.sqrt.f32(float)

attributes #0 = { inlinehint nofree norecurse willreturn nosync nounwind optsize }
