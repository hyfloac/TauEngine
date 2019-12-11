#include "camera/Skybox.hpp"
#include "camera/Camera3D.hpp"
#include "system/RenderingContext.hpp"
#include "model/IVertexArray.hpp"
#include "shader/IShader.hpp"
#include "texture/FITextureLoader.hpp"
#include "gl/GLUtils.hpp"

Skybox::Skybox(IRenderingContext& context, const char* vertexShaderPath, const char* pixelShaderPath, const char* skyboxPath, const char* fileExtension) noexcept
    : _shader(IShaderProgram::create(context)),
      _projectionUni(null), _viewUni(null), _skyboxUni(null),
      _skybox(null), _cubeVA(context.createVertexArray(1, DrawType::SeparatedTriangles))
{
    Ref<IShader> vertexShader = IShader::create(context, IShader::Type::Vertex, vertexShaderPath);
    Ref<IShader> pixelShader = IShader::create(context, IShader::Type::Pixel, pixelShaderPath);

    vertexShader->loadShader();
    pixelShader->loadShader();

    _shader->setVertexShader(context, vertexShader);
    _shader->setPixelShader(context, pixelShader);

    _shader->link(context);

    _projectionUni = _shader->getUniform<glm::mat4>("projectionMatrix", false);
    _viewUni = _shader->getUniform<glm::mat4>("cameraViewMatrix", false);
    _skyboxUni = _shader->getUniform<int>("skybox");

    const TextureLoader::GPUTextureSettings settings
    {
        -1,
        ETexture::Filter::Linear,
        ETexture::Filter::Linear,
        ETexture::WrapMode::ClampToEdge,
        ETexture::WrapMode::ClampToEdge,
        ETexture::WrapMode::ClampToEdge
    };
    _skybox = Ref<ITextureCube>(TextureLoader::loadTextureCubeEx(context, skyboxPath, fileExtension, settings));

    Ref<IBuffer> skyboxCube = context.createBuffer(1, IBuffer::Type::ArrayBuffer);

    float skyboxVertices[] = {
        // back
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        // left
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        // right
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        // front
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        // top
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        // bottom
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    skyboxCube->bind(context);
    skyboxCube->fillBuffer(context, sizeof(skyboxVertices), skyboxVertices);
    skyboxCube->descriptor().addDescriptor(ShaderDataType::Type::Vector3Float);
    skyboxCube->unbind(context);

    _cubeVA->addVertexBuffer(context, skyboxCube);
    _cubeVA->drawCount() = 36;
    // _cubeVA->drawCount() = 6;
}

void Skybox::render(IRenderingContext& context, const Camera3D& camera) const noexcept
{
    context.enableDepthWriting(false);
    context.setFaceWinding(false);

    glDepthFunc(GL_LEQUAL);

    _shader->bind(context);
    _projectionUni->set(camera.projectionMatrix());
    _viewUni->set(camera.viewRotMatrix());
    _skyboxUni->set(0);

    _skybox->bind(0);

    _cubeVA->bind(context);
    _cubeVA->preDraw(context);
    _cubeVA->draw(context);
    _cubeVA->postDraw(context);
    _cubeVA->unbind(context);

    _skybox->unbind(0);
    _shader->unbind(context);

    glDepthFunc(GL_LESS);

    context.enableDepthWriting(true);
}
