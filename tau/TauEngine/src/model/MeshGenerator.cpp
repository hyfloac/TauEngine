#include "model/MeshGenerator.hpp"
#include <cstring>

static bool triangleEquals(MeshGenerator::Triangle& a, MeshGenerator::Triangle& b) noexcept;
static bool triangleEquals(const MeshGenerator::Triangle& a, const MeshGenerator::Triangle& b, float epsilon) noexcept;
static void triangulateSquare(const MeshGenerator::Square& square, MeshGenerator::Triangle* subdivided) noexcept;

static void normalizeNormals(MeshGenerator::Triangle* triangles, uSys triangleCount) noexcept;
static void normalizeNormals(MeshGenerator::Triangle* triangles, uSys triangleCount, float epsilon) noexcept;

static uSys optimizeMesh(float* positions, const float* textures, u32* indices, uSys vertexCount, uSys indiceCount) noexcept;
static uSys optimizeMesh(float* positions, const float* textures, u32* indices, uSys vertexCount, uSys indiceCount, float epsilon) noexcept;

MeshGenerator::Mesh MeshGenerator::generateMesh(const EditableMesh& mesh, const GenerationArgs& args) noexcept
{
    const uSys totalTriangles = mesh.triangleCount + mesh.squareCount * 2;
    Triangle* triangles = new(::std::nothrow) Triangle[totalTriangles];
    ::std::memcpy(triangles, mesh.triangles, mesh.triangleCount * sizeof(Triangle));

    const uSys offset = mesh.triangleCount;
    for(uSys i = 0; i < mesh.squareCount; ++i)
    {
        triangulateSquare(mesh.squares[i], &triangles[2 * i + offset]);
    }

    if(args.simplifyEpsilon)
    {
        normalizeNormals(triangles, totalTriangles, args.epsilon);
    }
    else
    {
        normalizeNormals(triangles, totalTriangles);
    }

    const uSys vertexCount = totalTriangles * 3;

    float* positions = new(::std::nothrow) float[vertexCount * 3];
    float* normals = new(::std::nothrow) float[vertexCount * 3];
    // float* tangents = args.generateTangents ? new(::std::nothrow) float[vertexCount * 3] : null;
    // float* bitangents = args.generateTangents && args.generateBitangents ? new(::std::nothrow) float[vertexCount * 3] : null;
    float* textures = new(::std::nothrow) float[vertexCount * 2];
    u32* indices = new(::std::nothrow) u32[vertexCount];

    for(uSys i = 0; i < totalTriangles; ++i)
    {
        positions[i * 9 + 0] = triangles[i].position[0].x();
        positions[i * 9 + 1] = triangles[i].position[0].y();
        positions[i * 9 + 2] = triangles[i].position[0].z();
        positions[i * 9 + 3] = triangles[i].position[1].x();
        positions[i * 9 + 4] = triangles[i].position[1].y();
        positions[i * 9 + 5] = triangles[i].position[1].z();
        positions[i * 9 + 6] = triangles[i].position[2].x();
        positions[i * 9 + 7] = triangles[i].position[2].y();
        positions[i * 9 + 8] = triangles[i].position[2].z();

        normals[i * 9 + 0] = triangles[i].normal[0].x();
        normals[i * 9 + 1] = triangles[i].normal[0].y();
        normals[i * 9 + 2] = triangles[i].normal[0].z();
        normals[i * 9 + 3] = triangles[i].normal[1].x();
        normals[i * 9 + 4] = triangles[i].normal[1].y();
        normals[i * 9 + 5] = triangles[i].normal[1].z();
        normals[i * 9 + 6] = triangles[i].normal[2].x();
        normals[i * 9 + 7] = triangles[i].normal[2].y();
        normals[i * 9 + 8] = triangles[i].normal[2].z();

        textures[i * 6 + 0] = triangles[i].texture[0].x();
        textures[i * 6 + 1] = triangles[i].texture[0].y();
        textures[i * 6 + 2] = triangles[i].texture[1].x();
        textures[i * 6 + 3] = triangles[i].texture[1].y();
        textures[i * 6 + 4] = triangles[i].texture[2].x();
        textures[i * 6 + 5] = triangles[i].texture[2].y();

        indices[i * 3 + 0] = i * 3 + 0;
        indices[i * 3 + 1] = i * 3 + 1;
        indices[i * 3 + 2] = i * 3 + 2;
    }

    // TODO: Generate Tangents
    // TODO: Generate Bitangents

    uSys discardCount = 0;

    if(args.simplifyEpsilon)
    {
        discardCount = optimizeMesh(positions, textures, indices, vertexCount, vertexCount, args.epsilon);
    }
    else
    {
        discardCount = optimizeMesh(positions, textures, indices, vertexCount, vertexCount);
    }

    const uSys totalVertices = vertexCount - discardCount;

    float* positionsRet = new(::std::nothrow) float[totalVertices * 3];
    float* normalsRet = new(::std::nothrow) float[totalVertices * 3];
    float* texturesRet = new(::std::nothrow) float[totalVertices * 2];

    uSys insertIndex = 0;
    for(uSys i = 0; i < vertexCount; ++i)
    {
        if(isnan(positions[i * 3]))
        {
            for(uSys j = 0; j < vertexCount; ++j)
            {
                if(indices[j] > insertIndex)
                {
                    --indices[j];
                }
            }
        }
        else
        {
            positionsRet[insertIndex * 3 + 0] = positions[i * 3 + 0];
            positionsRet[insertIndex * 3 + 1] = positions[i * 3 + 1];
            positionsRet[insertIndex * 3 + 2] = positions[i * 3 + 2];

            normalsRet[insertIndex * 3 + 0] = normals[i * 3 + 0];
            normalsRet[insertIndex * 3 + 1] = normals[i * 3 + 1];
            normalsRet[insertIndex * 3 + 2] = normals[i * 3 + 2];

            texturesRet[insertIndex * 2 + 0] = textures[i * 2 + 0];
            texturesRet[insertIndex * 2 + 1] = textures[i * 2 + 1];
            ++insertIndex;
        }
    }

    return Mesh{ totalVertices, vertexCount, positionsRet, normalsRet, null, null, texturesRet, indices };
}

