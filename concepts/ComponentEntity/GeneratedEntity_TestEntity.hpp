#pragma once
#include "Entity.hpp"
#include <cstdint>
#include "./ForwardComponent.hpp"
#include "./FloatComponent.hpp"
class TestEntity : public Entity {
public:
    ForwardComponent* _comp0;
    FloatComponent* _comp1;
    TestEntity() {
        std::uint8_t* rawBuf = new std::uint8_t[8];
        _comp0 = new(rawBuf + 0) ForwardComponent;
        _comp1 = new(rawBuf + 4) FloatComponent;
    }
    ~TestEntity() {
        _comp0->~ForwardComponent();
        _comp1->~FloatComponent();
        delete _comp0;
    }
    void onEvent(Event& event) override {
        _comp0->onEvent(event, this);
        _comp1->onEvent(event, this);
    }
    void onUpdate(float fixedDelta) override {
        _comp0->onUpdate(fixedDelta, this);
        _comp1->onUpdate(fixedDelta, this);
    }
    void onRender(float delta) override {
        _comp0->onRender(delta, this);
        _comp1->onRender(delta, this);
    }
};