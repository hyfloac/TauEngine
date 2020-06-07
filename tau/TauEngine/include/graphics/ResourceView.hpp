#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"

#define RESOURCE_VIEW_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, IResourceView)

#define RESOURCE_VIEW_IMPL(_TYPE) RESOURCE_VIEW_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IResourceView
{
    DEFAULT_CONSTRUCT_PO(IResourceView);
    DEFAULT_DESTRUCT_VI(IResourceView);
    DEFAULT_CM_PO(IResourceView);
public:
    RTTD_BASE_IMPL(IResourceView);
    RTTD_BASE_CHECK(IResourceView);
    RTTD_BASE_CAST(IResourceView);
};
