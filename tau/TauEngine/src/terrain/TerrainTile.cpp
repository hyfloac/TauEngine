#include "terrain/TerrainTile.hpp"
#include "model/VertexArray.hpp"
#include "model/Buffer.hpp"
#include "system/GraphicsInterface.hpp"

CPPRef<IVertexArray> TerrainTile::generateTerrain(IGraphicsInterface& gi, const float size, const uSys edgeVertices) noexcept
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

    VertexBufferArgs buf3Args(1, false);
    VertexBufferArgs buf2Args(1, false);
    IndexBufferArgs indexArgs;

    buf3Args.usage = EBuffer::UsageType::StaticDraw;
    buf3Args.elementCount = numVertices;
    buf3Args.initialBuffer = pos;
    buf3Args.descriptor.addDescriptor(ShaderSemantic::Position, ShaderDataType::Vector3Float);

    buf2Args.usage = EBuffer::UsageType::StaticDraw;
    buf2Args.elementCount = numVertices;
    buf2Args.initialBuffer = tex;
    buf2Args.descriptor.addDescriptor(ShaderSemantic::TextureCoord, ShaderDataType::Vector2Float);

    indexArgs.usage = EBuffer::UsageType::StaticDraw;
    indexArgs.elementCount = 6 * numIndices;
    indexArgs.initialBuffer = indices;

    const NullableRef<IVertexBuffer> posBuf = gi.createBuffer().buildTauRef(buf3Args, nullptr);
    buf3Args.initialBuffer = norm;
    const NullableRef<IVertexBuffer> normBuf = gi.createBuffer().buildTauRef(buf3Args, nullptr);
    const NullableRef<IVertexBuffer> texBuf = gi.createBuffer().buildTauRef(buf2Args, nullptr);
    const NullableRef<IIndexBuffer> indicesBuf = gi.createBuffer().buildTauRef(indexArgs, nullptr);

    VertexArrayArgs vaArgs(3);
    vaArgs.buffers[0] = posBuf;
    vaArgs.buffers[1] = normBuf;
    vaArgs.buffers[2] = texBuf;
    vaArgs.indexBuffer = indicesBuf;
    vaArgs.drawType = DrawType::SeparatedTriangles;
    vaArgs.drawCount = numIndices * 2;
    CPPRef<IVertexArray> vao = gi.createVertexArray().buildCPPRef(vaArgs, null);

    return vao;
}
