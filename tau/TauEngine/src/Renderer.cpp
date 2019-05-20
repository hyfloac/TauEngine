#include <Renderer.hpp>

void Renderer::addProgram(GLProgram* program, std::vector<const GLShader*>& shaders) noexcept
{
    _scene.insert(std::make_pair<const GLProgram*, ShaderBundle>({ program }, { program, shaders, std::unordered_multimap<const RenderableObject*, WorldObject>() }));
}

void Renderer::exchangeShaders(const GLProgram* old, const GLProgram* RESTRICT replacement, std::vector<const GLShader*>& RESTRICT shaders) noexcept
{
    _scene[old].program = replacement;
    _scene[old].shaders = shaders;
}

void Renderer::addObject(const GLProgram* shaderProgram, const RenderableObject* RESTRICT obj, const WorldObject& RESTRICT loc) noexcept
{
    _scene[shaderProgram].models.insert(std::make_pair(obj, loc));
}

void Renderer::addObject(const GLProgram* shaderProgram, const RenderableObject* obj) noexcept
{
    _scene[shaderProgram].models.insert(std::make_pair(obj, WorldObject(obj)));
}

void Renderer::render() const noexcept
{
    for(const auto& modelPair : _scene)
    {
        const GLProgram* program = modelPair.second.program;
        program->activate();

        const std::unordered_multimap<const RenderableObject*, WorldObject> worldObjects = modelPair.second.models;

        for(auto modelObjectPair = worldObjects.begin(); modelObjectPair != worldObjects.end();)
        {
            const RenderableObject* model = modelObjectPair->first;

            model->preRender();

            do
            {
                ++modelObjectPair;

                model->render();
            } while(modelObjectPair != worldObjects.end() && model == modelObjectPair->first);

            RenderableObject::postRender();
        }

        GLProgram::deactivate();
    }
}
