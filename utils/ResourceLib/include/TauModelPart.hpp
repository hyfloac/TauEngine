#pragma once

#include <Objects.hpp>
#include <Utils.hpp>
#include <NumTypes.hpp>
#include "IFile.hpp"

class TauModelPart final
{
public:
    class AnimationComposition;
public:
    static DynArray<TauModelPart> parse(const CPPRef<IFile>& file) noexcept;
private:
    uSys _vertexCount;
    uSys _indiceCount;

    float* _positions;
    float* _normals;
    float* _tangents;
    float* _textureUVs;
    uSys* _indices;
    AnimationComposition* _animationBones; 

    uSys* _refCount;
public:
    TauModelPart() noexcept
        : _vertexCount(0), _indiceCount(0),
          _positions(null), _normals(null), _tangents(null), _textureUVs(null),
          _indices(null), _refCount(null)
    { }

    TauModelPart(nullptr_t) noexcept
        : _vertexCount(0), _indiceCount(0),
          _positions(null), _normals(null), _tangents(null), _textureUVs(null),
          _indices(null), _refCount(null)
    { }

    TauModelPart(const uSys vertexCount, const uSys indiceCount) noexcept
        : _vertexCount(vertexCount), _indiceCount(indiceCount),
          _positions(null), _normals(null), _tangents(null), _textureUVs(null),
          _indices(new(::std::nothrow) uSys[indiceCount]), _refCount(new(::std::nothrow) uSys(1))
    {
        const uSys vec3Count = vertexCount * 3;
        const uSys vec2Count = vertexCount * 2;

        _positions = new(::std::nothrow) float[vec3Count];
        _normals = new(::std::nothrow) float[vec3Count];
        _tangents = new(::std::nothrow) float[vec3Count];
        _textureUVs = new(::std::nothrow) float[vec2Count];
    }

    ~TauModelPart() noexcept
    {
        if(_refCount)
        {
            if(--(*_refCount) == 0)
            {
                delete[] _positions;
                delete[] _normals;
                delete[] _tangents;
                delete[] _textureUVs;
                delete[] _indices;
                delete _refCount;
            }
        }
    }

    TauModelPart(const TauModelPart& copy) noexcept
        :  _vertexCount(copy._vertexCount), _indiceCount(copy._indiceCount),
          _positions(copy._positions), _normals(copy._normals),
          _tangents(copy._tangents), _textureUVs(copy._textureUVs),
          _indices(copy._indices),
          _refCount(copy._refCount)
    { ++(*_refCount); }

    TauModelPart(TauModelPart&& move) noexcept
        : _vertexCount(move._vertexCount), _indiceCount(move._indiceCount),
          _positions(move._positions), _normals(move._normals),
          _tangents(move._tangents), _textureUVs(move._textureUVs),
          _indices(move._indices),
          _refCount(move._refCount)
    { ++(*_refCount); }

    TauModelPart& operator =(const TauModelPart& copy) noexcept
    {
        if(&copy == this)
        { return *this; }

        TauModelPart::~TauModelPart();
        _vertexCount = copy._vertexCount;
        _indiceCount = copy._indiceCount;
        _positions = copy._positions;
        _normals = copy._normals;
        _tangents = copy._tangents;
        _textureUVs = copy._textureUVs;
        _indices = copy._indices;
        _refCount = copy._refCount;

        if(_refCount)
        { ++(*_refCount); }

        return *this;
    }

    TauModelPart& operator =(TauModelPart&& move) noexcept
    {
        if(&move == this)
        { return *this; }

        TauModelPart::~TauModelPart();
        _vertexCount = move._vertexCount;
        _indiceCount = move._indiceCount;
        _positions = move._positions;
        _normals = move._normals;
        _tangents = move._tangents;
        _textureUVs = move._textureUVs;
        _indices = move._indices;
        _refCount = move._refCount;

        if(_refCount)
        { ++(*_refCount); }

        return *this;
    }

    TauModelPart& operator =(nullptr_t) noexcept
    {
        TauModelPart::~TauModelPart();
        _positions = nullptr;
        _normals = nullptr;
        _tangents = nullptr;
        _textureUVs = nullptr;
        _indices = nullptr;
        _vertexCount = 0;
        _indiceCount = 0;
        _refCount = nullptr;

        if(_refCount)
        { ++(*_refCount); }

        return *this;
    }

    [[nodiscard]] uSys vertexCount()    const noexcept { return _vertexCount; }
    [[nodiscard]] uSys indiceCount()    const noexcept { return _indiceCount; }
    [[nodiscard]] uSys positionsCount() const noexcept { return _vertexCount * 3; }
    [[nodiscard]] uSys normalsCount()   const noexcept { return _vertexCount * 3; }
    [[nodiscard]] uSys tangentsCount()  const noexcept { return _vertexCount * 3; }
    [[nodiscard]] uSys texturesCount()  const noexcept { return _vertexCount * 2; }

