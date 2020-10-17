#pragma once

#include <ds/AVLTree.hpp>
#include "DLL.hpp"
#include "EntityComponent.hpp"
#include "String.hpp"
#include <unordered_map>

class TAU_DLL TAU_NOVTABLE IEntity
{
    DEFAULT_CONSTRUCT_PO(IEntity);
    DEFAULT_DESTRUCT_VI(IEntity);
    DELETE_CM(IEntity);
public:
    template<typename _T>
    _T* getComponent() noexcept
    {
        IEntityComponent* const component = getComponent(_T::_getStaticType());
        return static_cast<_T*>(component);
    }

    virtual void update(float fixedDelta) noexcept { }
    virtual void render(const DeltaTime& delta) noexcept { }
#if TAU_ECS_EDITOR_MODE
    virtual void debugRender(const DeltaTime& delta) noexcept { }
#endif
protected:
    virtual IEntityComponent* getComponent(IEntityComponent::Type type) const noexcept = 0;
};

class TAU_DLL Entity : public IEntity
{
    DELETE_CM(Entity);
public:
    using ComponentList = AVLTree<IEntityComponent*, uSys, InsertMethod::Replace>;
private:
     ComponentList _components;
public:
    Entity(TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept
        : _components(allocator)
    { }

    ~Entity() noexcept override
    {
        _components.iterate([](ComponentList::Node* node){ delete node->value; });
    }

    template<typename _T>
    _T* addComponent() noexcept;

    template<typename _T, typename... _Args>
    _T* addComponent(_Args&&... args) noexcept;

    void update(float fixedDelta) noexcept override;
    void render(const DeltaTime& delta) noexcept override;
#if TAU_ECS_EDITOR_MODE
    void debugRender(const DeltaTime& delta) noexcept override;
#endif
protected:
    IEntityComponent* getComponent(IEntityComponent::Type type) const noexcept override;
private:
    friend class EntityManager;
};

class TAU_DLL EntityManager
{
public:
    /**
     *   A pointer to a builder for a component. This is
     * automatically supplied by AbstractEntityComponent.
     */
    typedef IEntityComponent*(* compCtor_f)(const WeakRef<Entity>& entity);

    /**
     *   A pointer to a builder for a component specifically using
     * placement new. This is automatically supplied by
     * AbstractEntityComponent.
     */
    typedef IEntityComponent*(* compPmtCtor_f)(void* placement, const WeakRef<Entity>& entity);

    struct ComponentData final
    {
        compCtor_f ctor;
        compPmtCtor_f placementCtor;
        uSys size;

        ComponentData(const compCtor_f _ctor, const compPmtCtor_f _placementCtor, const uSys _size) noexcept
            : ctor(_ctor)
            , placementCtor(_placementCtor)
            , size(_size)
        { }
    };
private:
    static ::std::unordered_map<IEntityComponent::Type, ComponentData> components;
public:
    template<typename _T>
    static void registerComponent() noexcept
    {
        const IEntityComponent::Type type = _T::_getStaticType();
        components.emplace(type, _T::buildRaw, _T::buildPlacement, sizeof(_T));
    }

    template<typename _T>
    static _T* buildComponent(Entity* const entity) noexcept
    {
        IEntityComponent* const component = buildComponent(_T::_getStaticType(), entity);
        if(!component)
        { return nullptr; }
        return static_cast<_T*>(component);
    }

    template<typename _T>
    static _T* buildComponent(void* const placement, Entity* const entity) noexcept
    {
        IEntityComponent* const component = buildComponent(_T::_getStaticType(), placement, entity);
        if(!component)
        { return nullptr; }
        return static_cast<_T*>(component);
    }

    [[nodiscard]] static IEntityComponent* buildComponent(IEntityComponent::Type type, const WeakRef<Entity>& entity) noexcept;
    [[nodiscard]] static IEntityComponent* buildComponent(IEntityComponent::Type type, void* placement, const WeakRef<Entity>& entity) noexcept;

    static DynString compileEntity(Entity* entity, const DynString& typeName) noexcept;
};


template<typename _T>
_T* Entity::addComponent() noexcept
{
    _T* const component = EntityManager::buildComponent<_T>(this);
    _components.insert(component);
    return component;
}

template<typename _T, typename ... _Args>
_T* Entity::addComponent(_Args&&... args) noexcept
{
    _T* const component = EntityManager::buildComponent<_T>(this);
    component->initialize(_TauAllocatorUtils::_forward<_Args>(args)...);
    _components.insert(component);
    return component;
}
