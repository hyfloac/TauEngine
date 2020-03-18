#include "gl/GLGraphicsAccelerator.hpp"

#ifdef _WIN32
RefDynArray<NullableRef<IGraphicsDisplay>> GLGraphicsAcceleratorNV::graphicsDisplays() noexcept
{
    return RefDynArray<NullableRef<IGraphicsDisplay>>(0);
}

RefDynArray<NullableRef<IGraphicsDisplay>> GLGraphicsAcceleratorATI::graphicsDisplays() noexcept
{
    return RefDynArray<NullableRef<IGraphicsDisplay>>(0);
}

NullableRef<GLGraphicsAcceleratorNV> GLGraphicsAcceleratorBuilderNV::build() const noexcept
{
    if(!WGLEW_NV_gpu_affinity) { return null; }
    if(!GL_NVX_gpu_memory_info) { return null; }

    /* Create temporary context with affinity to GPU */
    
    // ReSharper disable once CppLocalVariableMayBeConst
    HDC tmpHdc = wglCreateAffinityDCNV(&_gpuHandle);
    if(!tmpHdc) { return null; }

    // ReSharper disable once CppLocalVariableMayBeConst
    HGLRC rc = wglCreateContext(tmpHdc);
    if(!rc)
    {
        (void) wglDeleteDCNV(tmpHdc);
        return null;
    }
    
    (void) wglMakeCurrent(NULL, NULL);
    if(!wglMakeCurrent(tmpHdc, rc))
    {
        (void) wglDeleteContext(rc);
        (void) wglDeleteDCNV(tmpHdc);
        return null;
    }

    /* Get GPU info */

    DynString vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    DynString deviceName(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    GLint totalVideoMemory;
    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalVideoMemory);
    GLint dedicatedVideoMemory;
    glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicatedVideoMemory);
    const uSys sharedMemory = totalVideoMemory - dedicatedVideoMemory;

    /* Destroy temporary context */

    (void) wglMakeCurrent(NULL, NULL);
    (void) wglDeleteContext(rc);
    (void) wglDeleteDCNV(tmpHdc);

    return NullableRef<GLGraphicsAcceleratorNV>(DefaultTauAllocator::Instance(), vendor, deviceName, dedicatedVideoMemory, 0, sharedMemory, _gpuHandle);
}

NullableRef<GLGraphicsAcceleratorATI> GLGraphicsAcceleratorBuilderATI::build() const noexcept
{
    if(!WGLEW_AMD_gpu_association) { return null; }

    /* Create temporary context with affinity to GPU */

    // ReSharper disable once CppLocalVariableMayBeConst
    HGLRC rc = wglCreateAssociatedContextAMD(_gpuHandle);
    if(!rc) { return null; }
    
    (void) wglMakeCurrent(NULL, NULL);
    if(!wglMakeAssociatedContextCurrentAMD(rc))
    {
        (void) wglDeleteAssociatedContextAMD(rc);
        return null;
    }

    /* Get GPU info */

    DynString vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    DynString deviceName(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    GLuint totalVideoMemory;
    wglGetGPUInfoAMD(_gpuHandle, WGL_GPU_RAM_AMD, GL_UNSIGNED_INT, sizeof(GLuint), &totalVideoMemory);

    /* Destroy temporary context */

    (void) wglMakeCurrent(NULL, NULL);
    (void) wglDeleteAssociatedContextAMD(rc);

    return NullableRef<GLGraphicsAcceleratorATI>(DefaultTauAllocator::Instance(), vendor, deviceName, 0, 0, totalVideoMemory, _gpuHandle);
}
#endif