MeshGenerator::EditableMesh MeshGenerator::generateCube() noexcept
{
    EditableMesh ret{ 6, 0, new(::std::nothrow) Square[6], null };

    // Top (0, +y, 0)
    {
        ret.squares[0].position[0] = Vector3f(-1.0f,  1.0f, -1.0f);
        ret.squares[0].position[1] = Vector3f(-1.0f,  1.0f,  1.0f);
        ret.squares[0].position[2] = Vector3f( 1.0f,  1.0f, -1.0f);
        ret.squares[0].position[3] = Vector3f( 1.0f,  1.0f,  1.0f);

        ret.squares[0].normal[0] = Vector3f(0.0f, 1.0f, 0.0f);
        ret.squares[0].normal[1] = Vector3f(0.0f, 1.0f, 0.0f);
        ret.squares[0].normal[2] = Vector3f(0.0f, 1.0f, 0.0f);
        ret.squares[0].normal[3] = Vector3f(0.0f, 1.0f, 0.0f);

        ret.squares[0].texture[0] = Vector2f(0.0f, 0.0f);
        ret.squares[0].texture[1] = Vector2f(1.0f, 0.0f);
        ret.squares[0].texture[2] = Vector2f(0.0f, 1.0f);
        ret.squares[0].texture[3] = Vector2f(1.0f, 1.0f);
    }

    // Bottom (0, -y, 0)
    {
        ret.squares[1].position[0] = Vector3f(-1.0f, -1.0f, -1.0f);
        ret.squares[1].position[1] = Vector3f( 1.0f, -1.0f, -1.0f);
        ret.squares[1].position[2] = Vector3f(-1.0f, -1.0f,  1.0f);
        ret.squares[1].position[3] = Vector3f( 1.0f, -1.0f,  1.0f);
                    
        ret.squares[1].normal[0] = Vector3f( 0.0f, -1.0f,  0.0f);
        ret.squares[1].normal[1] = Vector3f( 0.0f, -1.0f,  0.0f);
        ret.squares[1].normal[2] = Vector3f( 0.0f, -1.0f,  0.0f);
        ret.squares[1].normal[3] = Vector3f( 0.0f, -1.0f,  0.0f);
                    
        ret.squares[1].texture[0] = Vector2f(0.0f, 0.0f);
        ret.squares[1].texture[1] = Vector2f(1.0f, 0.0f);
        ret.squares[1].texture[2] = Vector2f(0.0f, 1.0f);
        ret.squares[1].texture[3] = Vector2f(1.0f, 1.0f);
    }

    // Left (-x, 0, 0)
    {
        ret.squares[2].position[0] = Vector3f(-1.0f, -1.0f,  1.0f);
        ret.squares[2].position[1] = Vector3f(-1.0f,  1.0f,  1.0f);
        ret.squares[2].position[2] = Vector3f(-1.0f, -1.0f, -1.0f);
        ret.squares[2].position[3] = Vector3f(-1.0f,  1.0f, -1.0f);
                
        ret.squares[2].normal[0] = Vector3f( 1.0f,  0.0f,  0.0f);
        ret.squares[2].normal[1] = Vector3f( 1.0f,  0.0f,  0.0f);
        ret.squares[2].normal[2] = Vector3f( 1.0f,  0.0f,  0.0f);
        ret.squares[2].normal[3] = Vector3f( 1.0f,  0.0f,  0.0f);
                
        ret.squares[2].texture[0] = Vector2f(0.0f, 0.0f);
        ret.squares[2].texture[1] = Vector2f(1.0f, 0.0f);
        ret.squares[2].texture[2] = Vector2f(0.0f, 1.0f);
        ret.squares[2].texture[3] = Vector2f(1.0f, 1.0f);
    }

    // Right (+x, 0, 0)
    {
        ret.squares[3].position[0] = Vector3f( 1.0f, -1.0f, -1.0f);
        ret.squares[3].position[1] = Vector3f( 1.0f,  1.0f, -1.0f);
        ret.squares[3].position[2] = Vector3f( 1.0f, -1.0f,  1.0f);
        ret.squares[3].position[3] = Vector3f( 1.0f,  1.0f,  1.0f);
                
        ret.squares[3].normal[0] = Vector3f(-1.0f,  0.0f,  0.0f);
        ret.squares[3].normal[1] = Vector3f(-1.0f,  0.0f,  0.0f);
        ret.squares[3].normal[2] = Vector3f(-1.0f,  0.0f,  0.0f);
        ret.squares[3].normal[3] = Vector3f(-1.0f,  0.0f,  0.0f);
                
        ret.squares[3].texture[0] = Vector2f(0.0f, 0.0f);
        ret.squares[3].texture[1] = Vector2f(1.0f, 0.0f);
        ret.squares[3].texture[2] = Vector2f(0.0f, 1.0f);
        ret.squares[3].texture[3] = Vector2f(1.0f, 1.0f);
    }

    // Front (0, 0, -z)
    {
        ret.squares[4].position[0] = Vector3f(-1.0f,  1.0f, -1.0f);
        ret.squares[4].position[1] = Vector3f( 1.0f,  1.0f, -1.0f);
        ret.squares[4].position[2] = Vector3f(-1.0f, -1.0f, -1.0f);
        ret.squares[4].position[3] = Vector3f( 1.0f, -1.0f, -1.0f);
                
        ret.squares[4].normal[0] = Vector3f( 0.0f,  0.0f,  1.0f);
        ret.squares[4].normal[1] = Vector3f( 0.0f,  0.0f,  1.0f);
        ret.squares[4].normal[2] = Vector3f( 0.0f,  0.0f,  1.0f);
        ret.squares[4].normal[3] = Vector3f( 0.0f,  0.0f,  1.0f);
                
        ret.squares[4].texture[0] = Vector2f(0.0f, 0.0f);
        ret.squares[4].texture[1] = Vector2f(1.0f, 0.0f);
        ret.squares[4].texture[2] = Vector2f(0.0f, 1.0f);
        ret.squares[4].texture[3] = Vector2f(1.0f, 1.0f);
    }

    // Back (0, 0, +z)
    {
        ret.squares[5].position[0] = Vector3f(-1.0f, -1.0f,  1.0f);
        ret.squares[5].position[1] = Vector3f( 1.0f, -1.0f,  1.0f);
        ret.squares[5].position[2] = Vector3f(-1.0f,  1.0f,  1.0f);
        ret.squares[5].position[3] = Vector3f( 1.0f,  1.0f,  1.0f);
                
        ret.squares[5].normal[0] = Vector3f( 0.0f,  0.0f, -1.0f);
        ret.squares[5].normal[1] = Vector3f( 0.0f,  0.0f, -1.0f);
        ret.squares[5].normal[2] = Vector3f( 0.0f,  0.0f, -1.0f);
        ret.squares[5].normal[3] = Vector3f( 0.0f,  0.0f, -1.0f);
                
        ret.squares[5].texture[0] = Vector2f(0.0f, 0.0f);
        ret.squares[5].texture[1] = Vector2f(1.0f, 0.0f);
        ret.squares[5].texture[2] = Vector2f(0.0f, 1.0f);
        ret.squares[5].texture[3] = Vector2f(1.0f, 1.0f);
    }

    return ret;
}

