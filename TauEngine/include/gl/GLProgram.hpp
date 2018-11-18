#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <DLL.hpp>

class TAU_DLL GLProgram
{
private:
    GLuint _programId;
public:
    GLProgram() noexcept;
    ~GLProgram() noexcept;

    inline GLuint programId() const noexcept { return _programId; }

    inline operator GLuint() const noexcept { return _programId; }
    inline GLuint operator ()() const noexcept { return _programId; }

    inline void activate() const noexcept { glUseProgram(_programId); }

    inline static void deactivate() noexcept { glUseProgram(0); }

    bool linkAndValidate() noexcept;
};
