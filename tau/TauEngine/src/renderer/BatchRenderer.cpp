#include "renderer/BatchRenderer.hpp"
#include "graphics/Resource.hpp"
#include "system/GraphicsInterface.hpp"
#include "graphics/CommandList.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/PipelineState.hpp"
#include "graphics/DescriptorLayout.hpp"
#include "graphics/BlendingState.hpp"
#include "graphics/DepthStencilState.hpp"
#include "graphics/RasterizerState.hpp"
#include "graphics/InputLayout.hpp"
#include "graphics/DescriptorHeap.hpp"
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <MathDefines.hpp>

#include "texture/TextureView.hpp"

#pragma push(pack, 1)
struct InterleavedVertex final
{
    glm::vec3 position;
    glm::vec4 color;
};
#pragma pop(pack)

static_assert(sizeof(InterleavedVertex) == sizeof(float) * 7, "glm::vec3 is not 3 floats");

struct RendererData final
{
    glm::vec3 equilateralTriangleVertexPositions[3];
    glm::vec4 quadVertexPositions[4];
    glm::vec4 hexagonVertexPositions[7];
    
    u16 quadIndices[6];
    u16 hexagonIndices[18];
    IGraphicsInterface* gi;

    NullableRef<IResource> equilateralTriangleVertexBuffer;
    NullableRef<IResource> quadTextureBuffer;
    NullableRef<IResource> quadHexagonVertexBuffer;
    NullableRef<IResource> quadHexagonIndexBuffer;

    NullableRef<IVertexArray> equilateralTriangleVertexArray;
    NullableRef<IVertexArray> quadHexagonVertexArray;

    static constexpr uSys MaxTriangles = 65536;
    static constexpr uSys MaxQuads = 16384; // 65536 / 4
    static constexpr uSys MaxHexagons = 9362; // ~ 65536 / 7

    static constexpr uSys MaxTriangleVertices = MaxTriangles * 3;
    static constexpr uSys MaxQuadVertices = MaxQuads * 4;
    static constexpr uSys MaxHexagonVertices = MaxHexagons * 7;

    static constexpr uSys MaxQuadIndices = MaxQuads * 2 * 3;
    static constexpr uSys MaxHexagonIndices = MaxHexagons * 6 * 3;

    static constexpr uSys MaxQuadTextureIndex = 32;
    static constexpr uSys MaxHexagonTextureIndex = 32;

    uSys currentTriangleCount;
    uSys currentQuadCount;
    uSys currentHexagonCount;

    uSys currentQuadTextureIndex;
    uSys currentHexagonTextureIndex;

    NullableRef<ICommandList> cmdList;
    NullableRef<IPipelineState> pipelineState;
    NullableRef<IDescriptorHeap> quadHexagonTextureDescriptorHeap;

    u8* equilateralTriangleMapping;
    u8* quadHexagonMapping;
};

static RendererData rendererData;

