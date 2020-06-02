#include <cstdio>
#include <cinttypes>
#include <cstdint>
#include <cstddef>
#include <cstring>

class IPrinter
{
protected:
    IPrinter() = default;
public:
    virtual ~IPrinter() = default;

    IPrinter(const IPrinter& copy) = delete;
    IPrinter(IPrinter&& move) = delete;

    IPrinter& operator=(const IPrinter& copy) = delete;
    IPrinter& operator=(IPrinter&& move) = delete;

    virtual void print(const char c) = 0;
    virtual void print(const char* str) = 0;
    virtual void print(const int32_t x) = 0;
    virtual void print(const uint32_t x) = 0;
    virtual void print(const int64_t x) = 0;
    virtual void print(const uint64_t x) = 0;
    virtual void print(const float x) = 0;
    virtual void print(const double x) = 0;

    template<typename _CurrArg, typename... _Args>
    void print(const char* fmt, _CurrArg currArg, _Args... args)
    {
        for(size_t i = 0; fmt[i]; ++i)
        {
            if(fmt[i] == '%')
            {
                print(currArg);
                print(fmt, args...);
            }
            else
            {
                print(fmt[i]);
            }
        }
    }
};

class STDPrinter final : public IPrinter
{
private:
    FILE* _file;
public:
    STDPrinter(FILE* const file)
        : _file(file)
    { }

    ~STDPrinter() = default;

    STDPrinter(const STDPrinter& copy) = delete;
    STDPrinter(STDPrinter&& move) = delete;

    STDPrinter& operator=(const STDPrinter& copy) = delete;
    STDPrinter& operator=(STDPrinter&& move) = delete;

    void print(const char c) override
    { fputc(c, _file); }

    void print(const char* str) override
    { fputs(str, _file); }

    void print(const int32_t x) override
    { fprintf(_file, "%" PRId32, x); }

    void print(const uint32_t x) override
    { fprintf(_file, "%" PRIu32, x); }

    void print(const int64_t x) override
    { fprintf(_file, "%" PRId64, x); }

    void print(const uint64_t x) override
    { fprintf(_file, "%" PRIu64, x); }

    void print(const float x) override
    { fprintf(_file, "%f", x); }

    void print(const double x) override
    { fprintf(_file, "%f", x); }
};

class NullPrinter final : public IPrinter
{
public:
    NullPrinter() = default;

    ~NullPrinter() = default;

    NullPrinter(const NullPrinter& copy) = delete;
    NullPrinter(NullPrinter&& move) = delete;

    NullPrinter& operator=(const NullPrinter& copy) = delete;
    NullPrinter& operator=(NullPrinter&& move) = delete;

    void print(const char c) override { }
    void print(const char* str) override { }
    void print(const int32_t x) override { }
    void print(const uint32_t x) override { }
    void print(const int64_t x) override { }
    void print(const uint64_t x) override { }
    void print(const float x) override { }
    void print(const double x) override { }
};

class Child final
{
private:
    IPrinter& _printer;
public:
    Child(IPrinter& printer)
        : _printer(printer)
    { }

    void test() const
    {
        _printer.print("Test: (%, %)\n", 36, 17.3975f);
    }
};

class Parent final
{
private:
    IPrinter* _printer;
    Child _child;
public:
    Parent(const bool shouldPrint)
        : _child(*_printer)
    {
        if(shouldPrint)
        {
            _printer = new STDPrinter(stdout);
        }
        else
        {
            _printer = new NullPrinter;
        }
    }

    Child& getChild() { return _child; }
    const Child& getChild() const { return _child; }
};

int main(int argCount, char* args[])
{
    static const char STR_SHOULD_PRINT[] = "shouldPrint=";
    bool shouldPrint = false;

    for(size_t i = 1; i < argCount; ++i)
    {
        if(strncmp(STR_SHOULD_PRINT, args[i], strlen(STR_SHOULD_PRINT)) == 0)
        {
            const char* val = args[i] + strlen(STR_SHOULD_PRINT);
            if(strcmp(val, "0") == 0)
            {
                shouldPrint = false;
            }
            else if(strcmp(val, "1") == 1) 
            {
                shouldPrint = true;
            }
        }
    }

    Parent parent(shouldPrint);

    parent.getChild().test();

    return 0;
}
