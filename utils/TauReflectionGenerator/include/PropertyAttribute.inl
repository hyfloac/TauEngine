#pragma once

#if defined(IN_DEV) || 0
  #include "PropertyAttribute.hpp"
#endif

[[nodiscard]] inline bool operator==(const ::tau::PropertyAttributeData& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left.attribName() == right.attribName(); }

[[nodiscard]] inline bool operator!=(const ::tau::PropertyAttributeData& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left.attribName() != right.attribName(); }

[[nodiscard]] inline bool operator>(const ::tau::PropertyAttributeData& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left.attribName() > right.attribName(); }

[[nodiscard]] inline bool operator<(const ::tau::PropertyAttributeData& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left.attribName() < right.attribName(); }

[[nodiscard]] inline bool operator>=(const ::tau::PropertyAttributeData& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left.attribName() >= right.attribName(); }

[[nodiscard]] inline bool operator<=(const ::tau::PropertyAttributeData& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left.attribName() <= right.attribName(); }

[[nodiscard]] inline bool operator==(const ::tau::PropertyAttributeData& left, const DynString& right) noexcept
{ return left.attribName() == right; }

[[nodiscard]] inline bool operator!=(const ::tau::PropertyAttributeData& left, const DynString& right) noexcept
{ return left.attribName() != right; }

[[nodiscard]] inline bool operator>(const ::tau::PropertyAttributeData& left, const DynString& right) noexcept
{ return left.attribName() > right; }

[[nodiscard]] inline bool operator<(const ::tau::PropertyAttributeData& left, const DynString& right) noexcept
{ return left.attribName() < right; }

[[nodiscard]] inline bool operator>=(const ::tau::PropertyAttributeData& left, const DynString& right) noexcept
{ return left.attribName() >= right; }

[[nodiscard]] inline bool operator<=(const ::tau::PropertyAttributeData& left, const DynString& right) noexcept
{ return left.attribName() <= right; }

[[nodiscard]] inline bool operator==(const DynString& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left == right.attribName(); }

[[nodiscard]] inline bool operator!=(const DynString& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left != right.attribName(); }

[[nodiscard]] inline bool operator>(const DynString& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left > right.attribName(); }

[[nodiscard]] inline bool operator<(const DynString& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left < right.attribName(); }

[[nodiscard]] inline bool operator>=(const DynString& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left >= right.attribName(); }

[[nodiscard]] inline bool operator<=(const DynString& left, const ::tau::PropertyAttributeData& right) noexcept
{ return left <= right.attribName(); }
