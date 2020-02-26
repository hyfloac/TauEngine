#include <unordered_map>

enum class FuncType
{
    Int, Float, Bool, String
};

class FuncParam final
{
private:
    FuncType _type;
    const void* _data;
private:
    FuncParam(FuncType type, const void* data) noexcept
        : _type(type), _data(data)
    { }
public:
    [[nodiscard]] FuncParam createInt(const int* value) noexcept
    { return FuncParam(FuncType::Int, reinterpret_cast<const void*>(value)); }
    [[nodiscard]] FuncParam createFloat(const float* value) noexcept
    { return FuncParam(FuncType::Float, reinterpret_cast<const void*>(value)); }
    [[nodiscard]] FuncParam createBool(const bool* value) noexcept
    { return FuncParam(FuncType::Bool, reinterpret_cast<const void*>(value)); }
    [[nodiscard]] FuncParam createString(const char* value) noexcept
    { return FuncParam(FuncType::String, reinterpret_cast<const void*>(value)); }
public:
    [[nodiscard]] const int* getInt() const noexcept 
    {
        if(_type == FuncType::Int)
        { return reinterpret_cast<const int*>(_data); }
        return nullptr;
    }
    [[nodiscard]] const float* getFloat() const noexcept 
    {
        if(_type == FuncType::Float)
        { return reinterpret_cast<const float*>(_data); }
        return nullptr;
    }
    [[nodiscard]] const bool* getBool() const noexcept 
    {
        if(_type == FuncType::Bool)
        { return reinterpret_cast<const bool*>(_data); }
        return nullptr;
    }
    [[nodiscard]] const char* getString() const noexcept 
    {
        if(_type == FuncType::String)
        { return reinterpret_cast<const char*>(_data); }
        return nullptr;
    }

    [[nodiscard]] int getIntUnchecked() const noexcept 
    { return *reinterpret_cast<const int*>(_data); }
    [[nodiscard]] float getFloatUnchecked() const noexcept 
    { return *reinterpret_cast<const float*>(_data); }
    [[nodiscard]] bool getBoolUnchecked() const noexcept 
    { return *reinterpret_cast<const bool*>(_data); }
    [[nodiscard]] const char* getStringUnchecked() const noexcept 
    { return reinterpret_cast<const char*>(_data); }

    [[nodiscard]] bool isValid() const noexcept { return _data; }

    [[nodiscard]] FuncType type() const noexcept { return _type; }
};

typedef bool(*__cdecl func_f)(FuncParam* params, int count);

std::unordered_map<const char*, func_f> _functions;

extern void fillFunctions(std::unordered_map<const char*, func_f>&) noexcept;

extern FuncParam* extractParams(const char* paramString, int* out_count) noexcept;

void invoke(const char* command, const char* args) noexcept
{
    fillFunctions(_functions);

    if(_functions.count(command))
    {
        func_f func = _functions[command];
        int pCount;
        FuncParam* fParams = extractParams(args, &pCount);
        func(fParams, pCount);
    }
}

extern void testFunc(float x, int y, int z, bool w, const char* s) noexcept;

#define FUNC_EXECUTOR0(_FUNC_NAME) \
    bool __cdecl _FUNC_NAME##__Executor(FuncParam* params, int count) noexcept \
    { \
        if(count) { return false; } \
        _FUNC_NAME();\
        return true; \
    } 
#define FUNC_EXECUTOR1(_FUNC_NAME, _TYPE0) \
    bool __cdecl _FUNC_NAME##__Executor(FuncParam* params, int count) noexcept \
    { \
        if(count != 1) { return false; } \
        if(params[0].type() != FuncType::_TYPE0 || !params[0].isValid()) { return false; } \
        const auto var0 = params[0].get##_TYPE0##Unchecked(); \
        _FUNC_NAME(var0);\
        return true; \
    } 