static bool epsilonEquals(const float a, const float b, const float epsilon) noexcept
{
    if(a == b) { return true; }
    return abs(a - b) < epsilon;
}

static bool vecEquals(const Vector2f& a, const Vector2f& b) noexcept
{
    bool val = false;
    val |= a.x() == b.x();
    val |= a.y() == b.y();
    val |= a.x() == b.x();
    val |= a.y() == b.y();
    val |= a.x() == b.x();
    val |= a.y() == b.y();

    return !val;
}

static bool vecEquals(const Vector2f& a, const Vector2f& b, const float epsilon) noexcept
{
    bool val = false;
    val |= epsilonEquals(a.x(), b.x(), epsilon);
    val |= epsilonEquals(a.y(), b.y(), epsilon);
    val |= epsilonEquals(a.x(), b.x(), epsilon);
    val |= epsilonEquals(a.y(), b.y(), epsilon);
    val |= epsilonEquals(a.x(), b.x(), epsilon);
    val |= epsilonEquals(a.y(), b.y(), epsilon);

    return !val;
}

static bool vecEquals(const Vector3f& a, const Vector3f& b) noexcept
{
    bool val = false;
    val |= a.x() == b.x();
    val |= a.y() == b.y();
    val |= a.z() == b.z();
    val |= a.x() == b.x();
    val |= a.y() == b.y();
    val |= a.z() == b.z();
    val |= a.x() == b.x();
    val |= a.y() == b.y();
    val |= a.z() == b.z();

    return !val;
}

