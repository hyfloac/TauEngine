#pragma once

#include <Objects.hpp>
#include "DLL.hpp"

#include "maths/Vector2f.hpp"
#include "maths/Vector3f.hpp"

class TAU_DLL MeshGenerator final
{
    DELETE_CONSTRUCT(MeshGenerator);
    DELETE_DESTRUCT(MeshGenerator);
    DELETE_COPY(MeshGenerator);
public:
    struct Mesh final
    {
        uSys vertexCount;
        uSys indiceCount;
        float* positions;
        float* normals;
        float* tangents;
        float* bitangents;
        float* textures;
        u32* indices;

        void destroy()
        {
            delete positions;
            delete normals;
            delete tangents;
            delete bitangents;
            delete textures;
            delete indices;
            positions = null;
            normals = null;
            tangents = null;
            bitangents = null;
            textures = null;
            indices = null;
        }
    };

    struct Triangle final
    {
        Vector3f position[3];
        Vector3f normal[3];
        Vector2f texture[3];
    };

    struct Square final
    {
        Vector3f position[4];
        Vector3f normal[4];
        Vector2f texture[4];
    };

    /**
     * A series of individual squares and triangles.
     * All converted to triangles in the end.
     */
    struct EditableMesh final
    {
        uSys squareCount;
        uSys triangleCount;
        Square* squares;
        Triangle* triangles;

        void destroy()
        {
            delete squares;
            delete triangles;
            squares = null;
            triangles = null;
        }
    };

    struct GenerationArgs
    {
        union
        {
            struct
            {
                bool simplifyEpsilon : 1;
                bool generateTangents : 1;
                bool generateBitangents : 1;
                bool b3 : 1;
                bool b4 : 1;
                bool b5 : 1;
                bool b6 : 1;
                bool b7 : 1;
            };
            u8 packed;
        };

        float epsilon;
    };
public:
    static Mesh generateMesh(const EditableMesh& mesh, const GenerationArgs& args) noexcept;
    
    /**
     * top -> cube[0]
     * bottom -> cube[1]
     * left -> cube[2]
     * right -> cube[3]
     * front -> cube[4]
     * back -> cube[5]
     */
    static EditableMesh generateCube() noexcept;
};
