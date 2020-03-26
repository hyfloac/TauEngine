#include "terrain/TerrainTile.hpp"
#include "model/Buffer.hpp"
#include "system/RenderingContext.hpp"

CPPRef<IVertexArray> TerrainTile::generateTerrain(IRenderingContext& context, const float size, const uSys edgeVertices) noexcept
{
    const uSys numVertices = edgeVertices * edgeVertices;
    const uSys numIndices = (edgeVertices - 1) * (edgeVertices - 1);
    const float step = size / static_cast<float>(edgeVertices - 1);

    float* pos = new float[3 * numVertices];
    float* norm = new float[3 * numVertices];
    float* tex = new float[2 * numVertices];
    u32* indices = new u32[6 * numIndices];

    uSys ind3 = 0;
    uSys ind2 = 0;

    for(uSys y = 0; y < edgeVertices; ++y)
    {
        for(uSys x = 0; x < edgeVertices; ++x)
        {
            pos[ind3 + 0] = x * step;
            pos[ind3 + 1] = 0.0f;
            pos[ind3 + 2] = y * step;

            norm[ind3 + 0] = 0.0f;
            norm[ind3 + 1] = 1.0f;
            norm[ind3 + 2] = 0.0f;

            tex[ind2 + 0] = static_cast<float>(x) / static_cast<float>(edgeVertices - 1);
            tex[ind2 + 1] = static_cast<float>(y) / static_cast<float>(edgeVertices - 1);

            ind3 += 3;
            ind2 += 2;
        }
    }

    uSys ind = 0;

    for(uSys y = 0; y < edgeVertices - 1; ++y)
    {
        for(uSys x = 0; x < edgeVertices - 1; ++x)
        {
            const uSys topLeft = y * edgeVertices + x;
            const uSys topRight = topLeft + 1;
            const uSys bottomLeft = (y + 1) * edgeVertices + x;
            const uSys bottomRight = bottomLeft + 1;

            indices[ind++] = topLeft;
            indices[ind++] = bottomLeft;
            indices[ind++] = topRight;
            indices[ind++] = topRight;
            indices[ind++] = bottomLeft;
            indices[ind++] = bottomRight;
        }
    }

    BufferArgs buf3Builder(1);
    BufferArgs buf2Builder(1);
    IndexBufferArgs indiceBuilder;
    // CPPRef<IIndexBufferBuilder> indiceBuilder = context.createIndexBuffer();

    buf3Builder.type = EBuffer::Type::ArrayBuffer;
    buf3Builder.usage = EBuffer::UsageType::StaticDraw;
    buf3Builder.elementCount = numVertices;
    buf3Builder.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector3Float);

    buf2Builder.type = EBuffer::Type::ArrayBuffer;
    buf2Builder.usage = EBuffer::UsageType::StaticDraw;
    buf2Builder.elementCount = numVertices;
    buf2Builder.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

    indiceBuilder.usage = EBuffer::UsageType::StaticDraw;
    indiceBuilder.elementCount = 6 * numIndices;

    // CPPRef<IBuffer> posBuf = CPPRef<IBuffer>(buf3Builder->build(nullptr));
    // CPPRef<IBuffer> normBuf = CPPRef<IBuffer>(buf3Builder->build(nullptr));
    // CPPRef<IBuffer> texBuf = CPPRef<IBuffer>(buf2Builder->build(nullptr));
    CPPRef<IBuffer> posBuf = context.createBuffer().buildCPPRef(buf3Builder, nullptr);
    CPPRef<IBuffer> normBuf = context.createBuffer().buildCPPRef(buf3Builder, nullptr);
    CPPRef<IBuffer> texBuf = context.createBuffer().buildCPPRef(buf2Builder, nullptr);
    // CPPRef<IIndexBuffer> indicesBuf = CPPRef<IIndexBuffer>(indiceBuilder->build(nullptr));
    CPPRef<IIndexBuffer> indicesBuf = context.createIndexBuffer().buildCPPRef(indiceBuilder, nullptr);

    posBuf->fillBuffer(context, pos);
    normBuf->fillBuffer(context, norm);
    texBuf->fillBuffer(context, tex);
    indicesBuf->fillBuffer(context, indices);

    VertexArrayArgs vaArgs(3);
    vaArgs.buffers[0] = posBuf;
    vaArgs.buffers[1] = normBuf;
    vaArgs.buffers[2] = texBuf;
    vaArgs.indexBuffer = indicesBuf;
    vaArgs.drawType = DrawType::SeparatedTriangles;
    vaArgs.drawCount = numIndices * 2;
    CPPRef<IVertexArray> vao = context.createVertexArray().buildCPPRef(vaArgs, null);

    return vao;
}
