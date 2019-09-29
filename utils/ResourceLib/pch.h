#ifndef PCH_H
#define PCH_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <cinttypes>
#include <climits>
#include <limits>
#include <cfloat>

#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <utility>
#include <chrono>
#include <memory>

#include <NumTypes.hpp>
#include <MathDefines.hpp>
#include <Utils.hpp>
#include <Template.hpp>
#include <Safeties.hpp>
#include <VariableLengthArray.hpp>
#include <Alignment.h>
#include <DynArray.hpp>
#include <EnumBitFields.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

template<typename _T, typename _D = std::default_delete<_T>>
using Scoped = std::unique_ptr<_T, _D>;

template<typename _T>
using Ref = std::shared_ptr<_T>;

template<typename _Out, typename _In>
[[nodiscard]] Ref<_Out> RefCast(const Ref<_In>& in) noexcept
{ return std::static_pointer_cast<_Out>(in); }

#endif
