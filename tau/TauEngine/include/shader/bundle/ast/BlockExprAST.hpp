#pragma once

#include "ExprAST.hpp"
#include "shader/bundle/ShaderBundleVisitor.hpp"
#include "ShaderIOBindingExprAST.hpp"
#include "FileExprAST.hpp"
#include "RenderingMode.hpp"

#include <bitset>

#include "shader/EShader.hpp"

namespace sbp {
enum class BlockType
{
    Uniforms = 1,
    Textures
};

class TAU_DLL BlockExprAST final : public ExprAST
{
    DEFAULT_DESTRUCT(BlockExprAST);
    DEFAULT_CM_PU(BlockExprAST);
private:
    BlockType _type;
    NullableStrongRef<ShaderIOPointExprAST> _container;
public:
    inline BlockExprAST(const BlockType type, const NullableStrongRef<ShaderIOPointExprAST>& container) noexcept
        : _type(type)
        , _container(container)
    { }

    [[nodiscard]] BlockType type() const noexcept { return _type; }

    [[nodiscard]]       NullableStrongRef<ShaderIOPointExprAST>& container()       noexcept { return _container; }
    [[nodiscard]] const NullableStrongRef<ShaderIOPointExprAST>& container() const noexcept { return _container; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL ShaderStageBlockExprAST final : public ExprAST
{
    DEFAULT_DESTRUCT_VI(ShaderStageBlockExprAST);
DEFAULT_CM_PU(ShaderStageBlockExprAST);
private:
    EShader::Stage _stage;
    NullableStrongRef<FileExprAST> _file;
    NullableStrongRef<BlockExprAST> _uniforms;
    NullableStrongRef<BlockExprAST> _textures;
public:
    inline ShaderStageBlockExprAST(const EShader::Stage stage) noexcept
        : _stage(stage)
        , _file(null)
        , _uniforms(null)
        , _textures(null)
    { }

    [[nodiscard]] EShader::Stage stage() const noexcept { return _stage; }

    [[nodiscard]] NullableStrongRef<FileExprAST>&      file() noexcept { return _file;     }
    [[nodiscard]] NullableStrongRef<BlockExprAST>& uniforms() noexcept { return _uniforms; }
    [[nodiscard]] NullableStrongRef<BlockExprAST>& textures() noexcept { return _textures; }

    [[nodiscard]] const NullableStrongRef<FileExprAST>&      file() const noexcept { return _file;     }
    [[nodiscard]] const NullableStrongRef<BlockExprAST>& uniforms() const noexcept { return _uniforms; }
    [[nodiscard]] const NullableStrongRef<BlockExprAST>& textures() const noexcept { return _textures; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};

class TAU_DLL APIBlockExprAST final : public ExprAST
{
    DEFAULT_DESTRUCT(APIBlockExprAST);
DEFAULT_CM_PU(APIBlockExprAST);
public:
    using APISet = ::std::bitset<static_cast<uSys>(RenderingMode::Mode::_MAX_VALUE) + 1>;
private:
    APISet _apis;
    NullableStrongRef<ShaderStageBlockExprAST> _vertex;
    NullableStrongRef<ShaderStageBlockExprAST> _tessCtrl;
    NullableStrongRef<ShaderStageBlockExprAST> _tessEval;
    NullableStrongRef<ShaderStageBlockExprAST> _geometry;
    NullableStrongRef<ShaderStageBlockExprAST> _pixel;
    NullableStrongRef<APIBlockExprAST> _next;
public:
    inline APIBlockExprAST() noexcept
        : _apis()
        , _vertex(null)
        , _tessCtrl(null)
        , _tessEval(null)
        , _geometry(null)
        , _pixel(null)
        , _next(null)
    { }

    [[nodiscard]]       APISet& apis()       noexcept { return _apis; }
    [[nodiscard]] const APISet& apis() const noexcept { return _apis; }

    [[nodiscard]] bool hasAPI(const RenderingMode::Mode target) const noexcept
    {
        // https://lemire.me/blog/2018/02/21/iterating-over-set-bits-quickly/
        const uSys words = (_apis.size() - 1) / (CHAR_BIT * sizeof(uSys)) + 1;
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

    [[nodiscard]] NullableStrongRef<ShaderStageBlockExprAST>&   vertex() noexcept { return _vertex;   }
    [[nodiscard]] NullableStrongRef<ShaderStageBlockExprAST>& tessCtrl() noexcept { return _tessCtrl; }
    [[nodiscard]] NullableStrongRef<ShaderStageBlockExprAST>& tessEval() noexcept { return _tessEval; }
    [[nodiscard]] NullableStrongRef<ShaderStageBlockExprAST>& geometry() noexcept { return _geometry; }
    [[nodiscard]] NullableStrongRef<ShaderStageBlockExprAST>&    pixel() noexcept { return _pixel;    }

    [[nodiscard]] const NullableStrongRef<ShaderStageBlockExprAST>&   vertex() const noexcept { return _vertex;   }
    [[nodiscard]] const NullableStrongRef<ShaderStageBlockExprAST>& tessCtrl() const noexcept { return _tessCtrl; }
    [[nodiscard]] const NullableStrongRef<ShaderStageBlockExprAST>& tessEval() const noexcept { return _tessEval; }
    [[nodiscard]] const NullableStrongRef<ShaderStageBlockExprAST>& geometry() const noexcept { return _geometry; }
    [[nodiscard]] const NullableStrongRef<ShaderStageBlockExprAST>&    pixel() const noexcept { return _pixel;    }

    [[nodiscard]]       NullableStrongRef<APIBlockExprAST>& next()       noexcept { return _next; }
    [[nodiscard]] const NullableStrongRef<APIBlockExprAST>& next() const noexcept { return _next; }

    void visit(IShaderBundleVisitor& visitor) const noexcept override
    { visitor.visit(*this); }
};
}
