#pragma once

// #pragma warning(push, 0)
// #include <GL/glew.h>
// #pragma warning(pop)
//
// #include <Objects.hpp>
//
// #include "model/BufferDescriptor.hpp"
// #include "model/IBuffer.hpp"

// class TAU_DLL GLBufferDescriptor final : public IBufferDescriptor
// {
//     DEFAULT_DESTRUCT(GLBufferDescriptor);
//     DELETE_COPY(GLBufferDescriptor);
// private:
//     GLuint _currAttrib;
// public:
//     static GLuint generate() noexcept;
//
//     static GLenum getGLType(const DataType type) noexcept;
//
//     // static DataType getType(const GLenum type) noexcept;
//
//     static void _bind(GLuint vao) noexcept;
//
//     static void destroy(GLuint vao) noexcept;
//
//     static void attribPointer(GLuint index, u32 size, DataType type, bool normalized, i32 stride, const void* pointer) noexcept;
// public:
//     GLBufferDescriptor(const u64 uid, const std::size_t attribCount) noexcept
//         : IBufferDescriptor(uid, attribCount),
//           _currAttrib(0)
//     { }
//
//     void addAttribute(Ref<IBuffer> buffer, u32 size, DataType type, bool normalized, i32 stride, const void* pointer) noexcept override final;
//
//     void bind(IRenderingContext& context) noexcept override final;
//
//     void unbind(IRenderingContext& context) noexcept override final;
//
//     void enableAttributes(IRenderingContext& context) noexcept override final;
//
//     void disableAttributes(IRenderingContext& context) noexcept override final;
// };
