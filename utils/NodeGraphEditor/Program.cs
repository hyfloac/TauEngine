using System;
using NodeGraph.Node.Common;
using NodeGraph.Node.Common.Comparison;
using NodeGraph.Node.Common.Math;

namespace NodeGraphEditor
{
    static class Program
    {
        static int Main(string[] args)
        {
            ConstantNode<int> a = new ConstantNode<int>("A", 302);
            ConstantNode<float> b = new ConstantNode<float>("B", 72.0058f);
            ConstantNode<float> c = new ConstantNode<float>("C", 17.684f);

            UnaryOperatorNode<float, int> fToI = new TypeCastNode<float, int>("Type Cast Test");
            BinaryOperatorNode<int> add = new IntAddNode("Add Test");
            UnaryOperatorNode<int, float> iToF = new TypeCastNode<int, float>("Type Cast Test 2");
            FloatDivideNode divide = new FloatDivideNode("Divide Test");
            UnaryOperatorNode<float, double> sqrt = new FloatSqrtNode("Sqrt Test");

            fToI.Connect(b.OutValue, "In");

            add.Connect(a.OutValue, "X");
            add.Connect(fToI.OutResult, "Y");

            iToF.Connect(add.OutResult, "In");

            divide.Connect(iToF.OutResult, "X");
            divide.Connect(c.OutValue, "Y");

            sqrt.Connect(divide.OutResult, "In");

            sqrt.Compute();

            Console.WriteLine("Computed Value: {0}", sqrt.OutResult.Value);
            Console.WriteLine("True Value: {0}", Math.Sqrt((float) (302 + (int) 72.0058f) / 17.684f));

            ConstantNode<float> ctrl = new ConstantNode<float>("Ctrl", 4.6f);
            TypeCastNode<double, float> dToF = new TypeCastNode<double, float>("Type Cast Test 3");
            FloatGreaterThanNode compNode = new FloatGreaterThanNode("Float Comparison");
            SelectiveValue<float> selector = new SelectiveValue<float>("Selective Test");

            dToF.Connect(sqrt.OutResult, "In");

            compNode.Connect(ctrl.OutValue, "X");
            compNode.Connect(dToF.OutResult, "Y");

            selector.Connect(compNode.OutResult, "Control");
            selector.Connect(b.OutValue, "X");
            selector.Connect(c.OutValue, "Y");

            selector.Compute();

            Console.WriteLine("Selected Value: {0}", selector.OutValue.Value);

            Console.ReadKey(true);
            
            return 0;
        }
    }
}
