#include "SaturationPPLayer.hpp"
#include <model/Buffer.hpp>
#include <model/VertexArray.hpp>
#include <texture/FrameBuffer.hpp>
#include <shader/TextureUploader.hpp>
#include <shader/Shader.hpp>
#include <shader/ShaderProgram.hpp>
#include <system/SystemInterface.hpp>
#include <system/Window.hpp>
#include <events/WindowEvent.hpp>
#include <shader/Uniform.common.hpp>
#include <EnumBitFields.hpp>
#include "ControlEvent.hpp"

SaturationPPLayer::SaturationPPLayer(Globals& globals, float saturation) noexcept
    : _globals(globals)
    , _saturation(saturation)
    , _shader(null)
    , _va(null)
    , _frameBuffer(null)
    , _textureSampler(null)
    , _fbUploader(null)
    , _saturationUniform(globals.gi.createBuffer())
{ _saturationUniform.set(_globals.rc, _saturation); }

bool SaturationPPLayer::init() noexcept
{
    IShader* vaShader;

    {
        ShaderArgs args;
        args.vfsMount = "|TERes";

        args.path = "/shader/FrameBuffer/";
        args.fileName = "Vertex";
        args.stage = EShader::Stage::Vertex;

        IShaderBuilder::Error error;
        const CPPRef<IShader> vertexShader = _globals.gi.createShader().buildCPPRef(args, &error);

        if(error != IShaderBuilder::Error::NoError)
        {
            switch(error)
            {
                case IShaderBuilder::Error::CompileError:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to compile saturation post processing vertex shader.");
                    break;
                case IShaderBuilder::Error::InvalidFile:
                    SystemInterface::createAlert("Non-Critical Error", "Invalid vertex shader file for saturation post processing.");
                    break;
                case IShaderBuilder::Error::InvalidInclude:
                    SystemInterface::createAlert("Non-Critical Error", "Invalid include in vertex shader file for saturation post processing.");
                    break;
                case IShaderBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for vertex shader in saturation post processing.");
                    break;
                case IShaderBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for vertex shader in saturation post processing.");
                    break;
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating vertex shader for saturation post processing.");
                    break;
            }
            return false;
        }

        args.path = "/shader/FrameBuffer/Saturation/";
        args.fileName = "Pixel";
        args.stage = EShader::Stage::Pixel;
        const CPPRef<IShader> pixelShader = _globals.gi.createShader().buildCPPRef(args, null);

        if(error != IShaderBuilder::Error::NoError)
        {
            switch(error)
            {
                case IShaderBuilder::Error::CompileError:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to compile saturation post processing pixel shader.");
                    break;
                case IShaderBuilder::Error::InvalidFile:
                    SystemInterface::createAlert("Non-Critical Error", "Invalid pixel shader file for saturation post processing.");
                    break;
                case IShaderBuilder::Error::InvalidInclude:
                    SystemInterface::createAlert("Non-Critical Error", "Invalid include in pixel shader file for saturation post processing.");
                    break;
                case IShaderBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for pixel shader in saturation post processing.");
                    break;
                case IShaderBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for pixel shader in saturation post processing.");
                    break;
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating pixel shader for saturation post processing.");
                    break;
            }
            return false;
        }

        _shader = IShaderProgram::create(_globals.gi);
        _shader->setVertexShader(_globals.rc, vertexShader);
        _shader->setPixelShader(_globals.rc, pixelShader);

        vaShader = vertexShader.get();
    }

    {
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        VertexBufferArgs posArgs(2, false);
        posArgs.usage = EBuffer::UsageType::StaticDraw;
        posArgs.elementCount = 6;
        posArgs.initialBuffer = quadVertices;
        posArgs.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector2Float);
        posArgs.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

        IBufferBuilder::Error bufferError;
        const NullableRef<IVertexBuffer> positions = _globals.gi.createBuffer().buildTauRef(posArgs, &bufferError);

        if(bufferError != IBufferBuilder::Error::NoError)
        {
            switch(bufferError)
            {
                case IBufferBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for vertex buffer in saturation post processing.");
                    break;
                case IBufferBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for vertex buffer in saturation post processing.");
                    break;
                case IBufferBuilder::Error::UnknownError:
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating vertex buffer for saturation post processing.");
                    break;
            }
            return false;
        }

        VertexArrayArgs vaArgs(1);
        vaArgs.buffers[0] = positions;
        vaArgs.drawCount = 6;
        vaArgs.drawType = DrawType::SeparatedTriangles;

        IVertexArrayBuilder::Error vaError;
        _va = _globals.gi.createVertexArray().buildTauRef(vaArgs, &vaError);

        if(vaError != IVertexArrayBuilder::Error::NoError)
        {
            switch(vaError)
            {
                case IVertexArrayBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for vertex array in saturation post processing.");
                    break;
                case IVertexArrayBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for vertex array in saturation post processing.");
                    break;
                case IVertexArrayBuilder::Error::InternalError:
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating vertex array for saturation post processing.");
                    break;
            }
            return false;
        }
    }

    {
        Texture2DArgs tArgs;
        tArgs.width = _globals.window.width();
        tArgs.height = _globals.window.height();
        tArgs.mipmapLevels = 1;
        tArgs.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
        tArgs.flags = ETexture::BindFlags::RenderTarget | ETexture::BindFlags::ShaderAccess;
        tArgs.initialBuffer = null;

        ITextureBuilder::Error error;
        const CPPRef<ITexture2D> colorTexture = _globals.gi.createTexture().buildCPPRef(tArgs, &error);

        if(error != ITextureBuilder::Error::NoError)
        {
            switch(error)
            {
                case ITextureBuilder::Error::WidthIsZero:
                    SystemInterface::createAlert("Non-Critical Error", "Width is 0 for color texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::HeightIsZero:
                    SystemInterface::createAlert("Non-Critical Error", "Height is 0 for color texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::DataFormatIsInvalid:
                    SystemInterface::createAlert("Non-Critical Error", "Data format is invalid for color texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for color texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for color texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::InternalError:
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating color texture for saturation post processing.");
                    break;
            }
            return false;
        }

        TextureDepthStencilArgs tdsArgs;
        tdsArgs.width = _globals.window.width();
        tdsArgs.height = _globals.window.height();
        tdsArgs.flags = ETexture::DepthStencilBindFlags::RenderTarget | ETexture::DepthStencilBindFlags::DepthShaderAccess;
        tdsArgs.initialBuffer = null;

        const CPPRef<ITextureDepthStencil> dsTexture = _globals.gi.createTexture().buildCPPRef(tdsArgs, &error);

        if(error != ITextureBuilder::Error::NoError)
        {
            switch(error)
            {
                case ITextureBuilder::Error::WidthIsZero:
                    SystemInterface::createAlert("Non-Critical Error", "Width is 0 for depth texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::HeightIsZero:
                    SystemInterface::createAlert("Non-Critical Error", "Height is 0 for depth texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::DataFormatIsInvalid:
                    SystemInterface::createAlert("Non-Critical Error", "Data format is invalid for depth texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for depth texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for depth texture in saturation post processing.");
                    break;
                case ITextureBuilder::Error::InternalError:
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating depth texture for saturation post processing.");
                    break;
            }
            return false;
        }

        FrameBufferArgs fbArgs(1);
        fbArgs.colorAttachments[0] = colorTexture;
        fbArgs.depthStencilAttachment = dsTexture;

        IFrameBufferBuilder::Error fbError;
        _frameBuffer = _globals.gi.createFrameBuffer().buildTauRef(fbArgs, &fbError);

        if(fbError != IFrameBufferBuilder::Error::NoError)
        {
            switch(fbError)
            {
                case IFrameBufferBuilder::Error::BufferSizeDoesNotMatch:
                    SystemInterface::createAlert("Non-Critical Error", "Buffer sizes do not match for framebuffer in saturation post processing.");
                    break;
                case IFrameBufferBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for framebuffer in saturation post processing.");
                    break;
                case IFrameBufferBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for framebuffer in saturation post processing.");
                    break;
                case IFrameBufferBuilder::Error::CrossAPIFailure:
                case IFrameBufferBuilder::Error::InternalError:
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating framebuffer for saturation post processing.");
                    break;
            }
            return false;
        }

        TextureSamplerArgs tsArgs;
        tsArgs.magFilter() = ETexture::Filter::Linear;
        tsArgs.minFilter() = ETexture::Filter::Linear;
        tsArgs.mipFilter() = ETexture::Filter::Linear;
        tsArgs.wrapU = ETexture::WrapMode::Repeat;
        tsArgs.wrapV = ETexture::WrapMode::Repeat;
        tsArgs.wrapW = ETexture::WrapMode::Repeat;
        tsArgs.depthCompareFunc = ETexture::CompareFunc::Never;

        ITextureSamplerBuilder::Error samplerError;
        _textureSampler = _globals.gi.createTextureSampler().buildCPPRef(tsArgs, &samplerError);

        if(samplerError != ITextureSamplerBuilder::Error::NoError)
        {
            switch(samplerError)
            {
                case ITextureSamplerBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for texture sampler in saturation post processing.");
                    break;
                case ITextureSamplerBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for texture sampler in saturation post processing.");
                    break;
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating texture sampler for saturation post processing.");
                    break;
            }
            return false;
        }

        TextureUploaderArgs fbTuArgs(2);
        fbTuArgs.textures[0] = colorTexture->textureView();
        fbTuArgs.textures[1] = dsTexture->depthView();
        fbTuArgs.textureSampler = _textureSampler;

        ITextureUploaderBuilder::Error uploaderError;
        _fbUploader = _globals.gi.createTextureUploader().buildTauRef(fbTuArgs, &uploaderError);

        if(uploaderError != ITextureUploaderBuilder::Error::NoError)
        {
            switch(uploaderError)
            {
                case ITextureUploaderBuilder::Error::SystemMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for texture uploader in saturation post processing.");
                    break;
                case ITextureUploaderBuilder::Error::DriverMemoryAllocationFailure:
                    SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for texture uploader in saturation post processing.");
                    break;
                case ITextureUploaderBuilder::Error::CrossAPIFailure:
                default:
                    SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating texture uploader for saturation post processing.");
                    break;
            }
            return false;
        }
    }

    return true;
}

void SaturationPPLayer::onEvent(Event& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(this, &SaturationPPLayer::onWindowResize);
    dispatcher.dispatch<ControlEvent>(this, &SaturationPPLayer::onControlEvent);
}

void SaturationPPLayer::onFrameBufferBind() noexcept
{
    _frameBuffer->bind(_globals.rc, IFrameBuffer::AccessMode::Write);
    _frameBuffer->clearFrameBuffer(_globals.rc, true, true, false, { 0, 0, 0, 255 });
}

void SaturationPPLayer::onFrameBufferUnbind() noexcept
{
    _frameBuffer->unbind(_globals.rc);
}

void SaturationPPLayer::onRender() noexcept
{
    _shader->bind(_globals.rc);
    _saturationUniform.upload(_globals.rc, EShader::Stage::Pixel, 0);
    _fbUploader->upload(_globals.rc, TextureIndices(0, 0, 0), EShader::Stage::Pixel);

    _va->bind(_globals.rc);
    _va->draw(_globals.rc);
    _va->unbind(_globals.rc);

    _fbUploader->unbind(_globals.rc, TextureIndices(0, 0, 0), EShader::Stage::Pixel);
    _saturationUniform.unbind(_globals.rc, EShader::Stage::Pixel, 0);
    _shader->unbind(_globals.rc);
}

bool SaturationPPLayer::onWindowResize(WindowResizeEvent& e) noexcept
{
    Texture2DArgs tArgs;
    tArgs.width = e.newWidth();
    tArgs.height = e.newHeight();
    tArgs.mipmapLevels = 1;
    tArgs.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    tArgs.flags = ETexture::BindFlags::RenderTarget | ETexture::BindFlags::ShaderAccess;
    tArgs.initialBuffer = null;

    ITextureBuilder::Error error;
    const CPPRef<ITexture2D> colorTexture = _globals.gi.createTexture().buildCPPRef(tArgs, &error);

    if(error != ITextureBuilder::Error::NoError)
    {
        switch(error)
        {
            case ITextureBuilder::Error::WidthIsZero:
                SystemInterface::createAlert("Non-Critical Error", "Width is 0 for color texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::HeightIsZero:
                SystemInterface::createAlert("Non-Critical Error", "Height is 0 for color texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::DataFormatIsInvalid:
                SystemInterface::createAlert("Non-Critical Error", "Data format is invalid for color texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::SystemMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for color texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::DriverMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for color texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::InternalError:
            default:
                SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating color texture for saturation post processing.");
                break;
        }
        return false;
    }

    TextureDepthStencilArgs tdsArgs;
    tdsArgs.width = e.newWidth();
    tdsArgs.height = e.newHeight();
    tdsArgs.flags = ETexture::DepthStencilBindFlags::RenderTarget | ETexture::DepthStencilBindFlags::DepthShaderAccess;
    tdsArgs.initialBuffer = null;

    const CPPRef<ITextureDepthStencil> dsTexture = _globals.gi.createTexture().buildCPPRef(tdsArgs, &error);

    if(error != ITextureBuilder::Error::NoError)
    {
        switch(error)
        {
            case ITextureBuilder::Error::WidthIsZero:
                SystemInterface::createAlert("Non-Critical Error", "Width is 0 for depth texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::HeightIsZero:
                SystemInterface::createAlert("Non-Critical Error", "Height is 0 for depth texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::DataFormatIsInvalid:
                SystemInterface::createAlert("Non-Critical Error", "Data format is invalid for depth texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::SystemMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for depth texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::DriverMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for depth texture in saturation post processing.");
                break;
            case ITextureBuilder::Error::InternalError:
            default:
                SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating depth texture for saturation post processing.");
                break;
        }
        return false;
    }

    FrameBufferArgs fbArgs(1);
    fbArgs.colorAttachments[0] = colorTexture;
    fbArgs.depthStencilAttachment = dsTexture;

    IFrameBufferBuilder::Error fbError;
    _frameBuffer = _globals.gi.createFrameBuffer().buildTauRef(fbArgs, &fbError);

    if(fbError != IFrameBufferBuilder::Error::NoError)
    {
        switch(fbError)
        {
            case IFrameBufferBuilder::Error::BufferSizeDoesNotMatch:
                SystemInterface::createAlert("Non-Critical Error", "Buffer sizes do not match for framebuffer in saturation post processing.");
                break;
            case IFrameBufferBuilder::Error::SystemMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for framebuffer in saturation post processing.");
                break;
            case IFrameBufferBuilder::Error::DriverMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for framebuffer in saturation post processing.");
                break;
            case IFrameBufferBuilder::Error::CrossAPIFailure:
            case IFrameBufferBuilder::Error::InternalError:
            default:
                SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating framebuffer for saturation post processing.");
                break;
        }
        return false;
    }

    TextureUploaderArgs fbTuArgs(2);
    fbTuArgs.textures[0] = colorTexture->textureView();
    fbTuArgs.textures[1] = dsTexture->depthView();
    fbTuArgs.textureSampler = _textureSampler;

    ITextureUploaderBuilder::Error uploaderError;
    _fbUploader = _globals.gi.createTextureUploader().buildTauRef(fbTuArgs, &uploaderError);

    if(uploaderError != ITextureUploaderBuilder::Error::NoError)
    {
        switch(uploaderError)
        {
            case ITextureUploaderBuilder::Error::SystemMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Failed to allocate memory for texture uploader in saturation post processing.");
                break;
            case ITextureUploaderBuilder::Error::DriverMemoryAllocationFailure:
                SystemInterface::createAlert("Non-Critical Error", "Driver failed to allocate memory for texture uploader in saturation post processing.");
                break;
            case ITextureUploaderBuilder::Error::CrossAPIFailure:
            default:
                SystemInterface::createAlert("Non-Critical Error", "Unknown error when creating texture uploader for saturation post processing.");
                break;
        }
        return false;
    }

    return false;
}

bool SaturationPPLayer::onControlEvent(ControlEvent& e) noexcept
{
    if(e.type() == CE_SET_SATURATION)
    {
        _saturation = *reinterpret_cast<float*>(e.arg());
        _saturationUniform.set(_globals.rc, _saturation);
    }

    return false;
}
