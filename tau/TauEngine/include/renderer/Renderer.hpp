#pragma once

#include <Safeties.hpp>
#include <ArrayList.hpp>
#include <allocator/FixedBlockAllocator.hpp>

#include "entity/Entity.hpp"

#include <algorithm>

class IShaderProgram;
class Mesh;
class BoundingBox;
class Material;

using RenderHandle = uSys*;

class RenderGroup
{
private:
    struct EntityPair final
    {
        NullableWeakRef<Entity> entity;
        RenderHandle mapIndex;

        [[nodiscard]] inline bool operator<(const EntityPair& other) noexcept
        { return entity < other.entity; }
    };

    NullableRef<IShaderProgram> _shader;
    FixedBlockAllocator<> _renderMapAllocator;
    ArrayList<EntityPair, ALMoveMethod::MemCopy> _entities;
    bool _isDirty;
public:
    RenderGroup(const NullableRef<IShaderProgram>& shader, const uSys maxEntities) noexcept
        : _shader(shader)
        , _renderMapAllocator(sizeof(uSys*), PageAllocator::pageSize() / (maxEntities * sizeof(uSys)) + 1)
        , _entities(PageAllocator::pageSize() / (maxEntities * sizeof(EntityPair)) + 1)
        , _isDirty(false)
    { }

    RenderHandle add(const NullableWeakRef<Entity>& entity) noexcept
    {
        const RenderHandle ret = _renderMapAllocator.allocateT<uSys>();

        _entities.add({ entity, ret });

        _isDirty = true;

        return ret;
    }

    /**
     *   This function should be used with the intent of sorting
     * after removing the elements. The order of the list is
     * intentionally polluted.
     *
     * If removing only a few objects, use removeSingle.
     */
    void remove(const RenderHandle handle)
    {
        const uSys index = *handle;
        _renderMapAllocator.deallocateT<uSys>(handle);

        _entities.removeFast(index);
    }

    void removeSingle(const RenderHandle handle)
    {
        const uSys index = *handle;
        _renderMapAllocator.deallocateT<uSys>(handle);

        _entities.remove(index);
    }

    void sort() noexcept
    {
        if(!_isDirty)
        { return; }

        ::std::sort(_entities.arr(), _entities.arr() + _entities.count());

        for(uSys i = 0; i < _entities.count(); ++i)
        { *_entities[i].mapIndex = i; }

        _isDirty = false;
    }
};

class Renderer
{
    
};