static bool vecEquals(const Vector3f& a, const Vector3f& b, const float epsilon) noexcept
{
    bool val = false;
    val |= epsilonEquals(a.x(), b.x(), epsilon);
    val |= epsilonEquals(a.y(), b.y(), epsilon);
    val |= epsilonEquals(a.z(), b.z(), epsilon);
    val |= epsilonEquals(a.x(), b.x(), epsilon);
    val |= epsilonEquals(a.y(), b.y(), epsilon);
    val |= epsilonEquals(a.z(), b.z(), epsilon);
    val |= epsilonEquals(a.x(), b.x(), epsilon);
    val |= epsilonEquals(a.y(), b.y(), epsilon);
    val |= epsilonEquals(a.z(), b.z(), epsilon);

    return !val;
}

static bool triangleEquals(MeshGenerator::Triangle& a, MeshGenerator::Triangle& b) noexcept
{
    bool val = false;
    val |= a.position[0].x() == b.position[0].x();
    val |= a.position[0].y() == b.position[0].y();
    val |= a.position[0].z() == b.position[0].z();
    val |= a.position[1].x() == b.position[1].x();
    val |= a.position[1].y() == b.position[1].y();
    val |= a.position[1].z() == b.position[1].z();
    val |= a.position[2].x() == b.position[2].x();
    val |= a.position[2].y() == b.position[2].y();
    val |= a.position[2].z() == b.position[2].z();

    val |= a.normal[0].x() == b.normal[0].x();
    val |= a.normal[0].y() == b.normal[0].y();
    val |= a.normal[0].z() == b.normal[0].z();
    val |= a.normal[1].x() == b.normal[1].x();
    val |= a.normal[1].y() == b.normal[1].y();
    val |= a.normal[1].z() == b.normal[1].z();
    val |= a.normal[2].x() == b.normal[2].x();
    val |= a.normal[2].y() == b.normal[2].y();
    val |= a.normal[2].z() == b.normal[2].z();

    val |= a.texture[0].x() == b.texture[0].x();
    val |= a.texture[0].y() == b.texture[0].y();
    val |= a.texture[1].x() == b.texture[1].x();
    val |= a.texture[1].y() == b.texture[1].y();
    val |= a.texture[2].x() == b.texture[2].x();
    val |= a.texture[2].y() == b.texture[2].y();

    return !val;
}

