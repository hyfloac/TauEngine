#pragma once

#include <Objects.hpp>

class DeltaTime final
{
    DEFAULT_DESTRUCT(DeltaTime);
    DEFAULT_CM_PU(DeltaTime);
private:
    float _us;
    float _ms;
    float _s;
    float _usSinceLastUpdate;
    float _msSinceLastUpdate;
    float _sSinceLastUpdate;
    float _usSinceLaunch;
    float _msSinceLaunch;
    float _sSinceLaunch;
public:
    DeltaTime() noexcept;

    [[nodiscard]] float us() const noexcept { return _us; }
    [[nodiscard]] float ms() const noexcept { return _ms; }
    [[nodiscard]] float s() const noexcept { return _s; }
    [[nodiscard]] float uSeconds() const noexcept { return _us; }
    [[nodiscard]] float mSeconds() const noexcept { return _ms; }
    [[nodiscard]] float seconds() const noexcept { return _s; }
    [[nodiscard]] float microseconds() const noexcept { return _us; }
    [[nodiscard]] float milliseconds() const noexcept { return _ms; }

    [[nodiscard]] float usSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] float msSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }
    [[nodiscard]] float sSinceLastUpdate() const noexcept { return _sSinceLastUpdate; }
    [[nodiscard]] float uSecondsSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] float mSecondsSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }
    [[nodiscard]] float secondsSinceLastUpdate() const noexcept { return _sSinceLastUpdate; }
    [[nodiscard]] float microsecondsSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] float millisecondsSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }

    [[nodiscard]] float usSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] float msSinceLaunch() const noexcept { return _msSinceLaunch; }
    [[nodiscard]] float sSinceLaunch() const noexcept { return _sSinceLaunch; }
    [[nodiscard]] float uSecondsSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] float mSecondsSinceLaunch() const noexcept { return _msSinceLaunch; }
    [[nodiscard]] float secondsSinceLaunch() const noexcept { return _sSinceLaunch; }
    [[nodiscard]] float microsecondsSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] float millisecondsSinceLaunch() const noexcept { return _msSinceLaunch; }

    void setDeltaMicro(float us) noexcept;

    void onUpdate() noexcept;
};
