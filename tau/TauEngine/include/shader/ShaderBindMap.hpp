#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>
#include "DLL.hpp"
#include "EShader.hpp"

class TAU_DLL ShaderBindMap final
{
    DEFAULT_CONSTRUCT_PU(ShaderBindMap);
    DEFAULT_DESTRUCT(ShaderBindMap);
    DEFAULT_COPY(ShaderBindMap);
private:
    RefDynArray<u32> _vertexUniMap;
    RefDynArray<u32> _tessCtrlUniMap;
    RefDynArray<u32> _tessEvalUniMap;
    RefDynArray<u32> _geometryUniMap;
    RefDynArray<u32> _pixelUniMap;
    RefDynArray<u32> _vertexTexMap;
    RefDynArray<u32> _tessCtrlTexMap;
    RefDynArray<u32> _tessEvalTexMap;
    RefDynArray<u32> _geometryTexMap;
    RefDynArray<u32> _pixelTexMap;
public:
    ShaderBindMap(const RefDynArray<u32>& vertexUniMap, const RefDynArray<u32>& tessCtrlUniMap, const RefDynArray<u32>& tessEvalUniMap, const RefDynArray<u32>& geometryUniMap, const RefDynArray<u32>& pixelUniMap, const RefDynArray<u32>& vertexTexMap, const RefDynArray<u32>& tessCtrlTexMap, const RefDynArray<u32>& tessEvalTexMap, const RefDynArray<u32>& geometryTexMap, const RefDynArray<u32>& pixelTexMap) noexcept
        : _vertexUniMap(vertexUniMap)
        , _tessCtrlUniMap(tessCtrlUniMap)
        , _tessEvalUniMap(tessEvalUniMap)
        , _geometryUniMap(geometryUniMap)
        , _pixelUniMap(pixelUniMap)
        , _vertexTexMap(vertexTexMap)
        , _tessCtrlTexMap(tessCtrlTexMap)
        , _tessEvalTexMap(tessEvalTexMap)
        , _geometryTexMap(geometryTexMap)
        , _pixelTexMap(pixelTexMap)
    { }

    [[nodiscard]] u32 mapUniformBindPoint(u32 index, EShader::Stage stage) noexcept;
    [[nodiscard]] u32 mapTextureBindPoint(u32 index, EShader::Stage stage) noexcept;
};
