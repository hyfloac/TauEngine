#include "Renderer.hpp"
#include "system/RenderingContext.hpp"

void Renderer::addProgram(GLProgram* program, std::vector<const GLShader*>& shaders) noexcept
{
    _scene.insert(std::make_pair<const GLProgram*, ShaderBundle>({ program }, { program, shaders, std::unordered_multimap<RenderableObject, WorldObject>() }));
}

void Renderer::exchangeShaders(const GLProgram* old, const GLProgram* RESTRICT replacement, std::vector<const GLShader*>& RESTRICT shaders) noexcept
{
    _scene[old].program = replacement;
    _scene[old].shaders = shaders;
}

void Renderer::addObject(const GLProgram* shaderProgram, RenderableObject obj, WorldObject loc) noexcept
{
    _scene[shaderProgram].models.insert(std::make_pair<RenderableObject, WorldObject>(std::move(obj), std::move(loc)));
}

void Renderer::addObject(const GLProgram* shaderProgram, RenderableObject obj) noexcept
{
    _scene[shaderProgram].models.insert(std::make_pair<const RenderableObject&, WorldObject>(std::move(obj), WorldObject(obj)));
}

void Renderer::render(IRenderingContext& context) const noexcept
{
    for(const auto& modelPair : _scene)
    {
        const GLProgram* program = modelPair.second.program;
        program->activate();

        const std::unordered_multimap<RenderableObject, WorldObject> worldObjects = modelPair.second.models;

        for(auto modelObjectPair = worldObjects.begin(); modelObjectPair != worldObjects.end();)
        {
            const RenderableObject& model = modelObjectPair->first;

            model.preRender(context);

            do
            {
                ++modelObjectPair;

                model.render(context);
            } while(modelObjectPair != worldObjects.end() && model == modelObjectPair->first);

            model.postRender(context);
        }

        GLProgram::deactivate();
    }
}
