#pragma once

#include <Safeties.hpp>
#include <functional>
#include "DLL.hpp"
#include "Timings.hpp"

#if defined(TAU_DEVELOPMENT) || 1
  #define TAU_ECS_EDITOR_MODE 1
#endif

#ifndef TAU_ECS_EDITOR_MODE
  #ifdef TAU_ENGINE_EDITOR
    #define TAU_ECS_EDITOR_MODE 1
  #else
    #define TAU_ECS_EDITOR_MODE 0
  #endif
#endif

class Entity;

#pragma pack(push, 1)
class TAU_DLL TAU_NOVTABLE IEntityComponent
{
    DEFAULT_DESTRUCT_VI(IEntityComponent);
    DEFAULT_CM_PO(IEntityComponent);
public:
    struct Type final
    {
        DEFAULT_DESTRUCT(Type);
        DEFAULT_CM_PU(Type);
    private:
        Type* _ptr;
    public:
        Type() noexcept
            : _ptr(this)
        { }

        [[nodiscard]] const Type* ptr() const noexcept { return _ptr; }

        [[nodiscard]] uSys asInt() const noexcept { return reinterpret_cast<uSys>(_ptr); }

        operator uSys() const noexcept { return reinterpret_cast<uSys>(_ptr); }
    };
private:
    Entity* _entity;
    u32 _isVisible : 1;
protected:
    u32 _doesUpdate : 1;
    u32 _doesRender : 1;
#if TAU_ECS_EDITOR_MODE
protected:
    u32 _isEditorVisible : 1;
    u32 _isEditorOnly : 1;
    u32 _doesEditorRender : 1;
#endif
private:
    explicit IEntityComponent(Entity* const entity) noexcept
        : _entity(entity)
        , _isVisible(true)
        , _doesUpdate(true)
        , _doesRender(true)
#if TAU_ECS_EDITOR_MODE
        , _isEditorVisible(true)
        , _isEditorOnly(false)
        , _doesEditorRender(true)
#endif
    { }

    IEntityComponent(Entity* const entity, const bool isEditorOnly) noexcept
        : _entity(entity)
        , _isVisible(true)
        , _doesUpdate(true)
        , _doesRender(true)
#if TAU_ECS_EDITOR_MODE
        , _isEditorVisible(true)
        , _isEditorOnly(isEditorOnly)
        , _doesEditorRender(true)
#endif
    { }
public:
    [[nodiscard]]       Entity* entity()       noexcept { return _entity; }
    [[nodiscard]] const Entity* entity() const noexcept { return _entity; }

    [[nodiscard]] bool isVisible() const noexcept { return _isVisible; }
    [[nodiscard]] bool doesUpdate() const noexcept { return _doesUpdate; }
    [[nodiscard]] bool doesRender() const noexcept { return _doesRender; }

    virtual bool setVisible(const bool isVisible) noexcept
    {
        _isVisible = isVisible;
        return _isVisible;
    }

    virtual void setDoesUpdate(const bool doesUpdate) noexcept
    { _doesUpdate = doesUpdate; }

    virtual void setDoesRender(const bool doesRender) noexcept
    { _doesRender = doesRender; }

    virtual void update(float fixedDelta) noexcept { }
    virtual void render(const DeltaTime& delta) noexcept { }
protected:
    void setVisibleInit(const bool isVisible) noexcept
    { _isVisible = isVisible; }
#if TAU_ECS_EDITOR_MODE
public:
    [[nodiscard]] bool isEditorVisible() const noexcept { return _isEditorVisible; }
    [[nodiscard]] bool isEditorOnly() const noexcept { return _isEditorOnly; }
    [[nodiscard]] bool doesEditorRender() const noexcept { return _doesEditorRender; }

    virtual bool setEditorVisible(const bool isEditorVisible) noexcept
    {
        _isEditorVisible = isEditorVisible;
        return _isEditorVisible;
    }

    virtual void debugRender(const DeltaTime& delta) noexcept { render(delta); }
#endif
public:
    virtual Type _getType() const noexcept = 0;

    [[nodiscard]] operator Type() const noexcept { return _getType(); }
private:
    template<typename _Derived>
    friend class AbstractEntityComponent;
};

template<typename _Derived>
class TAU_DLL NOVTABLE AbstractEntityComponent : public IEntityComponent
{
public:
    static Type _getStaticType() noexcept
    {
        static Type type;
        return type;
    }

    static _Derived* buildRaw(Entity* const entity) noexcept
    { return new(::std::nothrow) _Derived(entity); }

    static _Derived* buildPlacement(void* placement, Entity* const entity) noexcept
    { return new(placement) _Derived(entity); }

    static _Derived* build(Entity* const entity) noexcept
    { return buildRaw(entity); }

    static _Derived* build(void* placement, Entity* const entity) noexcept
    { return buildPlacement(placement, entity); }
protected:
    explicit AbstractEntityComponent(Entity* const entity)
        : IEntityComponent(entity)
    { }

    AbstractEntityComponent(Entity* const entity, const bool isEditorOnly)
        : IEntityComponent(entity, isEditorOnly)
    { }
public:
    Type _getType() const noexcept override final
    { return _getStaticType(); }
};
#pragma pack(pop)

[[nodiscard]] inline bool operator==(const IEntityComponent::Type& left, const IEntityComponent::Type& right) noexcept
{ return left.asInt() == right.asInt(); }

[[nodiscard]] inline bool operator!=(const IEntityComponent::Type& left, const IEntityComponent::Type& right) noexcept
{ return left.asInt() != right.asInt(); }

[[nodiscard]] inline bool operator>(const IEntityComponent::Type& left, const IEntityComponent::Type& right) noexcept
{ return left.asInt() > right.asInt(); }

[[nodiscard]] inline bool operator<(const IEntityComponent::Type& left, const IEntityComponent::Type& right) noexcept
{ return left.asInt() < right.asInt(); }

[[nodiscard]] inline bool operator>=(const IEntityComponent::Type& left, const IEntityComponent::Type& right) noexcept
{ return left.asInt() >= right.asInt(); }

[[nodiscard]] inline bool operator<=(const IEntityComponent::Type& left, const IEntityComponent::Type& right) noexcept
{ return left.asInt() <= right.asInt(); }

namespace std {
    struct hash<IEntityComponent::Type> final
    {
        [[nodiscard]] inline ::std::size_t operator()(const IEntityComponent::Type& type) const noexcept
        { return type.asInt(); }
    };

    struct hash<IEntityComponent*> final
    {
        [[nodiscard]] inline ::std::size_t operator()(const IEntityComponent*& entity) const noexcept
        { return entity->_getType().asInt(); }
    };

    template<typename _Derived>
    struct hash<AbstractEntityComponent<_Derived>*> final
    {
        [[nodiscard]] inline ::std::size_t operator()(const AbstractEntityComponent<_Derived>*& entity) const noexcept
        { return entity->_getType().asInt(); }
    };
}
