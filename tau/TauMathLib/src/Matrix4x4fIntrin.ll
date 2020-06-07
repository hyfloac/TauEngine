source_filename = "src\5CMatrix4x4fIntrin.ll"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.25.28614"

define void @matrix4x4f_add(<16 x float>* nocapture readonly %pA, <16 x float>* nocapture readonly %pB, <16 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <16 x float>, <16 x float>* %pA
    %b = load <16 x float>, <16 x float>* %pB
    %val = fadd <16 x float> %a, %b
    store <16 x float> %val, <16 x float>* %pStore
    ret void
}

define void @matrix4x4f_sub(<16 x float>* nocapture readonly %pA, <16 x float>* nocapture readonly %pB, <16 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    %a = load <16 x float>, <16 x float>* %pA
    %b = load <16 x float>, <16 x float>* %pB
    %val = fsub <16 x float> %a, %b
    store <16 x float> %val, <16 x float>* %pStore
    ret void
}

define float @matrix4x4f_util_sumVector(<4 x float> %a) local_unnamed_addr #1
{
    %x = extractelement <4 x float> %a, i32 0
    %y = extractelement <4 x float> %a, i32 1
    %z = extractelement <4 x float> %a, i32 2
    %w = extractelement <4 x float> %a, i32 3

    %xy = fadd float %x, %y
    %zw = fadd float %z, %w
    %sum = fadd float %xy, %zw

    ret float %sum
}

define <4 x float> @matrix4x4f_util_buildVector(float %x, float %y, float %z, float %w) local_unnamed_addr #1
{
    %v0 = insertelement <4 x float> undef, float %x, i32 0
    %v1 = insertelement <4 x float>   %v0, float %y, i32 1
    %v2 = insertelement <4 x float>   %v1, float %z, i32 2
    %v  = insertelement <4 x float>   %v2, float %w, i32 3

    ret <4 x float> %v
}

