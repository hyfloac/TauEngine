#pragma once

#include "Event.hpp"
#include "Component.hpp"
#include <vector>
#include <string>
#include <sstream>

class Entity
{
public:
    float x, y;
public:
    Entity() 
        : x(0.0f), y(0.0f)
    { }

    virtual void onEvent(Event& event) = 0;

    virtual void onUpdate(float fixedDelta) = 0;

    virtual void onRender(float delta) = 0;
};

class ComponentEntity final : public Entity
{
public:
    std::vector<Component*> _components;
public:
    ~ComponentEntity()
    {
        for(Component* comp : _components)
        {
            delete comp;
        }
    }

    void onEvent(Event& event) override
    {
        for(Component* comp : _components)
        {
            comp->onEvent(event, this);
        }
    }

    void onUpdate(float fixedDelta) override
    {
        for(Component* comp : _components)
        {
            comp->onUpdate(fixedDelta, this);
        }
    }

    void onRender(float delta) override
    {
        for(Component* comp : _components)
        {
            comp->onRender(delta, this);
        }
    }
};

static std::string createConcreteEntity(ComponentEntity* entity, std::string name)
{
    std::stringstream ss;

    ss << "#pragma once" << std::endl;
    ss << "#include \"Entity.hpp\"" << std::endl;
    ss << "#include <cstdint>" << std::endl;

    for(auto comp : entity->_components)
    {
        ss << "#include \"" << comp->_getPath() << "\"" << std::endl;
    }

    ss << "class " << name << " : public Entity {" << std::endl;
    ss << "public:" << std::endl;

    int index = 0;
    std::uint64_t size = 0;
    for(auto comp : entity->_components)
    {
        size += comp->_getSize();
        ss << "    " << comp->_getName() << "* _comp" << index << ";" << std::endl;
        ++index;
    }

    ss << "    " << name << "() {" << std::endl;
    ss << "        std::uint8_t* rawBuf = new std::uint8_t[" << size << "];" << std::endl;
    
    index = 0;
    std::uint64_t offset = 0;
    for(auto comp : entity->_components)
    {
        ss << "        " << "_comp" << index << " = new(rawBuf + " << offset << ") " << comp->_getName() <<  ";" << std::endl;
        ++index;
        offset += comp->_getSize();
    }

    ss << "    }" << std::endl;

    ss << "    ~" << name << "() {" << std::endl;
    index = 0;
    for(auto comp : entity->_components)
    {
        ss << "        " << "_comp" << index << "->~" << comp->_getName() << "();" << std::endl;
        ++index;
    }
    ss << "        " << "delete _comp0;" << std::endl;
    ss << "    }" << std::endl;

    ss << "    void onEvent(Event& event) override {" << std::endl;
    for(std::uint64_t i = 0; i < entity->_components.size(); ++i)
    {
        ss << "        " << "_comp" << i << "->onEvent(event, this);" << std::endl;
    }
    ss << "    }" << std::endl;

    ss << "    void onUpdate(float fixedDelta) override {" << std::endl;
    for(std::uint64_t i = 0; i < entity->_components.size(); ++i)
    {
        ss << "        " << "_comp" << i << "->onUpdate(fixedDelta, this);" << std::endl;
    }
    ss << "    }" << std::endl;

    ss << "    void onRender(float delta) override {" << std::endl;
    for(std::uint64_t i = 0; i < entity->_components.size(); ++i)
    {
        ss << "        " << "_comp" << i << "->onRender(delta, this);" << std::endl;
    }
    ss << "    }" << std::endl;

    ss << "};" << std::endl;

    return ss.str();
}
