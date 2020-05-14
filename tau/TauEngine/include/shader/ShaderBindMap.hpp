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
    RefDynArray<i32> _vertexUniMap;
    RefDynArray<i32> _tessCtrlUniMap;
    RefDynArray<i32> _tessEvalUniMap;
    RefDynArray<i32> _geometryUniMap;
    RefDynArray<i32> _pixelUniMap;
    RefDynArray<i32> _vertexTexMap;
    RefDynArray<i32> _tessCtrlTexMap;
    RefDynArray<i32> _tessEvalTexMap;
    RefDynArray<i32> _geometryTexMap;
    RefDynArray<i32> _pixelTexMap;
public:
    ShaderBindMap(const RefDynArray<i32>& vertexUniMap, const RefDynArray<i32>& tessCtrlUniMap, const RefDynArray<i32>& tessEvalUniMap, const RefDynArray<i32>& geometryUniMap, const RefDynArray<i32>& pixelUniMap, const RefDynArray<i32>& vertexTexMap, const RefDynArray<i32>& tessCtrlTexMap, const RefDynArray<i32>& tessEvalTexMap, const RefDynArray<i32>& geometryTexMap, const RefDynArray<i32>& pixelTexMap) noexcept
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

    [[nodiscard]] i32 mapUniformBindPoint(u32 index, EShader::Stage stage) noexcept;
    [[nodiscard]] i32 mapTextureBindPoint(u32 index, EShader::Stage stage) noexcept;
};
