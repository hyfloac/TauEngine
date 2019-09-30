#pragma once

#include <memory>

template<typename _T, typename _D = std::default_delete<_T>>
using Scoped = std::unique_ptr<_T, _D>;

template<typename _T>
using Ref = std::shared_ptr<_T>;

template<typename _Out, typename _In>
[[nodiscard]] Ref<_Out> RefCast(const Ref<_In>& in) noexcept
{
    return std::static_pointer_cast<_Out>(in);
}
