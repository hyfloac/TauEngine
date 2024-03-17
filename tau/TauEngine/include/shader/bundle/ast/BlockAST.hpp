#pragma once

#pragma warning(push, 0)
#include <bitset>
#pragma warning(pop)

#include "AST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "shader/bundle/ShaderBundleLexer.hpp"
#include "FileAST.hpp"
#include "RenderingMode.hpp"
#include "shader/EShader.hpp"

namespace sbp {
class TAU_DLL UniformBindingAST final : public AST
{
    DEFAULT_DESTRUCT(UniformBindingAST);
    DEFAULT_CM_PU(UniformBindingAST);
private:
    NullableStrongRef<UniformBindingAST> _next;
    CommonRenderingModelToken _crmTarget;
    BindingUnion _bindPoint;
public:
    UniformBindingAST(const NullableStrongRef<UniformBindingAST>& next, const CommonRenderingModelToken crmTarget, const BindingUnion& bindPoint) noexcept
        : _next(next)
        , _crmTarget(crmTarget)
        , _bindPoint(bindPoint)
    { }

    UniformBindingAST(const NullableStrongRef<UniformBindingAST>& next, const CommonRenderingModelToken crmTarget, BindingUnion&& bindPoint) noexcept
        : _next(next)
        , _crmTarget(crmTarget)
        , _bindPoint(::std::move(bindPoint))
    { }

    [[nodiscard]]       NullableStrongRef<UniformBindingAST>& next()       noexcept { return _next; }
    [[nodiscard]] const NullableStrongRef<UniformBindingAST>& next() const noexcept { return _next; }

