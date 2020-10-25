#pragma once

#if defined(IN_DEV) || 0
  #include "Attribute.hpp"
#endif

[[nodiscard]] inline bool operator==(const ::tau::AttributeData& left, const ::tau::AttributeData& right) noexcept
{ return left.attribName() == right.attribName(); }

[[nodiscard]] inline bool operator!=(const ::tau::AttributeData& left, const ::tau::AttributeData& right) noexcept
{ return left.attribName() != right.attribName(); }

[[nodiscard]] inline bool operator>(const ::tau::AttributeData& left, const ::tau::AttributeData& right) noexcept
{ return left.attribName() > right.attribName(); }

[[nodiscard]] inline bool operator<(const ::tau::AttributeData& left, const ::tau::AttributeData& right) noexcept
{ return left.attribName() < right.attribName(); }

[[nodiscard]] inline bool operator>=(const ::tau::AttributeData& left, const ::tau::AttributeData& right) noexcept
{ return left.attribName() >= right.attribName(); }

[[nodiscard]] inline bool operator<=(const ::tau::AttributeData& left, const ::tau::AttributeData& right) noexcept
{ return left.attribName() <= right.attribName(); }

[[nodiscard]] inline bool operator==(const ::tau::AttributeData& left, const DynString& right) noexcept
{ return left.attribName() == right; }

[[nodiscard]] inline bool operator!=(const ::tau::AttributeData& left, const DynString& right) noexcept
{ return left.attribName() != right; }

[[nodiscard]] inline bool operator>(const ::tau::AttributeData& left, const DynString& right) noexcept
{ return left.attribName() > right; }

[[nodiscard]] inline bool operator<(const ::tau::AttributeData& left, const DynString& right) noexcept
{ return left.attribName() < right; }

[[nodiscard]] inline bool operator>=(const ::tau::AttributeData& left, const DynString& right) noexcept
{ return left.attribName() >= right; }

[[nodiscard]] inline bool operator<=(const ::tau::AttributeData& left, const DynString& right) noexcept
{ return left.attribName() <= right; }

[[nodiscard]] inline bool operator==(const DynString& left, const ::tau::AttributeData& right) noexcept
{ return left == right.attribName(); }

[[nodiscard]] inline bool operator!=(const DynString& left, const ::tau::AttributeData& right) noexcept
{ return left != right.attribName(); }

[[nodiscard]] inline bool operator>(const DynString& left, const ::tau::AttributeData& right) noexcept
{ return left > right.attribName(); }

[[nodiscard]] inline bool operator<(const DynString& left, const ::tau::AttributeData& right) noexcept
{ return left < right.attribName(); }

[[nodiscard]] inline bool operator>=(const DynString& left, const ::tau::AttributeData& right) noexcept
{ return left >= right.attribName(); }

[[nodiscard]] inline bool operator<=(const DynString& left, const ::tau::AttributeData& right) noexcept
{ return left <= right.attribName(); }
