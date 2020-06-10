#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include "DLL.hpp"

class IVertexBuffer;
class IIndexBuffer;
class IVertexArray;

class TAU_DLL IOcclusionQuery
{
    DEFAULT_CONSTRUCT_PO(IOcclusionQuery);
    DEFAULT_DESTRUCT_VI(IOcclusionQuery);
    DEFAULT_CM_PO(IOcclusionQuery);
protected:
    CPPRef<IVertexBuffer> _vertexBuffer;
    CPPRef<IIndexBuffer> _indexBuffer;
    NullableRef<IVertexArray> _va;
public:

};
