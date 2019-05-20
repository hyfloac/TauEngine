#pragma once

#include <DLL.hpp>
#include <unordered_map>
#include <model/RenderableObject.hpp>
#include <gl/GLProgram.hpp>
#include <WorldObject.hpp>
#include <shader/GLShader.hpp>
#include <vector>
#include <Utils.hpp>

struct ShaderBundle final
{
    const GLProgram* program;
    std::vector<const GLShader*> shaders;
    std::unordered_multimap<const RenderableObject*, WorldObject> models;
};

class TAU_DLL Renderer
{
private:
    std::unordered_map<const GLProgram*, ShaderBundle> _scene;
public:
    void addProgram(GLProgram* program, std::vector<const GLShader*>& shaders) noexcept;

    void exchangeShaders(const GLProgram* old, const GLProgram* RESTRICT replacement, std::vector<const GLShader*>& RESTRICT shaders) noexcept;

    void addObject(const GLProgram* shaderProgram, const RenderableObject* RESTRICT obj, const WorldObject& RESTRICT loc) noexcept;

    void addObject(const GLProgram* shaderProgram, const RenderableObject* obj) noexcept;

    void render() const noexcept;
};
