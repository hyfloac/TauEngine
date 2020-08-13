#pragma once

#include <Objects.hpp>

class DeltaTime final
{
    DEFAULT_DESTRUCT(DeltaTime);
    DEFAULT_CM_PU(DeltaTime);
private:
    double _us;
    double _ms;
    double _s;
    double _usSinceLastUpdate;
    double _msSinceLastUpdate;
    double _sSinceLastUpdate;
    double _usSinceLaunch;
    double _msSinceLaunch;
    double _sSinceLaunch;
public:
    DeltaTime() noexcept;

    [[nodiscard]] double us() const noexcept { return _us; }
    [[nodiscard]] double ms() const noexcept { return _ms; }
    [[nodiscard]] double s() const noexcept { return _s; }
    [[nodiscard]] double uSeconds() const noexcept { return _us; }
    [[nodiscard]] double mSeconds() const noexcept { return _ms; }
    [[nodiscard]] double seconds() const noexcept { return _s; }
    [[nodiscard]] double microseconds() const noexcept { return _us; }
    [[nodiscard]] double milliseconds() const noexcept { return _ms; }

    [[nodiscard]] double usSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] double msSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }
    [[nodiscard]] double sSinceLastUpdate() const noexcept { return _sSinceLastUpdate; }
    [[nodiscard]] double uSecondsSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] double mSecondsSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }
    [[nodiscard]] double secondsSinceLastUpdate() const noexcept { return _sSinceLastUpdate; }
    [[nodiscard]] double microsecondsSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] double millisecondsSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }

    [[nodiscard]] double usSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] double msSinceLaunch() const noexcept { return _msSinceLaunch; }
    [[nodiscard]] double sSinceLaunch() const noexcept { return _sSinceLaunch; }
    [[nodiscard]] double uSecondsSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] double mSecondsSinceLaunch() const noexcept { return _msSinceLaunch; }
    [[nodiscard]] double secondsSinceLaunch() const noexcept { return _sSinceLaunch; }
    [[nodiscard]] double microsecondsSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] double millisecondsSinceLaunch() const noexcept { return _msSinceLaunch; }

    void setDeltaMicro(double us) noexcept;

    void onUpdate() noexcept;
};
