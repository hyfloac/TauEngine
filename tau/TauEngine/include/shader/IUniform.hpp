#pragma once

#include <Objects.hpp>
#include "model/BufferDescriptor.hpp"

template<typename _T>
class IUniform : ShaderDataType::Typed
{
    DEFAULT_CONSTRUCT_PO(IUniform);
    DEFAULT_DESTRUCT_VI(IUniform);
    DELETE_COPY(IUniform);
public:
    virtual void set(_T value) noexcept = 0;

    [[nodiscard]] virtual ShaderDataType::Type dataType() noexcept override
    { return ShaderDataType::Unknown; }
};