void BatchRenderer::init(IGraphicsInterface& gi, ICommandList& cmdList) noexcept
{
    rendererData.gi = &gi;

    {
        NullableRef<IDescriptorLayout> descriptorLayout;
        NullableRef<IBlendingState> blendingState;
        NullableRef<IDepthStencilState> depthStencilState;
        NullableRef<IRasterizerState> rasterizerState;
        NullableRef<IInputLayout> inputLayout;

        {
            DescriptorLayoutEntry entry;
            entry.type = DescriptorLayoutEntry::Type::TextureView;
            entry.begin = 0;
            entry.count = 32;
            entry.shaderAccess = EGraphics::ShaderAccess::Pixel;

            DescriptorLayoutArgs args;
            args.entryCount = 1;
            args.entries = &entry;

            IDescriptorLayoutBuilder::Error error;
            descriptorLayout = gi.createDescriptorLayout().build(args, &error);
        }

        {
            BlendingArgs args(tau::Recommended);
            IBlendingStateBuilder::Error error;
            blendingState = gi.createBlendingState().buildTauRef(args, &error);
        }

        {
            DepthStencilArgs args(tau::Recommended);
            IDepthStencilStateBuilder::Error error;
            depthStencilState = gi.createDepthStencilState().buildTauRef(args, &error);
        }

        {
            RasterizerArgs args(tau::Recommended);
            IRasterizerStateBuilder::Error error;
            rasterizerState = gi.createRasterizerState().buildTauRef(args, &error);
        }

        {
            BufferDescriptorBuilder descriptorBuilder(1, false);
            descriptorBuilder.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector3Float);

            BufferDescriptor descriptor = descriptorBuilder.build();

            InputLayoutArgs args;
            args.shader = nullptr;
            args.descriptorCount = 1;
            args.descriptors = &descriptor;

            IInputLayoutBuilder::Error error;
            inputLayout = gi.createInputLayout().buildTauRef(args, &error);
        }

        PipelineArgs args;
        args.descriptorLayout = descriptorLayout;
        args.blendingState = blendingState;
        args.depthStencilState = depthStencilState;
        args.rasterizerState = rasterizerState;
        args.inputLayout = inputLayout;
        args.numRenderTargets = 1;
    }

    {
        DescriptorHeapArgs args;
        args.type = EGraphics::DescriptorType::TextureView;
        args.numDescriptors = 32 + 32;
        args.flags = DHF_ShaderAccess;

        IDescriptorHeapBuilder::Error error;
        rendererData.quadHexagonTextureDescriptorHeap = gi.createDescriptorHeap().buildTauRef(args, &error);
    }

    {
        rendererData.equilateralTriangleVertexPositions[0] = glm::vec3(0.0f, 1.0f, 1.0f); // (0, 1)
        rendererData.equilateralTriangleVertexPositions[1] = glm::vec3(-T_SQRT_3 / 2.0f, -0.5f, 1.0f); // (-sqrt(3)/2, -0.5)
        rendererData.equilateralTriangleVertexPositions[2] = glm::vec3(T_SQRT_3 / 2.0f, -0.5f, 1.0f); // (sqrt(3), -0.5)

        rendererData.quadVertexPositions[0] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // (0, 0)
        rendererData.quadVertexPositions[1] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); // (0, 1)
        rendererData.quadVertexPositions[2] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); // (1, 0)
        rendererData.quadVertexPositions[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // (1, 1)
        
        rendererData.hexagonVertexPositions[0] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // (0, 0)
        rendererData.hexagonVertexPositions[1] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f); // (0, 1)
        rendererData.hexagonVertexPositions[2] = glm::vec4(-T_SQRT_3 / 2.0f, 0.5f, 1.0f, 1.0f); // (-sqrt(3)/2, 0.5)
        rendererData.hexagonVertexPositions[3] = glm::vec4(-T_SQRT_3 / 2.0f, -0.5f, 1.0f, 1.0f); // (-sqrt(3)/2, -0.5)
        rendererData.hexagonVertexPositions[4] = glm::vec4(0.0f, -1.0f, 1.0f, 1.0f); // (0, -1)
        rendererData.hexagonVertexPositions[5] = glm::vec4(T_SQRT_3 / 2.0f, -0.5f, 1.0f, 1.0f); // (sqrt(3), -0.5)
        rendererData.hexagonVertexPositions[6] = glm::vec4(T_SQRT_3 / 2.0f, 0.5f, 1.0f, 1.0f); // (sqrt(3), 0.5)

        rendererData.quadIndices[0] = 0;
        rendererData.quadIndices[1] = 2;
        rendererData.quadIndices[2] = 1;
        rendererData.quadIndices[3] = 3;
        rendererData.quadIndices[4] = 1;
        rendererData.quadIndices[5] = 2;

        rendererData.hexagonIndices[0] = 0;
        rendererData.hexagonIndices[1] = 1;
        rendererData.hexagonIndices[2] = 2;

        rendererData.hexagonIndices[3] = 0;
        rendererData.hexagonIndices[4] = 2;
        rendererData.hexagonIndices[5] = 3;

        rendererData.hexagonIndices[6] = 0;
        rendererData.hexagonIndices[7] = 3;
        rendererData.hexagonIndices[8] = 4;

        rendererData.hexagonIndices[9]  = 0;
        rendererData.hexagonIndices[10] = 4;
        rendererData.hexagonIndices[11] = 5;

        rendererData.hexagonIndices[12] = 0;
        rendererData.hexagonIndices[13] = 5;
        rendererData.hexagonIndices[14] = 6;

        rendererData.hexagonIndices[15] = 0;
        rendererData.hexagonIndices[16] = 6;
        rendererData.hexagonIndices[17] = 1;
    }

    {
        static constexpr uSys Vec2Size = 2 * sizeof(float);
        static constexpr uSys Vec3Size = 3 * sizeof(float);
        static constexpr uSys Vec4Size = 4 * sizeof(float);

        IResourceBuilder::Error error;

        ResourceBufferArgs args;
        args.size = RendererData::MaxTriangleVertices * Vec3Size + RendererData::MaxTriangleVertices * Vec4Size;
        args.bufferType = EBuffer::Type::Vertex;
        args.usageType = EResource::UsageType::Streaming;
        args.initialBuffer = nullptr;

        {
            rendererData.equilateralTriangleVertexBuffer = gi.createResource().buildTauRef(args, nullptr, &error);
        }

        {
            args.size = (RendererData::MaxQuadVertices * Vec3Size + RendererData::MaxQuadVertices * Vec4Size) +
                (RendererData::MaxHexagonVertices * Vec3Size + RendererData::MaxHexagonVertices * Vec4Size);

            rendererData.quadHexagonVertexBuffer = gi.createResource().buildTauRef(args, nullptr, &error);
        }

        {
            args.size = RendererData::MaxQuadVertices * Vec2Size;
            args.usageType = EResource::UsageType::Default;

            rendererData.quadTextureBuffer = gi.createResource().buildTauRef(args, nullptr, &error);
        }

        {
            args.size = RendererData::MaxQuadIndices * sizeof(u16) + RendererData::MaxHexagonIndices * sizeof(u16);
            args.bufferType = EBuffer::Type::Index;

            rendererData.quadHexagonIndexBuffer = gi.createResource().buildTauRef(args, nullptr, &error);
        }
    }

    {
        BufferDescriptorBuilder descriptorBuilder(2, false);
        descriptorBuilder.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector3Float);
        descriptorBuilder.addDescriptor(ShaderSemantic::Color, ShaderDataType::Vector4Float);

        VertexBufferView equilateralTriangleView(rendererData.equilateralTriangleVertexBuffer, descriptorBuilder.build());

        VertexBufferView quadView[2];

        quadView[0] = VertexBufferView(rendererData.quadHexagonVertexBuffer, descriptorBuilder.build());

        descriptorBuilder.reset(1, false);
        descriptorBuilder.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

        quadView[1] = VertexBufferView(rendererData.quadTextureBuffer, descriptorBuilder.build());



        VertexArrayArgs args;
        args.bufferCount = 1;
        args.bufferViews = &equilateralTriangleView;

        IVertexArrayBuilder::Error error;
        rendererData.equilateralTriangleVertexArray = gi.createVertexArray().buildTauRef(args, &error);

        args.bufferCount = 2;
        args.bufferViews = quadView;

        rendererData.quadHexagonVertexArray = gi.createVertexArray().buildTauRef(args, &error);
    }

    {
        float* const texturesMapping = reinterpret_cast<float*>(rendererData.quadTextureBuffer->map(cmdList, EResource::MapType::Discard));

        float quadTextures[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };

        for(uSys i = 0; i < RendererData::MaxQuads; ++i)
        {
            ::std::memcpy(texturesMapping + i * 4, quadTextures, sizeof(quadTextures));
        }

        rendererData.quadHexagonIndexBuffer->unmap(cmdList);
    }

    {
        u16* const indicesMapping = reinterpret_cast<u16*>(rendererData.quadHexagonIndexBuffer->map(cmdList, EResource::MapType::Discard));

        for(uSys i = 0; i < RendererData::MaxQuadIndices; ++i)
        {
            const uSys writeOffset = i * 6;
            const u16 indiceOffset = static_cast<u16>(indiceOffset);

            indicesMapping[writeOffset + 0] = 0 + indiceOffset;
            indicesMapping[writeOffset + 1] = 2 + indiceOffset;
            indicesMapping[writeOffset + 2] = 1 + indiceOffset;
            indicesMapping[writeOffset + 3] = 3 + indiceOffset;
            indicesMapping[writeOffset + 4] = 1 + indiceOffset;
            indicesMapping[writeOffset + 5] = 2 + indiceOffset;
        }

        for(uSys i = 0; i < RendererData::MaxHexagonIndices; ++i)
        {
            const uSys writeOffset = i * 18 + RendererData::MaxQuadIndices;
            const u16 indiceOffset = static_cast<u16>(i * 18);

            indicesMapping[writeOffset + 0] = indiceOffset + 0;
            indicesMapping[writeOffset + 1] = indiceOffset + 1;
            indicesMapping[writeOffset + 2] = indiceOffset + 2;
                           
            indicesMapping[writeOffset + 3] = indiceOffset + 0;
            indicesMapping[writeOffset + 4] = indiceOffset + 2;
            indicesMapping[writeOffset + 5] = indiceOffset + 3;
            
            indicesMapping[writeOffset + 6] = indiceOffset + 0;
            indicesMapping[writeOffset + 7] = indiceOffset + 3;
            indicesMapping[writeOffset + 8] = indiceOffset + 4;
                           
            indicesMapping[writeOffset + 9]  = indiceOffset + 0;
            indicesMapping[writeOffset + 10] = indiceOffset + 4;
            indicesMapping[writeOffset + 11] = indiceOffset + 5;
            
            indicesMapping[writeOffset + 12] = indiceOffset + 0;
            indicesMapping[writeOffset + 13] = indiceOffset + 5;
            indicesMapping[writeOffset + 14] = indiceOffset + 6;
            
            indicesMapping[writeOffset + 15] = indiceOffset + 0;
            indicesMapping[writeOffset + 16] = indiceOffset + 6;
            indicesMapping[writeOffset + 17] = indiceOffset + 1;
        }

        
        rendererData.quadHexagonIndexBuffer->unmap(cmdList);
    }
}