static bool triangleEquals(const MeshGenerator::Triangle& a, const MeshGenerator::Triangle& b, const float epsilon) noexcept
{
    bool val = false;
    val |= epsilonEquals(a.position[0].x(), b.position[0].x(), epsilon);
    val |= epsilonEquals(a.position[0].y(), b.position[0].y(), epsilon);
    val |= epsilonEquals(a.position[0].z(), b.position[0].z(), epsilon);
    val |= epsilonEquals(a.position[1].x(), b.position[1].x(), epsilon);
    val |= epsilonEquals(a.position[1].y(), b.position[1].y(), epsilon);
    val |= epsilonEquals(a.position[1].z(), b.position[1].z(), epsilon);
    val |= epsilonEquals(a.position[2].x(), b.position[2].x(), epsilon);
    val |= epsilonEquals(a.position[2].y(), b.position[2].y(), epsilon);
    val |= epsilonEquals(a.position[2].z(), b.position[2].z(), epsilon);

    val |= epsilonEquals(a.normal[0].x(), b.normal[0].x(), epsilon);
    val |= epsilonEquals(a.normal[0].y(), b.normal[0].y(), epsilon);
    val |= epsilonEquals(a.normal[0].z(), b.normal[0].z(), epsilon);
    val |= epsilonEquals(a.normal[1].x(), b.normal[1].x(), epsilon);
    val |= epsilonEquals(a.normal[1].y(), b.normal[1].y(), epsilon);
    val |= epsilonEquals(a.normal[1].z(), b.normal[1].z(), epsilon);
    val |= epsilonEquals(a.normal[2].x(), b.normal[2].x(), epsilon);
    val |= epsilonEquals(a.normal[2].y(), b.normal[2].y(), epsilon);
    val |= epsilonEquals(a.normal[2].z(), b.normal[2].z(), epsilon);

    val |= epsilonEquals(a.texture[0].x(), b.texture[0].x(), epsilon);
    val |= epsilonEquals(a.texture[0].y(), b.texture[0].y(), epsilon);
    val |= epsilonEquals(a.texture[1].x(), b.texture[1].x(), epsilon);
    val |= epsilonEquals(a.texture[1].y(), b.texture[1].y(), epsilon);
    val |= epsilonEquals(a.texture[2].x(), b.texture[2].x(), epsilon);
    val |= epsilonEquals(a.texture[2].y(), b.texture[2].y(), epsilon);

    return !val;
}

static void triangulateSquare(const MeshGenerator::Square& square, MeshGenerator::Triangle* const subdivided) noexcept
{
    MeshGenerator::Triangle& first = subdivided[0];
    first.position[0] = square.position[0];
    first.position[1] = square.position[1];
    first.position[2] = square.position[3];
    first.normal[0] = square.normal[0];
    first.normal[1] = square.normal[1];
    first.normal[2] = square.normal[3];
    first.texture[0] = square.texture[0];
    first.texture[1] = square.texture[1];
    first.texture[2] = square.texture[3];

    MeshGenerator::Triangle& second = subdivided[1];
    second.position[0] = square.position[0];
    second.position[1] = square.position[3];
    second.position[2] = square.position[2];
    second.normal[0] = square.normal[0];
    second.normal[1] = square.normal[3];
    second.normal[2] = square.normal[2];
    second.texture[0] = square.texture[0];
    second.texture[1] = square.texture[3];
    second.texture[2] = square.texture[2];
}