define void @matrix4x4f_mul(<16 x float>* nocapture readonly %pA, <16 x float>* nocapture readonly %pB, <16 x float>* nocapture writeonly %pStore) local_unnamed_addr #0
{
    ; Dereference pointer
    %a = load <16 x float>, <16 x float>* %pA

    ; Get all elements from the matrix
    %a00 = extractelement <16 x float> %a, i32 0
    %a01 = extractelement <16 x float> %a, i32 1
    %a02 = extractelement <16 x float> %a, i32 2
    %a03 = extractelement <16 x float> %a, i32 3
    %a10 = extractelement <16 x float> %a, i32 4
    %a11 = extractelement <16 x float> %a, i32 5
    %a12 = extractelement <16 x float> %a, i32 6
    %a13 = extractelement <16 x float> %a, i32 7
    %a20 = extractelement <16 x float> %a, i32 8
    %a21 = extractelement <16 x float> %a, i32 9
    %a22 = extractelement <16 x float> %a, i32 10
    %a23 = extractelement <16 x float> %a, i32 11
    %a30 = extractelement <16 x float> %a, i32 12
    %a31 = extractelement <16 x float> %a, i32 13
    %a32 = extractelement <16 x float> %a, i32 14
    %a33 = extractelement <16 x float> %a, i32 15

    ; Reconstruct vectors
    %a0 = call <4 x float> @matrix4x4f_util_buildVector(float %a00, float %a01, float %a02, float %a03)
    %a1 = call <4 x float> @matrix4x4f_util_buildVector(float %a10, float %a11, float %a12, float %a13)
    %a2 = call <4 x float> @matrix4x4f_util_buildVector(float %a20, float %a21, float %a22, float %a23)
    %a3 = call <4 x float> @matrix4x4f_util_buildVector(float %a30, float %a31, float %a32, float %a33)

    ; Dereference pointer
    %b = load <16 x float>, <16 x float>* %pB

    ; Get all elements from the matrix
    %b00 = extractelement <16 x float> %b, i32 0
    %b01 = extractelement <16 x float> %b, i32 1
    %b02 = extractelement <16 x float> %b, i32 2
    %b03 = extractelement <16 x float> %b, i32 3
    %b10 = extractelement <16 x float> %b, i32 4
    %b11 = extractelement <16 x float> %b, i32 5
    %b12 = extractelement <16 x float> %b, i32 6
    %b13 = extractelement <16 x float> %b, i32 7
    %b20 = extractelement <16 x float> %b, i32 8
    %b21 = extractelement <16 x float> %b, i32 9
    %b22 = extractelement <16 x float> %b, i32 10
    %b23 = extractelement <16 x float> %b, i32 11
    %b30 = extractelement <16 x float> %b, i32 12
    %b31 = extractelement <16 x float> %b, i32 13
    %b32 = extractelement <16 x float> %b, i32 14
    %b33 = extractelement <16 x float> %b, i32 15

    ; Reconstruct vectors
    %b0 = call <4 x float> @matrix4x4f_util_buildVector(float %b00, float %a10, float %a20, float %a30)
    %b1 = call <4 x float> @matrix4x4f_util_buildVector(float %b01, float %a11, float %a21, float %a31)
    %b2 = call <4 x float> @matrix4x4f_util_buildVector(float %b02, float %a12, float %a22, float %a32)
    %b3 = call <4 x float> @matrix4x4f_util_buildVector(float %b03, float %a13, float %a23, float %a33)

    %v00 = fmul <4 x float> %a0, %b0
    %v01 = fmul <4 x float> %a0, %b1
    %v02 = fmul <4 x float> %a0, %b2
    %v03 = fmul <4 x float> %a0, %b3

    %v10 = fmul <4 x float> %a1, %b0
    %v11 = fmul <4 x float> %a1, %b1
    %v12 = fmul <4 x float> %a1, %b2
    %v13 = fmul <4 x float> %a1, %b3

    %v20 = fmul <4 x float> %a2, %b0
    %v21 = fmul <4 x float> %a2, %b1
    %v22 = fmul <4 x float> %a2, %b2
    %v23 = fmul <4 x float> %a2, %b3

    %v30 = fmul <4 x float> %a3, %b0
    %v31 = fmul <4 x float> %a3, %b1
    %v32 = fmul <4 x float> %a3, %b2
    %v33 = fmul <4 x float> %a3, %b3

    %s00 = call float @matrix4x4f_util_sumVector(<4 x float> %v00)
    %s01 = call float @matrix4x4f_util_sumVector(<4 x float> %v01)
    %s02 = call float @matrix4x4f_util_sumVector(<4 x float> %v02)
    %s03 = call float @matrix4x4f_util_sumVector(<4 x float> %v03)

    %s10 = call float @matrix4x4f_util_sumVector(<4 x float> %v10)
    %s11 = call float @matrix4x4f_util_sumVector(<4 x float> %v11)
    %s12 = call float @matrix4x4f_util_sumVector(<4 x float> %v12)
    %s13 = call float @matrix4x4f_util_sumVector(<4 x float> %v13)

    %s20 = call float @matrix4x4f_util_sumVector(<4 x float> %v20)
    %s21 = call float @matrix4x4f_util_sumVector(<4 x float> %v21)
    %s22 = call float @matrix4x4f_util_sumVector(<4 x float> %v22)
    %s23 = call float @matrix4x4f_util_sumVector(<4 x float> %v23)

    %s30 = call float @matrix4x4f_util_sumVector(<4 x float> %v30)
    %s31 = call float @matrix4x4f_util_sumVector(<4 x float> %v31)
    %s32 = call float @matrix4x4f_util_sumVector(<4 x float> %v32)
    %s33 = call float @matrix4x4f_util_sumVector(<4 x float> %v33)

    %s0 = call <4 x float> @matrix4x4f_util_buildVector(float %s00, float %s01, float %s02, float %s03)
    %s1 = call <4 x float> @matrix4x4f_util_buildVector(float %s10, float %s11, float %s12, float %s13)
    %s2 = call <4 x float> @matrix4x4f_util_buildVector(float %s20, float %s21, float %s22, float %s23)
    %s3 = call <4 x float> @matrix4x4f_util_buildVector(float %s30, float %s31, float %s32, float %s33)

    %pStoreVecs = bitcast <16 x float>* %pStore to <4 x float>*

    %pStore0 = getelementptr <4 x float>, <4 x float>* %pStoreVecs, i32 0
    %pStore1 = getelementptr <4 x float>, <4 x float>* %pStoreVecs, i32 1
    %pStore2 = getelementptr <4 x float>, <4 x float>* %pStoreVecs, i32 2
    %pStore3 = getelementptr <4 x float>, <4 x float>* %pStoreVecs, i32 3

    store <4 x float> %s0, <4 x float>* %pStore0
    store <4 x float> %s1, <4 x float>* %pStore1
    store <4 x float> %s2, <4 x float>* %pStore2
    store <4 x float> %s3, <4 x float>* %pStore3

    ret void
}

attributes #0 = { inlinehint nofree norecurse willreturn nosync nounwind optsize }
attributes #1 = { inlinehint nofree norecurse willreturn nosync nounwind optsize readnone alwaysinline }
