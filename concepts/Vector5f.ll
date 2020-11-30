%struct.Vector5f = type { float, float, float, float, float }

define void @vector5f_add(%struct.Vector5f* noalias nocapture sret(%struct.Vector5f) align 16 %pRet, %struct.Vector5f* nocapture readonly align 16 %pA, %struct.Vector5f* nocapture readonly align 16 %pB) local_unnamed_addr #1
{
    %pAHigh = bitcast %struct.Vector5f* %pA to <4 x float>*
    %pBHigh = bitcast %struct.Vector5f* %pB to <4 x float>*

    %pALow = getelementptr inbounds %struct.Vector5f, %struct.Vector5f* %pA, i64 0, i32 4
    %pBLow = getelementptr inbounds %struct.Vector5f, %struct.Vector5f* %pB, i64 0, i32 4

    %aHigh = load <4 x float>, <4 x float>* %pAHigh, align 16
    %bHigh = load <4 x float>, <4 x float>* %pBHigh, align 16

    %aLow = load float, float* %pALow, align 16
    %bLow = load float, float* %pBLow, align 16

    %valHigh = fadd <4 x float> %aHigh, %bHigh
    %valLow = fadd float %aLow, %bLow

    %pRetHigh = bitcast %struct.Vector5f* %pRet to <4 x float>*
    %pRetLow = getelementptr inbounds %struct.Vector5f, %struct.Vector5f* %pRet, i64 0, i32 4

    store <4 x float> %valHigh, <4 x float>* %pRetHigh, align 16
    store float %valLow, float* %pRetLow, align 16

    ret void
}

attributes #1 = { inlinehint nofree norecurse willreturn nosync nounwind optsize argmemonly }
