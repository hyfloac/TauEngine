#pragma once

#include <Safeties.hpp>

class IVertexArray;
class IGraphicsInterface;

class TerrainTile
{
private:
    static CPPRef<IVertexArray> generateTerrain(IGraphicsInterface& gi, float size, uSys edgeVertices) noexcept;
public:
};
