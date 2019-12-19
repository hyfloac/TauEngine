#pragma once
#pragma warning(push, 0)
#include <string>
#include <vector>
#pragma warning(pop)
#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Utils.hpp>

namespace objl
{
    class Vector2
    {
    private:
        float _x;
        float _y;
    public:
        Vector2() noexcept
            : _x(0.0f), _y(0.0f)
        { }

        Vector2(const float x, const float y) noexcept
            : _x(x), _y(y)
        { }

        inline float& x() noexcept { return _x; }
        inline float& y() noexcept { return _y; }

        inline float x() const noexcept { return _x; }
        inline float y() const noexcept { return _y; }

        inline bool    operator ==(const Vector2& other) const noexcept { return this->_x == other._x && this->_y == other._y; }
        inline bool    operator !=(const Vector2& other) const noexcept { return this->_x != other._x || this->_y != other._y; }

        inline Vector2 operator  +(const Vector2& right) const noexcept { return Vector2(this->_x + right._x, this->_y + right._y); }
        inline Vector2 operator  -(const Vector2& right) const noexcept { return Vector2(this->_x - right._x, this->_y - right._y); }

        inline Vector2 operator  *(const float   scalar) const noexcept { return Vector2(this->_x * scalar, this->_y * scalar); }
    };

    class Vector3
    {
    private:
        float _x;
        float _y;
        float _z;
    public:
        Vector3() noexcept
            : _x(0.0f), _y(0.0f), _z(0.0f)
        { }

        Vector3(const float x, const float y, const float z) noexcept
            : _x(x), _y(y), _z(z)
        { }

        inline float& x() noexcept { return _x; }
        inline float& y() noexcept { return _y; }
        inline float& z() noexcept { return _z; }

        inline float x() const noexcept { return _x; }
        inline float y() const noexcept { return _y; }
        inline float z() const noexcept { return _z; }

        inline bool    operator ==(const Vector3& other) const noexcept { return this->_x == other._x && this->_y == other._y && this->_z == other._z; }
        inline bool    operator !=(const Vector3& other) const noexcept { return this->_x != other._x || this->_y != other._y || this->_z != other._z; }

        inline Vector3 operator  +(const Vector3& right) const noexcept { return Vector3(this->_x + right._x, this->_y + right._y, this->_z + right._z); }
        inline Vector3 operator  -(const Vector3& right) const noexcept { return Vector3(this->_x - right._x, this->_y - right._y, this->_z - right._z); }
                                 
        inline Vector3 operator  *(const float   scalar) const noexcept { return Vector3(this->_x * scalar, this->_y * scalar, this->_z * scalar); }
        inline Vector3 operator  /(const float   scalar) const noexcept { return Vector3(this->_x / scalar, this->_y / scalar, this->_z / scalar); }

        Vector3 cross(const Vector3& other) const noexcept
        {
            return Vector3(this->_y * other._z - this->_z * other._y,
                           this->_z * other._x - this->_x * other._z,
                           this->_x * other._y - this->_y * other._x);
        }

        inline float magnitudeSquared() const noexcept 
        { return this->_x * this->_x + this->_y * this->_y + this->_z * this->_z; }

        float magnitude() const noexcept;

        inline float dot(const Vector3& other) const noexcept 
        { return this->_x * other._x + this->_y * other._y + this->_z * other._z; }

        float midAngleCos(const Vector3& other) const noexcept;

        Vector3 projection(const Vector3& other) const noexcept;
    };

    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector3 tangent;
        Vector3 bitangent;
        Vector2 textureCoordinate;
    };

    struct Material
    {
        // Material Name
        std::string name;
        // Ambient Color
        Vector3 Ka;
        // Diffuse Color
        Vector3 Kd;
        // Specular Color
        Vector3 Ks;
        // Specular Exponent
        float Ns;
        // Optical Density
        float Ni;
        // Dissolve
        float d;
        // Illumination
        i32 illum;
        // Ambient Texture Map
        std::string map_Ka;
        // Diffuse Texture Map
        std::string map_Kd;
        // Specular Texture Map
        std::string map_Ks;
        // Specular Highlight Map
        std::string map_Ns;
        // Alpha Texture Map
        std::string map_d;
        // Bump Map
        std::string map_bump;

        Material() noexcept
            : name(""),
              Ns(0.0f),
              Ni(0.0f),
              d(0.0f),
              illum(0)
        { }
    };

    struct Mesh
    {
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        Material material;

        Mesh() noexcept = default;

        Mesh(std::vector<Vertex>& _vertices, std::vector<u32>& _indices) noexcept
            : vertices(_vertices), indices(_indices)
        { }
    };

    namespace algorithm
    {
        inline Vector3 operator *(const float& RESTRICT left, const Vector3& RESTRICT right) noexcept { return right * left; }
    }

    class TAU_DLL Loader
    {
    private:
        std::vector<Mesh>     _meshes;
        std::vector<Vertex>   _vertices;
        std::vector<u32>      _indices;
        std::vector<Material> _materials;
    private:
        // Generate vertices from a list of positions, 
        //	tcoords, normals and a face line
        static void genVerticesFromRawOBJ(std::vector<Vertex>& RESTRICT vertices, const std::vector<Vector3>& RESTRICT positions, const std::vector<Vector2>& RESTRICT textureCoords, const std::vector<Vector3>& RESTRICT normals, const char* currentLine) noexcept;

        // Triangulate a list of vertices into a face by printing
        //	indices corresponding with triangles within it
        static void vertexTriangulation(std::vector<u32>& RESTRICT indices, const std::vector<Vertex>& RESTRICT vertices) noexcept;

        bool loadMaterials(const char* path) noexcept;
    public:
        Loader() noexcept = default;

        inline const std::vector<Mesh>& meshes() const noexcept { return _meshes; }
        inline       std::vector<Mesh>& meshes()       noexcept { return _meshes; }

        inline const std::vector<Vertex>& vertices() const noexcept { return _vertices; }
        inline       std::vector<Vertex>& vertices()       noexcept { return _vertices; }

        inline const std::vector<u32>& indices() const noexcept { return _indices; }
        inline       std::vector<u32>& indices()       noexcept { return _indices; }

        inline const std::vector<Material>& materials() const noexcept { return _materials; }
        inline       std::vector<Material>& materials()       noexcept { return _materials; }

        // Load a file into the loader
        //
        // If file is loaded return true
        //
        // If the file is unable to be found
        // or unable to be loaded return false
        bool loadFile(const char* path) noexcept;
    };
}
