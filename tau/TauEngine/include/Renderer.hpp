#pragma once

#include <DLL.hpp>
#include <unordered_map>
#include <model/RenderableObject.hpp>
#include <gl/GLProgram.hpp>
#include <WorldObject.hpp>
#include <gl/GLShader.hpp>
#include <vector>
#include <Utils.hpp>

struct ShaderBundle final
{
    const GLProgram* program;
    std::vector<const GLShader*> shaders;
    std::unordered_multimap<RenderableObject, WorldObject> models;
};

class IRenderingContext;

class TAU_DLL Renderer
{
private:
    std::unordered_map<const GLProgram*, ShaderBundle> _scene;
public:
    void addProgram(GLProgram* program, std::vector<const GLShader*>& shaders) noexcept;

    void exchangeShaders(const GLProgram* old, const GLProgram* RESTRICT replacement, std::vector<const GLShader*>& RESTRICT shaders) noexcept;

    void addObject(const GLProgram* shaderProgram, RenderableObject obj, WorldObject loc) noexcept;

    void addObject(const GLProgram* shaderProgram, RenderableObject obj) noexcept;

    void render(IRenderingContext& context) const noexcept;
};
