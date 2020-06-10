#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "ResourceEnums.hpp"

class IRenderingContext;

struct ResourceMapRange final
{
    DEFAULT_DESTRUCT(ResourceMapRange);
    DEFAULT_CM_PO(ResourceMapRange);
public:
    static const ResourceMapRange* all() noexcept
    { return null; }

    static const ResourceMapRange* none() noexcept
    {
        static ResourceMapRange _none(0, 0);
        return &_none;
    }
public:
    uSys begin;
    uSys end;
public:
    ResourceMapRange(const uSys _begin, const uSys _end) noexcept
        : begin(_begin)
        , end(_end)
    { }
};

#define RESOURCE_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IResource)

#define RESOURCE_IMPL(_TYPE) RESOURCE_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IResource
{
    DEFAULT_DESTRUCT_VI(IResource);
    DEFAULT_CM_PO(IResource);
protected:
    uSys _size;
protected:
    IResource(const uSys size) noexcept
        : _size(size)
    { }
public:
    [[nodiscard]] uSys size() const noexcept { return _size; }

    [[nodiscard]] virtual EResource::Type resourceType() const noexcept = 0;

    [[nodiscard]] virtual void* map(IRenderingContext& context, const ResourceMapRange* mapReadRange) noexcept = 0;
    virtual void unmap(IRenderingContext& context) noexcept = 0;

    RTT_BASE_IMPL(IResource);
    RTT_BASE_CHECK(IResource);
    RTT_BASE_CAST(IResource);
};
