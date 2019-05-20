    #pragma once

    #include <cstdint>
    #include <thread>
    #include <mutex>
    #include <Windows.h>

    // The various rending instructions.
    // Some have been removed for brevity.
    enum class RenderingOpcode : std::uint8_t
    {
        FINISH_RENDER = 1, // First opcode set to 1 instead of 0 to allow for faster thread locking.
        LOAD_SHADER_UNIFORM,
        ACTIVATE_SHADER_PROGRAM,
        GL_DRAW_ARRAYS,
        GL_CLEAR_BUFFERS
    };

    // The various uniform types.
    // Some have been removed for brevity.
    enum class ShaderUniformType : std::uint8_t
    {
        INTEGER,
        FLOAT,
        MAT4F
    };

    // Some simple macros to make parameter packing easier
    #define PACK_FLOAT(__F)  (reinterpret_cast<std::uint32_t&>(__F))
    #define PACK_DOUBLE(__D) (reinterpret_cast<std::uint64_t&>(__F))
    #define PACK_PTR(__P)    (reinterpret_cast<std::uint64_t>(__P))

    /**
     *  A series of parameters for an instruction.
     *
     *  Ideally I would use templates, potentially with varargs.
     */
    struct ParameterPack final
    {
        std::uint64_t p0, p1, p2, p3, p4, p5, p6, p7;

        ParameterPack(std::uint64_t _p0 = 0, std::uint64_t _p1 = 0, std::uint64_t _p2 = 0, std::uint64_t _p3 = 0, 
                      std::uint64_t _p4 = 0, std::uint64_t _p5 = 0, std::uint64_t _p6 = 0, std::uint64_t _p7 = 0) 
            : p0(_p0), p1(_p1), p2(_p2), p3(_p3), p4(_p4), p5(_p5), p6(_p6), p7(_p7)
        { }

        ~ParameterPack() = default;

        ParameterPack(const ParameterPack&) = default;
        ParameterPack(ParameterPack&&) = default;

        ParameterPack& operator =(const ParameterPack&) = default;
        ParameterPack& operator =(ParameterPack&&) = default;
    };

    class RenderingPipeline final
    {
    private:
        std::uint8_t* _instBuffer;
        std::uint32_t _insertPtr;
        std::uint32_t _instPtr;

        volatile bool _initialReady;
        volatile bool _readyForInsert;
        volatile bool _running;
        std::mutex    _mutex;
        std::thread   _renderThread;
    public:
        RenderingPipeline(HDC hdc, HGLRC renderingContext, const std::uint32_t bufferSize = 16384);
        
        ~RenderingPipeline();

        void pushInstruction(const RenderingOpcode opcode, const ParameterPack&& params);
    private:
        void runRenderingCycle();

        void renderThreadFunc(HDC hdc, HGLRC renderingContext);
    };