#define FUNC_EXECUTOR2(_FUNC_NAME, _TYPE0, _TYPE1) \
    bool __cdecl _FUNC_NAME##__Executor(FuncParam* params, int count) noexcept \
    { \
        if(count != 2) { return false; } \
        if(params[0].type() != FuncType::_TYPE0 || \
           params[1].type() != FuncType::_TYPE1 || \
          !params[0].isValid()                  || \
          !params[1].isValid()) { return false; }  \
        const auto var0 = params[0].get##_TYPE0##Unchecked(); \
        const auto var1 = params[1].get##_TYPE1##Unchecked(); \
        _FUNC_NAME(var0, var1);\
        return true; \
    } 
#define FUNC_EXECUTOR3(_FUNC_NAME, _TYPE0, _TYPE1, _TYPE2) \
    bool __cdecl _FUNC_NAME##__Executor(FuncParam* params, int count) noexcept \
    { \
        if(count != 3) { return false; } \
        if(params[0].type() != FuncType::_TYPE0 || \
           params[1].type() != FuncType::_TYPE1 || \
           params[2].type() != FuncType::_TYPE2 || \
          !params[0].isValid()                  || \
          !params[1].isValid()                  || \
          !params[2].isValid()) { return false; }  \
        const auto var0 = params[0].get##_TYPE0##Unchecked(); \
        const auto var1 = params[1].get##_TYPE1##Unchecked(); \
        const auto var2 = params[2].get##_TYPE2##Unchecked(); \
        _FUNC_NAME(var0, var1, var2);\
        return true; \
    } 
#define FUNC_EXECUTOR4(_FUNC_NAME, _TYPE0, _TYPE1, _TYPE2, _TYPE3) \
    bool __cdecl _FUNC_NAME##__Executor(FuncParam* params, int count) noexcept \
    { \
        if(count != 4) { return false; } \
        if(params[0].type() != FuncType::_TYPE0 || \
           params[1].type() != FuncType::_TYPE1 || \
           params[2].type() != FuncType::_TYPE2 || \
           params[3].type() != FuncType::_TYPE3 || \
          !params[0].isValid()                  || \
          !params[1].isValid()                  || \
          !params[2].isValid()                  || \
          !params[3].isValid()) { return false; }  \
        const auto var0 = params[0].get##_TYPE0##Unchecked(); \
        const auto var1 = params[1].get##_TYPE1##Unchecked(); \
        const auto var2 = params[2].get##_TYPE2##Unchecked(); \
        const auto var3 = params[3].get##_TYPE3##Unchecked(); \
        _FUNC_NAME(var0, var1, var2, var3);\
        return true; \
    } 
#define FUNC_EXECUTOR5(_FUNC_NAME, _TYPE0, _TYPE1, _TYPE2, _TYPE3, _TYPE4) \
    bool __cdecl _FUNC_NAME##__Executor(FuncParam* params, int count) noexcept \
    { \
        if(count != 5) { return false; } \
        if(params[0].type() != FuncType::_TYPE0 || \
           params[1].type() != FuncType::_TYPE1 || \
           params[2].type() != FuncType::_TYPE2 || \
           params[3].type() != FuncType::_TYPE3 || \
           params[4].type() != FuncType::_TYPE4 || \
          !params[0].isValid()                  || \
          !params[1].isValid()                  || \
          !params[2].isValid()                  || \
          !params[3].isValid()                  || \
          !params[4].isValid()) { return false; }  \
        const auto var0 = params[0].get##_TYPE0##Unchecked(); \
        const auto var1 = params[1].get##_TYPE1##Unchecked(); \
        const auto var2 = params[2].get##_TYPE2##Unchecked(); \
        const auto var3 = params[3].get##_TYPE3##Unchecked(); \
        const auto var4 = params[4].get##_TYPE4##Unchecked(); \
        _FUNC_NAME(var0, var1, var2, var3, var4);\
        return true; \
    } 

FUNC_EXECUTOR5(testFunc, Float, Int, Int, Bool, String);