void BatchRenderer::flush() noexcept
{
    rendererData.equilateralTriangleVertexBuffer->unmap(*rendererData.cmdList.get());
    rendererData.quadHexagonVertexBuffer->unmap(*rendererData.cmdList.get());

    if(rendererData.currentTriangleCount > 0)
    {
        rendererData.cmdList->setVertexArray(rendererData.equilateralTriangleVertexArray);
        rendererData.cmdList->draw(rendererData.currentTriangleCount * 3, 0);
    }

    if(rendererData.currentQuadCount > 0 || 
       rendererData.currentHexagonCount > 0)
    {
        rendererData.cmdList->setVertexArray(rendererData.quadHexagonVertexArray);
    }

    if(rendererData.currentQuadCount > 0)
    {
        rendererData.cmdList->drawIndexed(rendererData.currentQuadCount * 2 * 3, 0, 0);
    }

    if(rendererData.currentHexagonCount > 0)
    {
        rendererData.cmdList->drawIndexed(rendererData.currentHexagonCount * 6 * 3, RendererData::MaxQuadIndices, RendererData::MaxQuadVertices);
    }
}

void BatchRenderer::beginBatch(NullableRef<ICommandList> cmdList)
{
    rendererData.cmdList = cmdList;

    rendererData.equilateralTriangleMapping = reinterpret_cast<u8*>(rendererData.equilateralTriangleVertexBuffer->map(*cmdList.get(), EResource::MapType::Discard));
    rendererData.quadHexagonMapping = reinterpret_cast<u8*>(rendererData.quadHexagonVertexBuffer->map(*cmdList.get(), EResource::MapType::Discard));

    rendererData.currentTriangleCount = 0;
    rendererData.currentQuadCount = 0;
    rendererData.currentHexagonCount = 0;

    cmdList->setDrawType(EGraphics::DrawType::Triangles);
    cmdList->setPipelineState(rendererData.cmdList);
    cmdList->setBlendFactor(nullptr);
    cmdList->setStencilRef(0);
    
    const IndexBufferView indexBufferView(rendererData.quadHexagonIndexBuffer, EBuffer::IndexSize::Uint16);
    rendererData.cmdList->setIndexBuffer(indexBufferView);
}

