#pragma once

#define _RTTI_ENUM_NAME rtti___enum_rtti_types
#define _RTTI_GET_NAME(_BASE) rtti___get_rtti_type_##_BASE

#define RTTI_TYPES(...)        public: enum class _RTTI_ENUM_NAME { __VA_ARGS__ };
#define RTTI_BASE(_TYPE)       public: virtual _TYPE::_RTTI_ENUM_NAME _RTTI_GET_NAME(_TYPE)() const = 0;
#define RTTI_SUB(_BASE, _TYPE) public: _BASE::_RTTI_ENUM_NAME _RTTI_GET_NAME(_BASE)() const override { return _BASE::_RTTI_ENUM_NAME::_TYPE; }

#define  IS_TYPE(_BASE_TYPE, _CAST_TO, _PTR) ((_PTR) && ((_PTR)->_RTTI_GET_NAME(_BASE_TYPE)() == _BASE_TYPE::_RTTI_ENUM_NAME::_CAST_TO))
#define DYN_CAST(_BASE_TYPE, _CAST_TO, _PTR) (IS_TYPE(_BASE_TYPE, _CAST_TO, _PTR) ? reinterpret_cast<_CAST_TO*>(_PTR) : nullptr)
