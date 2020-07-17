#pragma once

#include <String.hpp>
#include <ArrayList.hpp>

#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "RenderingMode.hpp"
#include "shader/EShader.hpp"

#include "ShaderBundleLexer.hpp"
#include "ast/AST.hpp"

class ShaderInfoExtractorVisitor;

namespace sbp {
struct UniformBindPoint final
{
    DEFAULT_DESTRUCT(UniformBindPoint);
    DEFAULT_CM_PU(UniformBindPoint);
public:
    CommonRenderingModelToken crmTarget;
    BindingUnion binding;
public:
    UniformBindPoint() noexcept
        : crmTarget(static_cast<CommonRenderingModelToken>(-1))
    { }

    UniformBindPoint(const CommonRenderingModelToken _crmTarget, const u32 _mapPoint) noexcept
        : crmTarget(_crmTarget)
        , binding(_mapPoint)
    { }

    UniformBindPoint(const CommonRenderingModelToken _crmTarget, const DynString& _bindName) noexcept
        : crmTarget(_crmTarget)
        , binding(_bindName)
    { }

    UniformBindPoint(const CommonRenderingModelToken _crmTarget, const BindingUnion& _bindingUnion) noexcept
        : crmTarget(_crmTarget)
        , binding(_bindingUnion)
    { }

    UniformBindPoint(const CommonRenderingModelToken _crmTarget, BindingUnion&& _bindingUnion) noexcept
        : crmTarget(_crmTarget)
        , binding(::std::move(_bindingUnion))
    { }
};

struct TextureBindPoint final
{
    DEFAULT_DESTRUCT(TextureBindPoint);
    DEFAULT_CM_PU(TextureBindPoint);
public:
    CommonRenderingModelToken crmTarget;
    BindingUnion binding;
    u32 sampler;
public:
    TextureBindPoint() noexcept
        : crmTarget(static_cast<CommonRenderingModelToken>(-1))
        , sampler(0)
    { }

    TextureBindPoint(const CommonRenderingModelToken _crmTarget, const u32 _mapPoint, const u32 _sampler) noexcept
        : crmTarget(_crmTarget)
        , binding(_mapPoint)
        , sampler(_sampler)
    { }

    TextureBindPoint(const CommonRenderingModelToken _crmTarget, const DynString& _bindName, const u32 _sampler) noexcept
        : crmTarget(_crmTarget)
        , binding(_bindName)
        , sampler(_sampler)
    { }

    TextureBindPoint(const CommonRenderingModelToken _crmTarget, const BindingUnion& _bindingUnion, const u32 _sampler) noexcept
        : crmTarget(_crmTarget)
        , binding(_bindingUnion)
        , sampler(_sampler)
    { }

    TextureBindPoint(const CommonRenderingModelToken _crmTarget, BindingUnion&& _bindingUnion, const u32 _sampler) noexcept
        : crmTarget(_crmTarget)
        , binding(::std::move(_bindingUnion))
        , sampler(_sampler)
    { }
};

struct ShaderInfo final
{
    DynString fileName;
    ArrayList<UniformBindPoint> uniformPoints;
    ArrayList<TextureBindPoint> texturePoints;

    ShaderInfo() noexcept
        : fileName("")
        , uniformPoints(256)
        , texturePoints(256)
    { }
};

class ShaderInfoIterator final
{
    DEFAULT_DESTRUCT(ShaderInfoIterator);
    DEFAULT_CM(ShaderInfoIterator);
private:
    const ShaderInfoExtractorVisitor* _visitor;
    const ShaderInfo* _shaderInfo;
    EShader::Stage _stage;
public:
    ShaderInfoIterator(const ShaderInfoExtractorVisitor* const visitor, const ShaderInfo* const shaderInfo, const EShader::Stage stage) noexcept
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

    sbp::ShaderInfo _vertexInfo;
    sbp::ShaderInfo _tessCtrlInfo;
    sbp::ShaderInfo _tessEvalInfo;
    sbp::ShaderInfo _geometryInfo;
    sbp::ShaderInfo _pixelInfo;
public:
    inline ShaderInfoExtractorVisitor(const RenderingMode::Mode targetMode) noexcept
        : _targetMode(targetMode)
        , _currentStage(static_cast<EShader::Stage>(0))
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
            case EShader::Stage::Vertex:                 return _vertexInfo;
            case EShader::Stage::TessellationControl:    return _tessCtrlInfo;
            case EShader::Stage::TessellationEvaluation: return _tessEvalInfo;
            case EShader::Stage::Geometry:               return _geometryInfo;
            case EShader::Stage::Pixel:                  return _pixelInfo;
            default:                                     return _vertexInfo;
        }
    }

    [[nodiscard]] sbp::ShaderInfo& get(const EShader::Stage stage) noexcept
    {
        switch(stage)
        {
            case EShader::Stage::Vertex:                 return _vertexInfo;
            case EShader::Stage::TessellationControl:    return _tessCtrlInfo;
            case EShader::Stage::TessellationEvaluation: return _tessEvalInfo;
            case EShader::Stage::Geometry:               return _geometryInfo;
            case EShader::Stage::Pixel:                  return _pixelInfo;
            default:                                     return _vertexInfo;
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

    void visit(const sbp::AST* expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::AST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::RootAST& expr) noexcept override
    { IShaderBundleVisitor::visit(expr); }

    void visit(const sbp::FileAST& expr) noexcept override;
    void visit(const sbp::UniformBindingAST& expr) noexcept override;
    void visit(const sbp::TextureParamsBlockAST& expr) noexcept override;
    void visit(const sbp::ShaderStageBlockAST& expr) noexcept override;
    void visit(const sbp::APIBlockAST& expr) noexcept override;
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
