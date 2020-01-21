#pragma once

#include "model/IVertexArray.hpp"
#include <GL/glew.h>

class TAU_DLL GLVertexArray final : public IVertexArray
{
    DELETE_COPY(GLVertexArray);
public:
    static GLuint generate() noexcept;

    static void _bind(GLuint vao) noexcept;

    static void destroy(GLuint vao) noexcept;
    
    static GLenum getGLType(ShaderDataType::Type type) noexcept;
private:
    GLenum _glDrawType;
public:
    GLVertexArray(uSys bufferCount, DrawType drawType);

    ~GLVertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    void internalSetup(IRenderingContext& context) noexcept override;

    void preDraw(IRenderingContext& context) noexcept override;

    void postDraw(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context) noexcept override;

    void drawIndexed(IRenderingContext& context) noexcept override;

    void drawType(DrawType drawType) noexcept override;
};
