#pragma once

#include "Event.hpp"

#define COMPONENT_IMPL(_NAME) public: \
    _NAME(const _NAME& copy) = delete; \
    _NAME(_NAME&& move) = delete; \
    _NAME& operator=(const _NAME& copy) = delete; \
    _NAME& operator=(_NAME&& move) = delete; \
    const char* _getName() override { return #_NAME; } \
    const char* _getPath() override { return __FILE__; } \
    unsigned long long _getSize() override { return sizeof(_NAME); }

class Entity;

class Component
{
public:
    virtual ~Component() = default;

    virtual void onEvent(Event& event, Entity* entity) { }

    virtual void onUpdate(float fixedDelta, Entity* entity) { }

    virtual void onRender(float delta, Entity* entity) { }

    virtual const char* _getName() = 0;
    virtual const char* _getPath() = 0;
    virtual unsigned long long _getSize() = 0;
};
