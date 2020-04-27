#pragma once

#include <NumTypes.hpp>

#include "ShaderBundleVisitor.hpp"

struct PrintSBVArgs final
{
    DEFAULT_CONSTRUCT_PU(PrintSBVArgs);
    DEFAULT_DESTRUCT(PrintSBVArgs);
    DEFAULT_COPY(PrintSBVArgs);
public:
    enum TabsOrSpaces
    {
        Spaces = 0,
        Tabs
    };

    enum NewLineChar
    {
        LF = 0,
        CR,
        CRLF
    };

    enum TessellationControlName
    {
        TessellationControl = 0,
        Hull
    };

    enum TessellationEvaluationName
    {
        TessellationEvaluation = 0,
        Domain
    };

    enum PixelName
    {
        Pixel = 0,
        Fragment
    };

    enum Braces
    {
        SameLine = 0,
        NextLine
    };
public:
    uSys indentCount;
    TabsOrSpaces indentChar;
    NewLineChar newLineChar;
    TessellationControlName tessCtrlName;
    TessellationEvaluationName tessEvalName;
    PixelName pixelName;
    Braces braces;
};

class TAU_DLL PrintShaderBundleVisitor final : public IShaderBundleVisitor
{
    DEFAULT_DESTRUCT(PrintShaderBundleVisitor);
    DELETE_COPY(PrintShaderBundleVisitor);
private:
    uSys _indentCount;
    char _indentChar;
    const char* _newLine;
    const char* _tessCtrlName;
    const char* _tessEvalName;
    const char* _pixelName;
    bool _bracesSameLine;
    uSys _currIndent;
public:
    PrintShaderBundleVisitor(const PrintSBVArgs* args) noexcept;

    void visit(const ExprAST* expr) noexcept override;
    void visit(const FileExprAST& expr) noexcept override;
    void visit(const TypedBlockExprAST& expr) noexcept override;
    void visit(const NamedBlockExprAST& expr) noexcept override;
    void visit(const ShaderIOMapPointExprAST& expr) noexcept override;
    void visit(const ShaderIOBindPointExprAST& expr) noexcept override;
private:
    void printIndent() const noexcept;

    void printComma(const ExprAST& expr) const noexcept;

    void printBrace() const noexcept;
public:
    CPPRef<IShaderProgram> getShader(IRenderingContext& ctx, IGraphicsInterface& gi) noexcept override { return null; }
    [[nodiscard]] ShaderBindMap getBindMap() noexcept override { return ShaderBindMap(RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}, RefDynArray<u32>{}); }
};