void BatchRenderer::endBatch()
{
    flush();
    rendererData.cmdList = nullptr;
}

void BatchRenderer::drawEquilateralTriangle(const glm::vec2 centerPosition, const float radius, const RGBAColor color) noexcept
{ drawEquilateralTriangle(glm::vec3(centerPosition.x, 0.0f, centerPosition.y), radius, color); }

void BatchRenderer::drawEquilateralTriangle(const glm::vec3 centerPosition, const float radius, const RGBAColor color) noexcept
{
    InterleavedVertex vertices[3];

    vertices[0].position = rendererData.equilateralTriangleVertexPositions[0] * radius + centerPosition;
    vertices[1].position = rendererData.equilateralTriangleVertexPositions[1] * radius + centerPosition;
    vertices[2].position = rendererData.equilateralTriangleVertexPositions[2] * radius + centerPosition;

    const glm::vec4 color4 = glm::vec4(static_cast<float>(color.r) / 255.0f,
                                       static_cast<float>(color.g) / 255.0f,
                                       static_cast<float>(color.b) / 255.0f, 
                                       static_cast<float>(color.a) / 255.0f);

    vertices[0].color = color4;
    vertices[1].color = color4;
    vertices[2].color = color4;

    const uSys writeIndex = rendererData.currentTriangleCount++ * sizeof(float) * (3 + 4);

    (void) ::std::memcpy(rendererData.equilateralTriangleMapping + writeIndex, vertices, sizeof(vertices));
}

