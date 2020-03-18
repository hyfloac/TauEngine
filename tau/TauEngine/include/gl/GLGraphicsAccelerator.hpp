#pragma once

#include "system/GraphicsAccelerator.hpp"

#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>

class TAU_DLL GLGraphicsAcceleratorNV final : public IGraphicsAccelerator
{
    DEFAULT_DESTRUCT(GLGraphicsAcceleratorNV);
    DELETE_COPY(GLGraphicsAcceleratorNV);
private:
    HGPUNV _gpuHandle;
public:
    GLGraphicsAcceleratorNV(const DynString& vendor, const DynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory, HGPUNV gpuHandle)
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory),
          _gpuHandle(gpuHandle)
    { }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return false; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsDisplay>> graphicsDisplays() noexcept override;
};

class TAU_DLL GLGraphicsAcceleratorATI final : public IGraphicsAccelerator
{
    DEFAULT_DESTRUCT(GLGraphicsAcceleratorATI);
    DELETE_COPY(GLGraphicsAcceleratorATI);
private:
    UINT _gpuHandle;
public:
    GLGraphicsAcceleratorATI(const DynString& vendor, const DynString& deviceName, const u64 videoMemory, const u64 systemMemory, const u64 sharedMemory, UINT gpuHandle)
        : IGraphicsAccelerator(vendor, deviceName, videoMemory, systemMemory, sharedMemory),
          _gpuHandle(gpuHandle)
    { }

    [[nodiscard]] bool hasTessellationShaders() noexcept override { return false; }
    [[nodiscard]] bool hasGeometryShaders() noexcept override { return false; }
    [[nodiscard]] bool has64BitFloat() noexcept override { return false; }
    [[nodiscard]] bool has64BitInt() noexcept override { return false; }
    [[nodiscard]] bool has16BitInt() noexcept override { return false; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsDisplay>> graphicsDisplays() noexcept override;
};

class TAU_DLL GLGraphicsAcceleratorBuilderNV final
{
    DEFAULT_CONSTRUCT_PU(GLGraphicsAcceleratorBuilderNV);
    DEFAULT_DESTRUCT(GLGraphicsAcceleratorBuilderNV);
    DEFAULT_COPY(GLGraphicsAcceleratorBuilderNV);
private:
    HGPUNV _gpuHandle;
public:
    void setAdapter(HGPUNV gpuHandle) noexcept { _gpuHandle = gpuHandle; }

    [[nodiscard]] NullableRef<GLGraphicsAcceleratorNV> build() const noexcept;
};

class TAU_DLL GLGraphicsAcceleratorBuilderATI final
{
    DEFAULT_CONSTRUCT_PU(GLGraphicsAcceleratorBuilderATI);
    DEFAULT_DESTRUCT(GLGraphicsAcceleratorBuilderATI);
    DEFAULT_COPY(GLGraphicsAcceleratorBuilderATI);
private:
    UINT _gpuHandle;
public:
    void setAdapter(UINT gpuHandle) noexcept { _gpuHandle = gpuHandle; }

    [[nodiscard]] NullableRef<GLGraphicsAcceleratorATI> build() const noexcept;
};
#endif
