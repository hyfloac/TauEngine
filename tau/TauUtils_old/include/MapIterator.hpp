#pragma once

#include <map>
#include <unordered_map>

#include "Objects.hpp"

template<typename _Key, typename _Value, class _Pr = ::std::less<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class MapKeyIterator : public ::std::map<_Key, _Value, _Pr, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(MapKeyIterator);
    DEFAULT_DESTRUCT(MapKeyIterator);
    DEFAULT_CM_PU(MapKeyIterator);
public:
    using MapIterator = typename ::std::map<_Key, _Value, _Pr, _Alloc>::iterator;
public:
    MapKeyIterator(const MapIterator& iterator) noexcept
        : MapIterator(iterator)
    { }

    [[nodiscard]]       _Key* operator->()       noexcept { return &(MapIterator::operator->()->first); }
    [[nodiscard]] const _Key& operator *() const noexcept { return MapIterator::operator*().first; }
};

template<typename _Key, typename _Value, class _Pr = ::std::less<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class MapConstKeyIterator : public ::std::map<_Key, _Value, _Pr, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(MapConstKeyIterator);
    DEFAULT_DESTRUCT(MapConstKeyIterator);
    DEFAULT_CM_PU(MapConstKeyIterator);
public:
    using MapIterator = typename ::std::map<_Key, _Value, _Pr, _Alloc>::iterator;
public:
    MapConstKeyIterator(const MapIterator& iterator) noexcept
        : MapIterator(iterator)
    { }

    [[nodiscard]] const _Key* operator->() const noexcept { return &(MapIterator::operator->()->first); }
    [[nodiscard]] const _Key& operator *() const noexcept { return MapIterator::operator*().first; }
};

template<typename _Key, typename _Value, class _Hasher = ::std::hash<_Key>, class _KeyEq = ::std::equal_to<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class HashMapKeyIterator : public ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(HashMapKeyIterator);
    DEFAULT_DESTRUCT(HashMapKeyIterator);
    DEFAULT_CM_PU(HashMapKeyIterator);
public:
    using HashMapIterator = typename ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator;
public:
    HashMapKeyIterator(const HashMapIterator& iterator) noexcept
        : HashMapIterator(iterator)
    { }

    [[nodiscard]]       _Key* operator->()       noexcept { return &(HashMapIterator::operator->()->first); }
    [[nodiscard]] const _Key& operator *() const noexcept { return HashMapIterator::operator*().first; }
};

template<typename _Key, typename _Value, class _Hasher = ::std::hash<_Key>, class _KeyEq = ::std::equal_to<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class HashMapConstKeyIterator : public ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(HashMapConstKeyIterator);
    DEFAULT_DESTRUCT(HashMapConstKeyIterator);
    DEFAULT_CM_PU(HashMapConstKeyIterator);
public:
    using HashMapIterator = typename ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator;
public:
    HashMapConstKeyIterator(const HashMapIterator& iterator) noexcept
        : HashMapIterator(iterator)
    { }

    [[nodiscard]] const _Key* operator->() const noexcept { return &(HashMapIterator::operator->()->first); }
    [[nodiscard]] const _Key& operator *() const noexcept { return HashMapIterator::operator*().first; }
};

template<typename _Key, typename _Value, class _Pr = ::std::less<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class MapValueIterator : public ::std::map<_Key, _Value, _Pr, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(MapValueIterator);
    DEFAULT_DESTRUCT(MapValueIterator);
    DEFAULT_CM_PU(MapValueIterator);
public:
    using MapIterator = typename ::std::map<_Key, _Value, _Pr, _Alloc>::iterator;
public:
    MapValueIterator(const MapIterator& iterator) noexcept
        : MapIterator(iterator)
    { }

    [[nodiscard]]       _Value* operator->()       noexcept { return &(MapIterator::operator->()->second); }
    [[nodiscard]] const _Value& operator *() const noexcept { return MapIterator::operator*().second; }
};

template<typename _Key, typename _Value, class _Pr = ::std::less<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class MapConstValueIterator : public ::std::map<_Key, _Value, _Pr, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(MapConstValueIterator);
    DEFAULT_DESTRUCT(MapConstValueIterator);
    DEFAULT_CM_PU(MapConstValueIterator);
public:
    using MapIterator = typename ::std::map<_Key, _Value, _Pr, _Alloc>::iterator;
public:
    MapConstValueIterator(const MapIterator& iterator) noexcept
        : MapIterator(iterator)
    { }

    [[nodiscard]] const _Value* operator->() const noexcept { return &(MapIterator::operator->()->second); }
    [[nodiscard]] const _Value& operator *() const noexcept { return MapIterator::operator*().second; }
};

template<typename _Key, typename _Value, class _Hasher = ::std::hash<_Key>, class _KeyEq = ::std::equal_to<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class HashMapValueIterator : public ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(HashMapValueIterator);
    DEFAULT_DESTRUCT(HashMapValueIterator);
    DEFAULT_CM_PU(HashMapValueIterator);
public:
    using HashMapIterator = typename ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator;
public:
    HashMapValueIterator(const HashMapIterator& iterator) noexcept
        : HashMapIterator(iterator)
    { }

    [[nodiscard]]       _Value* operator->()       noexcept { return &(HashMapIterator::operator->()->second); }
    [[nodiscard]] const _Value& operator *() const noexcept { return HashMapIterator::operator*().second; }
};

template<typename _Key, typename _Value, class _Hasher = ::std::hash<_Key>, class _KeyEq = ::std::equal_to<_Key>, class _Alloc = ::std::allocator<::std::pair<const _Key, _Value>>>
class HashMapConstValueIterator : public ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator
{
    DEFAULT_CONSTRUCT_PU(HashMapConstValueIterator);
    DEFAULT_DESTRUCT(HashMapConstValueIterator);
    DEFAULT_CM_PU(HashMapConstValueIterator);
public:
    using HashMapIterator = typename ::std::unordered_map<_Key, _Value, _Hasher, _KeyEq, _Alloc>::iterator;
public:
    HashMapConstValueIterator(const HashMapIterator& iterator) noexcept
        : HashMapIterator(iterator)
    { }

    [[nodiscard]] const _Value* operator->() const noexcept { return &(HashMapIterator::operator->()->second); }
    [[nodiscard]] const _Value& operator *() const noexcept { return HashMapIterator::operator*().second; }
};

template<typename _Iter>
struct IteratorBundle final
{
    DEFAULT_CONSTRUCT_PU(IteratorBundle);
    DEFAULT_DESTRUCT(IteratorBundle);
    DEFAULT_CM_PU(IteratorBundle);
public:
    _Iter _begin;
    _Iter _end;
public:
    IteratorBundle(const _Iter& begin, const _Iter& end) noexcept
        : _begin(begin)
        , _end(end)
    { }

    [[nodiscard]] _Iter begin() const noexcept { return _begin; }
    [[nodiscard]] _Iter   end() const noexcept { return _end;   }
};