void BatchRenderer::drawEquilateralTriangle(glm::vec2 centerPosition, const float radius, const float rotation, const RGBAColor color) noexcept
{ drawEquilateralTriangle(glm::vec3(centerPosition.x, 0.0f, centerPosition.y), radius, rotation, color); }

void BatchRenderer::drawEquilateralTriangle(glm::vec3 centerPosition, const float radius, const float rotation, const RGBAColor color) noexcept
{
    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerPosition) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 1.0f, 0.0f }) *
        glm::scale(glm::mat4(1.0f), glm::vec3(radius, radius, 1.0f));

    InterleavedVertex vertices[3];

    vertices[0].position = transform * glm::vec4(rendererData.equilateralTriangleVertexPositions[0], 1.0f);
    vertices[1].position = transform * glm::vec4(rendererData.equilateralTriangleVertexPositions[1], 1.0f);
    vertices[2].position = transform * glm::vec4(rendererData.equilateralTriangleVertexPositions[2], 1.0f);

    const glm::vec4 color4 = glm::vec4(static_cast<float>(color.r) / 255.0f,
                                       static_cast<float>(color.g) / 255.0f,
                                       static_cast<float>(color.b) / 255.0f, 
                                       static_cast<float>(color.a) / 255.0f);

    vertices[0].color = color4;
    vertices[1].color = color4;
    vertices[2].color = color4;

    const uSys writeIndex = rendererData.currentTriangleCount++ * sizeof(float) * (3 + 4);

    (void) ::std::memcpy(rendererData.equilateralTriangleMapping + writeIndex, vertices, sizeof(vertices));
}

void BatchRenderer::drawQuad(const glm::mat4& transform, const RGBAColor color) noexcept
{
    InterleavedVertex vertices[4];

    vertices[0].position = transform * rendererData.quadVertexPositions[0];
    vertices[1].position = transform * rendererData.quadVertexPositions[1];
    vertices[2].position = transform * rendererData.quadVertexPositions[2];
    vertices[3].position = transform * rendererData.quadVertexPositions[3];

    const glm::vec4 color4 = glm::vec4(static_cast<float>(color.r) / 255.0f,
                                       static_cast<float>(color.g) / 255.0f,
                                       static_cast<float>(color.b) / 255.0f, 
                                       static_cast<float>(color.a) / 255.0f);

    vertices[0].color = color4;
    vertices[1].color = color4;
    vertices[2].color = color4;
    vertices[3].color = color4;

    const uSys writeIndex = rendererData.currentQuadCount++ * sizeof(float) * (3 + 4);

    (void) ::std::memcpy(rendererData.quadHexagonMapping + writeIndex, vertices, sizeof(vertices));
}

void BatchRenderer::drawQuad(const glm::mat4& transform, NullableRef<IResource>& texture, const RGBAColor tint) noexcept
{
    if(texture->resourceType() != EResource::Type::Texture2D)
    { return; }

    if(rendererData.currentQuadTextureIndex >= 31)
    { flush(); }

    InterleavedVertex vertices[4];

    vertices[0].position = transform * rendererData.quadVertexPositions[0];
    vertices[1].position = transform * rendererData.quadVertexPositions[1];
    vertices[2].position = transform * rendererData.quadVertexPositions[2];
    vertices[3].position = transform * rendererData.quadVertexPositions[3];

    const glm::vec4 color4 = glm::vec4(static_cast<float>(tint.r) / 255.0f,
                                       static_cast<float>(tint.g) / 255.0f,
                                       static_cast<float>(tint.b) / 255.0f, 
                                       static_cast<float>(tint.a) / 255.0f);

    vertices[0].color = color4;
    vertices[1].color = color4;
    vertices[2].color = color4;
    vertices[3].color = color4;

    const uSys writeIndex = rendererData.currentQuadCount++ * sizeof(float) * (3 + 4);

    (void) ::std::memcpy(rendererData.quadHexagonMapping + writeIndex, vertices, sizeof(vertices));

    TextureViewArgs args;
    args.texture = texture.get();
    args.dataFormat = args.texture->getArgs<ResourceTexture2DArgs>()->dataFormat;
    args.type = ETexture::Type::Texture2D;

    CPUDescriptorHandle handle = rendererData.quadHexagonTextureDescriptorHeap->getBaseCPUHandle();
    handle.offset(rendererData.currentQuadTextureIndex++, rendererData.quadHexagonTextureDescriptorHeap->getOffsetStride());

    ITextureViewBuilder::Error error;
    (void) rendererData.gi->createTextureView().build(args, handle, &error);
}