static void normalizeNormals(MeshGenerator::Triangle* const triangles, const uSys triangleCount) noexcept
{
    for(uSys i = 0; i < triangleCount; ++i)
    {
        for(uSys j = 0; j < 3; ++j)
        {
            const Vector3f position = triangles[i].position[j];
            Vector3f normalSum = triangles[i].normal[j];
            for(uSys k = i + 1; k < triangleCount; ++k)
            {
                if(vecEquals(position, triangles[k].position[j]))
                {
                    normalSum += triangles[k].normal[j];
                }
            }
            normalSum.normalize();
            for(uSys k = i + 1; k < triangleCount; ++k)
            {
                if(vecEquals(position, triangles[k].position[j]))
                {
                    triangles[k].normal[j] = normalSum;
                }
            }
            triangles[i].normal[j] = normalSum;
        }
    }
}

static void normalizeNormals(MeshGenerator::Triangle* const triangles, const uSys triangleCount, const float epsilon) noexcept
{
    for(uSys i = 0; i < triangleCount; ++i)
    {
        for(uSys j = 0; j < 3; ++j)
        {
            const Vector3f position = triangles[i].position[j];
            Vector3f normalSum = triangles[i].normal[j];
            for(uSys k = i + 1; k < triangleCount; ++k)
            {
                if(vecEquals(position, triangles[k].position[j], epsilon))
                {
                    normalSum += triangles[k].normal[j];
                }
            }
            normalSum.normalize();
            for(uSys k = i + 1; k < triangleCount; ++k)
            {
                if(vecEquals(position, triangles[k].position[j], epsilon))
                {
                    triangles[k].normal[j] = normalSum;
                }
            }
            triangles[i].normal[j] = normalSum;
        }
    }
}

static uSys optimizeMesh(float* const positions, const float* const textures, u32* const indices, const uSys vertexCount, const uSys indiceCount) noexcept
{
    uSys discardCount = 0;

    for(uSys i = 0; i < vertexCount; ++i)
    {
        if(isnan(positions[i * 3]))
        {
            continue;
        }

        const float posX = positions[i * 3 + 0];
        const float posY = positions[i * 3 + 1];
        const float posZ = positions[i * 3 + 2];

        const float texU = textures[i * 2 + 0];
        const float texV = textures[i * 2 + 1];

        for(uSys j = i + 1; j < vertexCount; ++j)
        {
            const float posX1 = positions[j * 3 + 0];
            const float posY1 = positions[j * 3 + 1];
            const float posZ1 = positions[j * 3 + 2];

            const float texU1 = textures[j * 2 + 0];
            const float texV1 = textures[j * 2 + 1];

            if(posX == posX1 &&
                posY == posY1 &&
                posZ == posZ1 &&
                texU == texU1 &&
                texV == texV1)
            {
                ++discardCount;
                for(uSys k = 0; k < indiceCount; ++k)
                {
                    if(indices[k] == j)
                    {
                        indices[k] = i;
                    }
                }
                positions[j * 3] = NAN;
            }
        }
    }

    return discardCount;
}

static uSys optimizeMesh(float* const positions, const float* const textures, u32* const indices, const uSys vertexCount, const uSys indiceCount, const float epsilon) noexcept
{
    uSys discardCount = 0;

    for(uSys i = 0; i < vertexCount; ++i)
    {
        if(isnan(positions[i * 3]))
        {
            continue;
        }

        const float posX = positions[i * 3 + 0];
        const float posY = positions[i * 3 + 1];
        const float posZ = positions[i * 3 + 2];

        const float texU = textures[i * 2 + 0];
        const float texV = textures[i * 2 + 0];


        for(uSys j = i + 1; j < vertexCount; ++j)
        {
            const float posX1 = positions[j * 3 + 0];
            const float posY1 = positions[j * 3 + 1];
            const float posZ1 = positions[j * 3 + 2];

            const float texU1 = textures[j * 2 + 0];
            const float texV1 = textures[j * 2 + 0];

            if(epsilonEquals(posX, posX1, epsilon) &&
               epsilonEquals(posY, posY1, epsilon) &&
               epsilonEquals(posZ, posZ1, epsilon) &&
               epsilonEquals(texU, texU1, epsilon) &&
               epsilonEquals(texV, texV1, epsilon))
            {
                ++discardCount;
                for(uSys k = 0; k < indiceCount; ++k)
                {
                    if(indices[k] == j)
                    {
                        indices[k] = i;
                    }
                }
                positions[j * 3] = NAN;
            }
        }
    }

    return discardCount;
}
