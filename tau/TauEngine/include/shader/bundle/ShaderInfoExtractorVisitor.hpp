#pragma once

#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "RenderingMode.hpp"
#include "shader/EShader.hpp"

#include <String.hpp>
#include <ArrayList.hpp>

namespace sbp {
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
        i32 mapPoint;
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
    {
        ::std::memcpy(bindName, _bindName.c_str(), _bindName.length() + 1);
    }

    ~BindPointUnion() noexcept
    {
        if(type == Str)
        { delete bindName; }
    }
};

struct ShaderInfo
{
    DynString fileName;
    ArrayList<BindPointUnion> uniformPoints;
    ArrayList<BindPointUnion> texturePoints;

    inline ShaderInfo() noexcept
        : fileName("")
        , uniformPoints(4)
        , texturePoints(4)
    { }
};

struct OuterShaderInfo final : public ShaderInfo
{
    ArrayList<BindPointUnion> ioPoints;

    inline OuterShaderInfo() noexcept
        : ShaderInfo()
        , ioPoints(4)
    { }
};

enum class BlockType;
}

class TAU_DLL ShaderInfoExtractorVisitor final : public IShaderBundleVisitor
{
    DEFAULT_DESTRUCT(ShaderInfoExtractorVisitor);
    DELETE_COPY(ShaderInfoExtractorVisitor);
private:
    RenderingMode::Mode _targetMode;
    EShader::Stage _currentStage;
    sbp::BlockType _currentBlock;

    sbp::OuterShaderInfo _vertexInfo;
    sbp::ShaderInfo _tessCtrlInfo;
    sbp::ShaderInfo _tessEvalInfo;
    sbp::ShaderInfo _geometryInfo;
    sbp::OuterShaderInfo _pixelInfo;
public:
    inline ShaderInfoExtractorVisitor(const RenderingMode::Mode targetMode) noexcept
        : _targetMode(targetMode)
        , _currentStage(static_cast<EShader::Stage>(0))
        , _currentBlock(static_cast<sbp::BlockType>(0))
    { }

    [[nodiscard]] const sbp::OuterShaderInfo& vertexInfo() const noexcept { return _vertexInfo;   }
    [[nodiscard]] const sbp::ShaderInfo&    tessCtrlInfo() const noexcept { return _tessCtrlInfo; }
    [[nodiscard]] const sbp::ShaderInfo&    tessEvalInfo() const noexcept { return _tessEvalInfo; }
    [[nodiscard]] const sbp::ShaderInfo&    geometryInfo() const noexcept { return _geometryInfo; }
    [[nodiscard]] const sbp::OuterShaderInfo&  pixelInfo() const noexcept { return _pixelInfo;    }

    void reset() noexcept
    {
        _vertexInfo = {};
        _tessCtrlInfo = {};
        _tessEvalInfo = {};
        _geometryInfo = {};
        _pixelInfo = {};
    }

    void visit(const sbp::ExprAST* expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::ExprAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::RootExprAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::FileExprAST& expr) noexcept override;
    void visit(const sbp::BlockExprAST& expr) noexcept override;
    void visit(const sbp::ShaderStageBlockExprAST& expr) noexcept override;
    void visit(const sbp::OuterShaderStageBlockExprAST& expr) noexcept override;
    void visit(const sbp::APIBlockExprAST& expr) noexcept override;
    void visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept override;
    void visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept override;
};
