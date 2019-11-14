#pragma once

#include <String.hpp>
#include <vector>

struct Vec2f
{
    float x, y;
};

struct Vec3f
{
    float x, y, z;
};

struct Vertex
{
    Vec3f pos;
    Vec3f normal;
    Vec3f tangent;
    Vec3f biTangent;
    Vec2f tex;
};

struct Material
{
    DynString name;
    Vec3f ambientColor;
    Vec3f diffuseColor;
    Vec3f specularColor;
    float specularExponent;
    float opticalDensity;
    float dissolve;
    i32 illumination;
    DynString ambientTexMap;
    DynString diffuseTexMap;
    DynString specularTexMap;
    DynString specularHighlightMap;
    DynString alphaTexMap;
    DynString bumpTexMap;
};

struct Mesh
{
    DynString name;
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
};
