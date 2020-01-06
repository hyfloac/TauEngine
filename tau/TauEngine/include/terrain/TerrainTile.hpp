#pragma once

#include "model/IVertexArray.hpp"
#include "maths/Vector3f.hpp"

class TerrainTile
{
private:
    static Ref<IVertexArray> generateTerrain(IRenderingContext& context, float size, uSys edgeVertices) noexcept;
public:
};