    [[nodiscard]] const float* positions()  const noexcept { return _positions;  }
    [[nodiscard]] const float* normals()    const noexcept { return _normals;    }
    [[nodiscard]] const float* tangents()   const noexcept { return _tangents;   }
    [[nodiscard]] const float* textureUVs() const noexcept { return _textureUVs; }
    [[nodiscard]] const uSys*  indices()    const noexcept { return _indices;    }

    [[nodiscard]] float* positions()  noexcept { return _positions;  }
    [[nodiscard]] float* normals()    noexcept { return _normals;    }
    [[nodiscard]] float* tangents()   noexcept { return _tangents;   }
    [[nodiscard]] float* textureUVs() noexcept { return _textureUVs; }
    [[nodiscard]] uSys*  indices()    noexcept { return _indices;    }

    [[nodiscard]] bool isNull() const noexcept { return _refCount; }
    [[nodiscard]] operator bool() const noexcept { return _refCount; }

    [[nodiscard]] bool operator ==(const TauModelPart& other) const noexcept { return _refCount == other._refCount; }
    [[nodiscard]] bool operator !=(const TauModelPart& other) const noexcept { return _refCount != other._refCount; }
};

class TauModelPart::AnimationComposition final
{
    DELETE_COPY(AnimationComposition);
private:
    /**
     * A cache of the number of vertices in a TauModelPart.
     */
    uSys _numVertices;
    /**
     * The number of weights each vertex has.
     */
    uSys _weightCount;
    /**
     * The number of bones for this model part.
     */
    uSys _boneCount;
    /**
     *   The list of weights for each vertex, this is represented as a
     * contiguous stream of values with a constant block size, there
     * are {@code  _vertexCount * _numWeights @endcode} elements.
     */
    float* _weights;
    /**
     *   The list of bone indices that affect each vertex, this is
     * represented as a contiguous stream of values with a constant
     * block size, there are {@code _vertexCount * _numWeights @endcode}
     * elements.
     */
    uSys* _indices;

    /**
     *   The initial positions of each bone. This is Vector3Float
     * represented as a Vector4Float. This is because it has to be
     * passed to the shader as a uniform array which generally does
     * not have a good history of supporting Vector3Float.
     */
    float* _bonePositions;
    /**
     * The initial rotation of each bone. This a Quaternion.
     */
    float* _boneRotations;
public:
    AnimationComposition(const uSys numVertices, const uSys weightCount, const uSys boneCount) noexcept
        : _numVertices(numVertices), _weightCount(weightCount), _boneCount(boneCount),
        _weights(null), _indices(null), _bonePositions(null), _boneRotations(null)
    {
        const uSys numWeights = weightCount * numVertices;
        const uSys numBones = boneCount * 4;

        _weights = new(::std::nothrow) float[numWeights];
        _indices = new(::std::nothrow) uSys[numWeights];

        _bonePositions = new(::std::align_val_t { 64 }, ::std::nothrow) float[numBones];
        _boneRotations = new(::std::align_val_t { 64 }, ::std::nothrow) float[numBones];
    }

    ~AnimationComposition() noexcept
    {
        delete[] _weights;
        delete[] _indices;

        operator delete[](_bonePositions, ::std::align_val_t { 64 }, ::std::nothrow);
        operator delete[](_boneRotations, ::std::align_val_t { 64 }, ::std::nothrow);
    }

    [[nodiscard]] uSys weightCount() const noexcept { return _weightCount; }
    [[nodiscard]] uSys boneCount() const noexcept { return _boneCount; }
    [[nodiscard]] uSys numWeights() const noexcept { return _weightCount * _numVertices; }
    [[nodiscard]] uSys numIndices() const noexcept { return _weightCount * _numVertices; }
    [[nodiscard]] uSys bonePositionsCount() const noexcept { return _boneCount * 4; }
    [[nodiscard]] uSys boneRotationsCount() const noexcept { return _boneCount * 4; }

    [[nodiscard]] const float* weights()       const noexcept { return _weights; }
    [[nodiscard]] const uSys* indices()       const noexcept { return _indices; }
    [[nodiscard]] const float* bonePositions() const noexcept { return _bonePositions; }
    [[nodiscard]] const float* boneRotations() const noexcept { return _boneRotations; }

    [[nodiscard]] float* weights()       noexcept { return _weights; }
    [[nodiscard]] uSys* indices()       noexcept { return _indices; }
    [[nodiscard]] float* bonePositions() noexcept { return _bonePositions; }
    [[nodiscard]] float* boneRotations() noexcept { return _boneRotations; }
};
