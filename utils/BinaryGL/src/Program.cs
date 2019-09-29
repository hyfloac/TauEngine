using System;

namespace BinaryGL
{
    static class Program
    {
        static void Main(string[] args)
        {
            Structure structure = new Structure("Material", 5);
            structure.Members[0] = new StructureMember { Type = VariableTypes.FVec4, ArraySizes = null, Name = "color" };
            structure.Members[1] = new StructureMember { Type = VariableTypes.Bool, ArraySizes = null, Name = "useLight" };
            structure.Members[2] = new StructureMember { Type = VariableTypes.Sampler2D, ArraySizes = null, Name = "textureSampler" };
            structure.Members[3] = new StructureMember { Type = VariableTypes.Bool, ArraySizes = null, Name = "useTexture" };
            structure.Members[4] = new StructureMember { Type = VariableTypes.Float, ArraySizes = null, Name = "reflectance" };

            Console.WriteLine(structure.GetCompactString());
            Console.WriteLine();
            Console.WriteLine(structure.GetCleanString());

            Console.WriteLine("\n");

            IOVariable v_pos = new IOVariable(PassThroughType.INPUT, VariableTypes.FVec3, "pos", 0);
            IOVariable v_norm = new IOVariable(PassThroughType.INPUT, VariableTypes.FVec3, "normals", 1);
            IOVariable v_tex = new IOVariable(PassThroughType.INPUT, VariableTypes.FVec2, "textureCoords", 2);
            
            IOVariable v_out_tex = new IOVariable(PassThroughType.OUTPUT, VariableTypes.FVec2, "f_textureCoords", 3);

            IOVariable f_tex = new IOVariable(PassThroughType.INPUT, VariableTypes.FVec2, "f_textureCoords", 3);
            IOVariable f_out_color = new IOVariable(PassThroughType.OUTPUT, VariableTypes.FVec4, "fragColor", 0);

            Console.WriteLine("#Vertex");
            Console.WriteLine(v_pos.GetCleanString());
            Console.WriteLine(v_norm.GetCleanString());
            Console.WriteLine(v_tex.GetCleanString());
            Console.WriteLine();
            Console.WriteLine(v_out_tex.GetCleanString());
            Console.WriteLine();
            Console.WriteLine("#Fragment");
            Console.WriteLine(f_tex.GetCleanString());
            Console.WriteLine();
            Console.WriteLine(f_out_color.GetCleanString());
            Console.WriteLine("\n");

            Console.WriteLine("#Vertex");
            Console.WriteLine(v_pos.GetCompactString());
            Console.WriteLine(v_norm.GetCompactString());
            Console.WriteLine(v_tex.GetCompactString());
            Console.WriteLine();
            Console.WriteLine(v_out_tex.GetCompactString());
            Console.WriteLine();
            Console.WriteLine("#Fragment");
            Console.WriteLine(f_tex.GetCompactString());
            Console.WriteLine();
            Console.WriteLine(f_out_color.GetCompactString());
            Console.WriteLine("\n");

            Console.WriteLine("\n");

            FunctionPrototype f0 = new FunctionPrototype(VariableTypes.Float, "calc", 0, VariableTypes.Float, VariableTypes.BVec2, VariableTypes.DMat3x4);
            FunctionPrototype f1 = new FunctionPrototype(VariableTypes.Int, "get", 1);

            Console.WriteLine(f0.GetCleanString());
            Console.WriteLine(f0.GetCompactString());
            Console.WriteLine();
            Console.WriteLine(f1.GetCleanString());
            Console.WriteLine(f1.GetCompactString());
            Console.WriteLine();

            Console.ReadKey();
        }
    }
}