    [[nodiscard]] CommonRenderingModelToken crmTarget() const noexcept { return _crmTarget; }
    [[nodiscard]] const BindingUnion& bindPoint() const noexcept { return _bindPoint; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL TextureParamsBlockAST final : public AST
{
    DEFAULT_DESTRUCT(TextureParamsBlockAST);
    DEFAULT_CM_PU(TextureParamsBlockAST);
private:
    NullableStrongRef<TextureParamsBlockAST> _next;
    CommonRenderingModelToken _crmTarget;
    BindingUnion _bindPoint;
    u32 _sampler;
public:
    TextureParamsBlockAST(const NullableStrongRef<TextureParamsBlockAST>& next, const CommonRenderingModelToken crmTarget, const BindingUnion& bindPoint, const u32 sampler) noexcept
        : _next(next)
        , _crmTarget(crmTarget)
        , _bindPoint(bindPoint)
        , _sampler(sampler)
    { }

    TextureParamsBlockAST(const NullableStrongRef<TextureParamsBlockAST>& next, const CommonRenderingModelToken crmTarget, BindingUnion&& bindPoint, const u32 sampler) noexcept
        : _next(next)
        , _crmTarget(crmTarget)
        , _bindPoint(::std::move(bindPoint))
        , _sampler(sampler)
    { }
    
    [[nodiscard]]       NullableStrongRef<TextureParamsBlockAST>& next()       noexcept { return _next; }
    [[nodiscard]] const NullableStrongRef<TextureParamsBlockAST>& next() const noexcept { return _next; }

    [[nodiscard]] CommonRenderingModelToken crmTarget() const noexcept { return _crmTarget; }
    [[nodiscard]] const BindingUnion& bindPoint() const noexcept { return _bindPoint; }
    [[nodiscard]] u32 sampler() const noexcept { return _sampler; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL ShaderStageBlockAST final : public AST
{
    DEFAULT_DESTRUCT_VI(ShaderStageBlockAST);
    DEFAULT_CM_PU(ShaderStageBlockAST);
private:
    EShader::Stage _stage;
    NullableStrongRef<FileAST> _file;
    NullableStrongRef<UniformBlockExprAST> _uniforms;
    NullableStrongRef<TextureParamsBlockAST> _textures;
public:
    ShaderStageBlockAST(const EShader::Stage stage) noexcept
        : _stage(stage)
        , _file(nullptr)
        , _uniforms(nullptr)
        , _textures(nullptr)
    { }

    [[nodiscard]] EShader::Stage stage() const noexcept { return _stage; }

    [[nodiscard]] NullableStrongRef<FileAST>&                   file() noexcept { return _file;     }
    [[nodiscard]] NullableStrongRef<UniformBlockExprAST>&       uniforms() noexcept { return _uniforms; }
    [[nodiscard]] NullableStrongRef<TextureParamsBlockAST>& textures() noexcept { return _textures; }

    [[nodiscard]] const NullableStrongRef<FileAST>&                   file() const noexcept { return _file;     }
    [[nodiscard]] const NullableStrongRef<UniformBlockExprAST>&       uniforms() const noexcept { return _uniforms; }
    [[nodiscard]] const NullableStrongRef<TextureParamsBlockAST>& textures() const noexcept { return _textures; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL APIBlockAST final : public AST
{
    DEFAULT_CONSTRUCT_PU(APIBlockAST);
    DEFAULT_DESTRUCT(APIBlockAST);
    DEFAULT_CM_PU(APIBlockAST);
public:
    using APISet = ::std::bitset<static_cast<uSys>(RenderingMode::Mode::MAX_VALUE) + 1>;
private:
    APISet _apis;
    StrongRef<ShaderStageBlockAST> _vertex;
    StrongRef<ShaderStageBlockAST> _tessCtrl;
    StrongRef<ShaderStageBlockAST> _tessEval;
    StrongRef<ShaderStageBlockAST> _geometry;
    StrongRef<ShaderStageBlockAST> _pixel;
    StrongRef<APIBlockAST> _next;
public:
    [[nodiscard]]       APISet& apis()       noexcept { return _apis; }
    [[nodiscard]] const APISet& apis() const noexcept { return _apis; }

    [[nodiscard]] bool hasAPI(const RenderingMode::Mode target) const noexcept
    {
        // https://lemire.me/blog/2018/02/21/iterating-over-set-bits-quickly/
        constexpr uSys words = (_apis.size() - 1) / (CHAR_BIT * sizeof(uSys)) + 1;
        for(uSys k = 0; k < words; ++k)
        {
            u64 bitSet = _apis._Getword(k);
            while(bitSet != 0)
            {
                const u64 t = bitSet & -bitSet;
                unsigned long r;
                _BitScanForward64(&r, bitSet);
                bitSet ^= t;

                const u64 bit = k * 64 + r;

                const RenderingMode::Mode api = static_cast<RenderingMode::Mode>(bit);
                if(api == target)
                {
                    return true;
                }
            }
        }
        return false;
    }

    [[nodiscard]] StrongRef<ShaderStageBlockAST>&   vertex() noexcept { return _vertex;   }
    [[nodiscard]] StrongRef<ShaderStageBlockAST>& tessCtrl() noexcept { return _tessCtrl; }
    [[nodiscard]] StrongRef<ShaderStageBlockAST>& tessEval() noexcept { return _tessEval; }
    [[nodiscard]] StrongRef<ShaderStageBlockAST>& geometry() noexcept { return _geometry; }
    [[nodiscard]] StrongRef<ShaderStageBlockAST>&    pixel() noexcept { return _pixel;    }

    [[nodiscard]] const StrongRef<ShaderStageBlockAST>&   vertex() const noexcept { return _vertex;   }
    [[nodiscard]] const StrongRef<ShaderStageBlockAST>& tessCtrl() const noexcept { return _tessCtrl; }
    [[nodiscard]] const StrongRef<ShaderStageBlockAST>& tessEval() const noexcept { return _tessEval; }
    [[nodiscard]] const StrongRef<ShaderStageBlockAST>& geometry() const noexcept { return _geometry; }
    [[nodiscard]] const StrongRef<ShaderStageBlockAST>&    pixel() const noexcept { return _pixel;    }

    [[nodiscard]]       StrongRef<APIBlockAST>& next()       noexcept { return _next; }
    [[nodiscard]] const StrongRef<APIBlockAST>& next() const noexcept { return _next; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
