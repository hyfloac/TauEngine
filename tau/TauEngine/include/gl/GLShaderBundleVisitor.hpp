#pragma once

#include "shader/bundle/ShaderBundleVisitor.hpp"

#include <String.hpp>
#include <DynArray.hpp>
#include <ArrayList.hpp>

#include "shader/EShader.hpp"
#include "shader/IShaderProgram.hpp"

struct BindPointUnion
{
    DELETE_COPY(BindPointUnion);
public:
    enum Type
    {
        Number = 1,
        Str
    };
public:
    Type type;
    u32 bindPoint;
    union
    {
        u32 mapPoint;
        char* bindName;
    };
public:
    BindPointUnion() noexcept
        : type(static_cast<Type>(0))
        , bindPoint(-1)
        , bindName(null)
    { }

    BindPointUnion(const u32 _bindPoint, const u32 _mapPoint) noexcept
        : type(Number)
        , bindPoint(_bindPoint)
        , mapPoint(_mapPoint)
    { }

    BindPointUnion(const u32 _bindPoint, const DynString& _bindName) noexcept
        : type(Str)
        , bindPoint(_bindPoint)
        , bindName(new(::std::nothrow) char[_bindName.length() + 1])
    { ::std::memcpy(bindName, _bindName.c_str(), _bindName.length() + 1); }

    ~BindPointUnion() noexcept
    {
        if(type == Str)
        { delete bindName; }
    }

};

struct GLShaderInfo
{
    DynString fileName;
    ArrayList<BindPointUnion> uniformPoints;
    ArrayList<BindPointUnion> texturePoints;

    inline GLShaderInfo() noexcept
        : fileName("")
        , uniformPoints(4)
        , texturePoints(4)
    { }
};

struct GLOuterShaderInfo final : public GLShaderInfo
{
    ArrayList<BindPointUnion> ioPoints;

    inline GLOuterShaderInfo() noexcept
        : GLShaderInfo()
        , ioPoints()
    { }
};

using namespace sbp;

// class TAU_DLL GLShaderBundleVisitor : public IShaderBundleVisitor
// {
//     DEFAULT_CONSTRUCT_PU(GLShaderBundleVisitor);
//     DEFAULT_DESTRUCT(GLShaderBundleVisitor);
//     DELETE_COPY(GLShaderBundleVisitor);
// public:
//     enum class BindBlock
//     {
//         Inputs = 1,
//         Outputs,
//         Uniforms,
//         Textures
//     };
// protected:
//     GLOuterShaderInfo _vertex;
//     GLShaderInfo _tessCtrl;
//     GLShaderInfo _tessEval;
//     GLShaderInfo _geometry;
//     GLOuterShaderInfo _pixel;
//
//     EShader::Stage _currentStage;
//     BindBlock _bindingBlock;
//
//     CPPRef<IShaderProgram> _builtShader;
//     RefDynArray<u32> _vertexUniMap;
//     RefDynArray<u32> _tessCtrlUniMap;
//     RefDynArray<u32> _tessEvalUniMap;
//     RefDynArray<u32> _geometryUniMap;
//     RefDynArray<u32> _pixelUniMap;
//     RefDynArray<u32> _vertexTexMap;
//     RefDynArray<u32> _tessCtrlTexMap;
//     RefDynArray<u32> _tessEvalTexMap;
//     RefDynArray<u32> _geometryTexMap;
//     RefDynArray<u32> _pixelTexMap;
// public:
//     CPPRef<IShaderProgram> getShader(IRenderingContext& ctx, IGraphicsInterface& gi) noexcept override;
//     [[nodiscard]] ShaderBindMap getBindMap() noexcept override;
//
//     void visit(const ExprAST* expr) noexcept override;
//     void visit(const FileExprAST& expr) noexcept override;
//     void visit(const TypedBlockExprAST& expr) noexcept override;
//     void visit(const ShaderIOMapPointExprAST& expr) noexcept override;
//     void visit(const ShaderIOBindPointExprAST& expr) noexcept override;
// };
