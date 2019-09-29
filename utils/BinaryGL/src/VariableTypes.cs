namespace BinaryGL
{
    public enum VariableType : byte
    {
        Bool,
        Int, 
        UInt,
        Float,
        Double,

        BVec2,
        BVec3,
        BVec4,

        IVec2,
        IVec3,
        IVec4,

        UVec2,
        UVec3,
        UVec4,

        FVec2,
        FVec3,
        FVec4,

        DVec2,
        DVec3,
        DVec4,

        FMat2,
        FMat3,
        FMat4,

        DMat2,
        DMat3,
        DMat4,

        FMat2x2,
        FMat2x3,
        FMat2x4,

        FMat3x2,
        FMat3x3,
        FMat3x4,

        FMat4x2,
        FMat4x3,
        FMat4x4,

        DMat2x2,
        DMat2x3,
        DMat2x4,
        
        DMat3x2,
        DMat3x3,
        DMat3x4,
        
        DMat4x2,
        DMat4x3,
        DMat4x4,

        Struct,
        Void,
        Sampler2D,
        SamplerCube
    }

    public static class VariableTypeUtils
    {
        public static string GetGLSLName(this VariableType type)
        {
            switch(type)
            {
                case VariableType.Bool:        return "bool";
                case VariableType.Int:         return "int";
                case VariableType.UInt:        return "uint";
                case VariableType.Float:       return "float";
                case VariableType.Double:      return "double";
                                                
                case VariableType.BVec2:       return "bvec2";
                case VariableType.BVec3:       return "bvec3";
                case VariableType.BVec4:       return "bvec4";
                                                
                case VariableType.IVec2:       return "ivec2";
                case VariableType.IVec3:       return "ivec3";
                case VariableType.IVec4:       return "ivec4";
                                                
                case VariableType.UVec2:       return "uvec2";
                case VariableType.UVec3:       return "uvec3";
                case VariableType.UVec4:       return "uvec4";
                                                
                case VariableType.FVec2:       return "vec2";
                case VariableType.FVec3:       return "vec3";
                case VariableType.FVec4:       return "vec4";
                                                
                case VariableType.DVec2:       return "dvec2";
                case VariableType.DVec3:       return "dvec3";
                case VariableType.DVec4:       return "dvec4";
                                                
                case VariableType.FMat2:       return "mat2";
                case VariableType.FMat3:       return "mat3";
                case VariableType.FMat4:       return "mat4";
                                                
                case VariableType.DMat2:       return "dmat2";
                case VariableType.DMat3:       return "dmat3";
                case VariableType.DMat4:       return "dmat4";
                                                
                case VariableType.FMat2x2:     return "mat2x2";
                case VariableType.FMat2x3:     return "mat2x3";
                case VariableType.FMat2x4:     return "mat2x4";
                                                
                case VariableType.FMat3x2:     return "mat3x2";
                case VariableType.FMat3x3:     return "mat3x3";
                case VariableType.FMat3x4:     return "mat3x4";
                                                
                case VariableType.FMat4x2:     return "mat4x2";
                case VariableType.FMat4x3:     return "mat4x3";
                case VariableType.FMat4x4:     return "mat4x4";
                                                
                case VariableType.DMat2x2:     return "dmat2x2";
                case VariableType.DMat2x3:     return "dmat2x3";
                case VariableType.DMat2x4:     return "dmat2x4";
                                                
                case VariableType.DMat3x2:     return "dmat3x2";
                case VariableType.DMat3x3:     return "dmat3x3";
                case VariableType.DMat3x4:     return "dmat3x4";
                                                
                case VariableType.DMat4x2:     return "mat4x2";
                case VariableType.DMat4x3:     return "dmat4x3";
                case VariableType.DMat4x4:     return "dmat4x4";
                                                
                case VariableType.Struct:      return "struct";
                case VariableType.Void:        return "void";
                case VariableType.Sampler2D:   return "sampler2D";
                case VariableType.SamplerCube: return "samplerCube";
            }

            return "";
        }
    }


}
