#include "pbr/SphereGenerator.hpp"

#include "maths/Vector2f.hpp"
#include "maths/Vector3f.hpp"

#pragma push(pack, 1)
struct Vec2 final
{
    float x;
    float y;
};

struct Vec3 final
{
    float x;
    float y;
    float z;
};

struct Triangle final
{
    Vector3f position[3];
    Vector3f normal[3];
    Vector2f texture[3];
};

/**
 * 0 1
 * 2 3
 */
struct Square final
{
    Vector3f position[4];
    Vector3f normal[4];
    Vector2f texture[4];
};

struct TriSquare final
{
    Triangle triangles[2];
};

struct SubdividedSquare final
{
    Square squares[4];
};
#pragma pop(pack)

static void triangulateSquare(const Square& square, TriSquare* const subdivided) noexcept;
static void subdivideSquare(const Square& square, SubdividedSquare* const subdivided) noexcept;

CPPRef<IVertexArray> generateNormalizedCubeSphere(const uSys subdivisions) noexcept
{
    return null;
}

static void triangulateSquare(const Square& square, TriSquare* const subdivided) noexcept
{
    Triangle& first = subdivided->triangles[0];
    first.position[0] = square.position[0];
    first.position[1] = square.position[1];
    first.position[2] = square.position[3];
    first.normal[0] = square.normal[0];
    first.normal[1] = square.normal[1];
    first.normal[2] = square.normal[3];
    first.texture[0] = square.texture[0];
    first.texture[1] = square.texture[1];
    first.texture[2] = square.texture[3];

    Triangle& second = subdivided->triangles[1];
    second.position[0] = square.position[1];
    second.position[1] = square.position[2];
    second.position[2] = square.position[3];
    second.normal[0] = square.normal[1];
    second.normal[1] = square.normal[2];
    second.normal[2] = square.normal[3];
    second.texture[0] = square.texture[1];
    second.texture[1] = square.texture[2];
    second.texture[2] = square.texture[3];
}

static void subdivideSquare(const Square& square, SubdividedSquare* const subdivided) noexcept
{
    {
        const Vector3f a = square.position[0];
        const Vector3f b = square.position[1];
        const Vector3f c = square.position[3];
        const Vector3f d = square.position[2];

        const Vector3f ab = (a + b) / 2.0f;
        const Vector3f bc = (b + c) / 2.0f;
        const Vector3f cd = (c + d) / 2.0f;
        const Vector3f da = (d + a) / 2.0f;

        const Vector3f q = (ab + bc + cd + da) / 2.0f;

        subdivided->squares[0].position[0] = a;
        subdivided->squares[0].position[1] = ab;
        subdivided->squares[0].position[3] = q;
        subdivided->squares[0].position[2] = da;

        subdivided->squares[1].position[0] = ab;
        subdivided->squares[1].position[1] = b;
        subdivided->squares[1].position[3] = bc;
        subdivided->squares[1].position[2] = q;

        subdivided->squares[2].position[0] = q;
        subdivided->squares[2].position[1] = bc;
        subdivided->squares[2].position[3] = c;
        subdivided->squares[2].position[2] = cd;

        subdivided->squares[3].position[0] = da;
        subdivided->squares[3].position[1] = q;
        subdivided->squares[3].position[3] = cd;
        subdivided->squares[3].position[2] = d;
    }

    {
        const Vector3f a = square.normal[0];
        const Vector3f b = square.normal[1];
        const Vector3f c = square.normal[3];
        const Vector3f d = square.normal[2];

        const Vector3f ab = (a + b) / 2.0f;
        const Vector3f bc = (b + c) / 2.0f;
        const Vector3f cd = (c + d) / 2.0f;
        const Vector3f da = (d + a) / 2.0f;

        const Vector3f q = (ab + bc + cd + da) / 2.0f;

        subdivided->squares[0].normal[0] = a;
        subdivided->squares[0].normal[1] = ab;
        subdivided->squares[0].normal[3] = q;
        subdivided->squares[0].normal[2] = da;
                               
        subdivided->squares[1].normal[0] = ab;
        subdivided->squares[1].normal[1] = b;
        subdivided->squares[1].normal[3] = bc;
        subdivided->squares[1].normal[2] = q;
                               
        subdivided->squares[2].normal[0] = q;
        subdivided->squares[2].normal[1] = bc;
        subdivided->squares[2].normal[3] = c;
        subdivided->squares[2].normal[2] = cd;
                               
        subdivided->squares[3].normal[0] = da;
        subdivided->squares[3].normal[1] = q;
        subdivided->squares[3].normal[3] = cd;
        subdivided->squares[3].normal[2] = d;
    }

    {
        const Vector2f a = square.texture[0];
        const Vector2f b = square.texture[1];
        const Vector2f c = square.texture[3];
        const Vector2f d = square.texture[2];
                    
        const Vector2f ab = (a + b) / 2.0f;
        const Vector2f bc = (b + c) / 2.0f;
        const Vector2f cd = (c + d) / 2.0f;
        const Vector2f da = (d + a) / 2.0f;
                    
        const Vector2f q = (ab + bc + cd + da) / 2.0f;

        subdivided->squares[0].texture[0] = a;
        subdivided->squares[0].texture[1] = ab;
        subdivided->squares[0].texture[3] = q;
        subdivided->squares[0].texture[2] = da;
                               
        subdivided->squares[1].texture[0] = ab;
        subdivided->squares[1].texture[1] = b;
        subdivided->squares[1].texture[3] = bc;
        subdivided->squares[1].texture[2] = q;
                               
        subdivided->squares[2].texture[0] = q;
        subdivided->squares[2].texture[1] = bc;
        subdivided->squares[2].texture[3] = c;
        subdivided->squares[2].texture[2] = cd;
                               
        subdivided->squares[3].texture[0] = da;
        subdivided->squares[3].texture[1] = q;
        subdivided->squares[3].texture[3] = cd;
        subdivided->squares[3].texture[2] = d;
    }
}

/**
 * top -> cube[0]
 * bottom -> cube[1]
 * left -> cube[2]
 * right -> cube[3]
 * front -> cube[4]
 * back -> cube[5]
 */
static void generateCube(const float magnitude, Square cube[6]) noexcept
{
    cube[0].position[0] = Vector3f( 1.0f,  1.0f,  1.0f);
    cube[0].position[1] = Vector3f(-1.0f,  1.0f,  1.0f);
    cube[0].position[2] = Vector3f( 1.0f,  1.0f, -1.0f);
    cube[0].position[3] = Vector3f(-1.0f,  1.0f, -1.0f);
}