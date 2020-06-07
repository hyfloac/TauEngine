#pragma once

#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "RenderingMode.hpp"
#include "shader/EShader.hpp"

#include <String.hpp>
#include <ArrayList.hpp>

#include "ShaderBundleLexer.hpp"

class ShaderInfoExtractorVisitor;

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
    CommonRenderingModelToken crmTarget;
    union
    {
        i32 mapPoint;
        char* bindName;
    };
public:
    BindPointUnion() noexcept
        : type(static_cast<Type>(0))
        , crmTarget(static_cast<CommonRenderingModelToken>(-1))
        , bindName(null)
    { }

    BindPointUnion(const CommonRenderingModelToken _crmTarget, const u32 _mapPoint) noexcept
        : type(Number)
        , crmTarget(_crmTarget)
        , mapPoint(_mapPoint)
    { }

    BindPointUnion(const CommonRenderingModelToken _crmTarget, const DynString& _bindName) noexcept
        : type(Str)
        , crmTarget(_crmTarget)
        , bindName(new(::std::nothrow) char[_bindName.length() + 1])
    { ::std::memcpy(bindName, _bindName.c_str(), _bindName.length() + 1); }

    ~BindPointUnion() noexcept
    {
        if(type == Str)
        { delete[] bindName; }
    }

    BindPointUnion(BindPointUnion&& move) noexcept
        : type(move.type)
        , crmTarget(move.crmTarget)
    {
        if(type == Str)
        {
            bindName = move.bindName;
            move.bindName = null;
        }
        else if(type == Number)
        { mapPoint = move.mapPoint; }
    }

    BindPointUnion& operator=(BindPointUnion&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        type = move.type;
        crmTarget = move.crmTarget;

        if(type == Str)
        {
            bindName = move.bindName;
            move.bindName = null;
        }
        else if(type == Number)
        { mapPoint = move.mapPoint; }

        return *this;
    }
};

struct ShaderInfo final
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

enum class BlockType;

class ShaderInfoIterator final
{
    DEFAULT_DESTRUCT(ShaderInfoIterator);
    DEFAULT_CM(ShaderInfoIterator);
private:
    const ShaderInfoExtractorVisitor* _visitor;
    const ShaderInfo* _shaderInfo;
    EShader::Stage _stage;
public:
    inline ShaderInfoIterator(const ShaderInfoExtractorVisitor* const visitor, const ShaderInfo* const shaderInfo, const EShader::Stage stage) noexcept
        : _visitor(visitor)
        , _shaderInfo(shaderInfo)
        , _stage(stage)
    { }

    inline ShaderInfoIterator& operator++() noexcept;
    inline ShaderInfoIterator& operator--() noexcept;

    inline ShaderInfoIterator operator++(int) noexcept;
    inline ShaderInfoIterator operator--(int) noexcept;

    [[nodiscard]] const ShaderInfo& operator*() const noexcept { return *_shaderInfo; }

    [[nodiscard]] const ShaderInfo* shaderInfo() const noexcept { return _shaderInfo; }
    [[nodiscard]] EShader::Stage stage() const noexcept { return _stage; }

    [[nodiscard]] inline bool operator ==(const ShaderInfoIterator& other) const noexcept { return _stage == other._stage; }
    [[nodiscard]] inline bool operator !=(const ShaderInfoIterator& other) const noexcept { return _stage != other._stage; }
};
}

class TAU_DLL ShaderInfoExtractorVisitor final : public IShaderBundleVisitor
{
    DEFAULT_DESTRUCT(ShaderInfoExtractorVisitor);
    DEFAULT_CM_PU(ShaderInfoExtractorVisitor);
private:
    RenderingMode::Mode _targetMode;
    EShader::Stage _currentStage;
    sbp::BlockType _currentBlock;

    sbp::ShaderInfo _vertexInfo;
    sbp::ShaderInfo _tessCtrlInfo;
    sbp::ShaderInfo _tessEvalInfo;
    sbp::ShaderInfo _geometryInfo;
    sbp::ShaderInfo _pixelInfo;
public:
    inline ShaderInfoExtractorVisitor(const RenderingMode::Mode targetMode) noexcept
        : _targetMode(targetMode)
        , _currentStage(static_cast<EShader::Stage>(0))
        , _currentBlock(static_cast<sbp::BlockType>(0))
    { }

    [[nodiscard]] const sbp::ShaderInfo&   vertexInfo() const noexcept { return _vertexInfo;   }
    [[nodiscard]] const sbp::ShaderInfo& tessCtrlInfo() const noexcept { return _tessCtrlInfo; }
    [[nodiscard]] const sbp::ShaderInfo& tessEvalInfo() const noexcept { return _tessEvalInfo; }
    [[nodiscard]] const sbp::ShaderInfo& geometryInfo() const noexcept { return _geometryInfo; }
    [[nodiscard]] const sbp::ShaderInfo&    pixelInfo() const noexcept { return _pixelInfo;    }

    [[nodiscard]] const sbp::ShaderInfo& get(const EShader::Stage stage) const noexcept
    {
        switch(stage)
        {
            case EShader::Stage::Vertex: return _vertexInfo;
            case EShader::Stage::TessellationControl: return _tessCtrlInfo;
            case EShader::Stage::TessellationEvaluation: return _tessEvalInfo;
            case EShader::Stage::Geometry: return _geometryInfo;
            case EShader::Stage::Pixel: return _pixelInfo;
            default: return _vertexInfo;
        }
    }

    void reset() noexcept
    {
        _vertexInfo   = { };
        _tessCtrlInfo = { };
        _tessEvalInfo = { };
        _geometryInfo = { };
        _pixelInfo    = { };
    }

    [[nodiscard]] sbp::ShaderInfoIterator begin() const noexcept { return sbp::ShaderInfoIterator(this, &_vertexInfo, EShader::Stage::Vertex); }
    [[nodiscard]] sbp::ShaderInfoIterator   end() const noexcept { return sbp::ShaderInfoIterator(this, &_pixelInfo, EShader::Stage::Pixel); }

    void visit(const sbp::ExprAST* expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::ExprAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::RootExprAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::FileExprAST& expr) noexcept override;
    void visit(const sbp::BlockExprAST& expr) noexcept override;
    void visit(const sbp::ShaderStageBlockExprAST& expr) noexcept override;
    void visit(const sbp::APIBlockExprAST& expr) noexcept override;
    void visit(const sbp::ShaderIOMapPointExprAST& expr) noexcept override;
    void visit(const sbp::ShaderIOBindPointExprAST& expr) noexcept override;
};

inline sbp::ShaderInfoIterator& sbp::ShaderInfoIterator::operator++() noexcept
{
    if(_stage < EShader::Stage::Pixel)
    {
        _stage = static_cast<EShader::Stage>(static_cast<uSys>(_stage) + 1);
        _shaderInfo = &_visitor->get(_stage);
    }
    return *this;
}

inline sbp::ShaderInfoIterator& sbp::ShaderInfoIterator::operator--() noexcept
{
    if(_stage > EShader::Stage::Vertex)
    {
        _stage = static_cast<EShader::Stage>(static_cast<uSys>(_stage) - 1);
        _shaderInfo = &_visitor->get(_stage);
    }
    return *this;
}

inline sbp::ShaderInfoIterator sbp::ShaderInfoIterator::operator++(int) noexcept
{
    const ShaderInfoIterator copy(*this);
    this->operator++();
    return copy;
}

inline sbp::ShaderInfoIterator sbp::ShaderInfoIterator::operator--(int) noexcept
{
    const ShaderInfoIterator copy(*this);
    this->operator--();
    return copy;
}
