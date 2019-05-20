#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <unordered_map>
#pragma warning(pop)

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Utils.hpp>

enum class DataType : u8
{
    Byte = 1,
    UByte,
    Short,
    UShort,
    Int,
    UInt,
    HalfFloat,
    Float,
    Double,
    Fixed,
    Int2_10_10_10_Rev,
    UInt2_10_10_10_Rev,
    UInt10F_11F_11F_Rev
};

TAU_DLL GLenum getGLType(const DataType type) noexcept;

TAU_DLL DataType getType(const GLenum type) noexcept;

class TAU_DLL VertexArray final
{
private:
    GLuint _array;
public:
    inline static void unbind() noexcept { glBindVertexArray(0); }

    static void setAttribute(GLuint index, GLuint size, DataType type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) noexcept
    { glVertexAttribPointer(index, size, getGLType(type), normalized, stride, pointer); }

    static void enableAttribute(GLuint index) noexcept { glEnableVertexAttribArray(index); }

    static void disableAttribute(GLuint index) noexcept { glDisableVertexAttribArray(index); }
public:
    VertexArray() noexcept
        : _array()
    { glGenVertexArrays(1, &_array); }

    VertexArray(const VertexArray& copy) noexcept = delete;
    VertexArray(VertexArray&& move) noexcept = delete;

    ~VertexArray() noexcept { glDeleteVertexArrays(1, &_array); }

    VertexArray& operator=(const VertexArray& copy) noexcept = delete;
    VertexArray& operator=(VertexArray&& move) noexcept = delete;

    inline GLuint array()      const noexcept { return _array; }
    inline operator GLuint()   const noexcept { return _array; }
    inline GLuint operator()() const noexcept { return _array; }

    inline void bind() const noexcept { glBindVertexArray(this->_array); }

    inline bool operator ==(const VertexArray& other) const noexcept { return _array == other._array; }
    inline bool operator !=(const VertexArray& other) const noexcept { return _array != other._array; }
};

class TAU_DLL VertexArrayShared final
{
private:
    GLuint _array;
    u32* _refCount;
public:
    inline static void unbind() noexcept { VertexArray::unbind(); }

    inline static void setAttribute(GLuint index, GLuint size, DataType type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) noexcept { VertexArray::setAttribute(index, size, type, normalized, stride, pointer); }

    inline static void  enableAttribute(GLuint index) noexcept { VertexArray::enableAttribute(index);  }

    inline static void disableAttribute(GLuint index) noexcept { VertexArray::disableAttribute(index); }
public:
    VertexArrayShared() noexcept;
    VertexArrayShared(const VertexArrayShared& copy) noexcept;
    VertexArrayShared(VertexArrayShared&& move) noexcept;

    ~VertexArrayShared() noexcept;

    VertexArrayShared& operator=(const VertexArrayShared& copy) noexcept;
    VertexArrayShared& operator=(VertexArrayShared&& move) noexcept;

    inline GLuint array()      const noexcept { return _array; }
    inline operator GLuint()   const noexcept { return _array; }
    inline GLuint operator()() const noexcept { return _array; }

    void bind() const noexcept { glBindVertexArray(this->_array); }

    inline bool operator ==(const VertexArrayShared& other) const noexcept { return _array == other._array;  }
    inline bool operator !=(const VertexArrayShared& other) const noexcept { return _array != other._array;  }
    inline bool operator ==(const VertexArray&       other) const noexcept { return _array == other.array(); }
    inline bool operator !=(const VertexArray&       other) const noexcept { return _array != other.array(); }
};

static inline bool operator ==(const VertexArray& RESTRICT left, const VertexArrayShared& RESTRICT right) noexcept { return left.array() == right.array(); }
static inline bool operator !=(const VertexArray& RESTRICT left, const VertexArrayShared& RESTRICT right) noexcept { return left.array() != right.array(); }

namespace std
{
    template<>
    struct hash<VertexArray>
    {
        inline size_t operator()(const VertexArray& arr) const noexcept
        {
            return static_cast<size_t>(arr.array());
        }
    };

    template<>
    struct hash<VertexArrayShared>
    {
        inline size_t operator()(const VertexArrayShared& arr) const noexcept
        {
            return static_cast<size_t>(arr.array());
        }
    };
}
