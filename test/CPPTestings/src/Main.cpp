#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <cassert>

#include "Testing.hpp"
#include "SmartPointer.hpp"

#define PHASE_SHIFT (-24)

void printCasting();
void printSizes();
void printAtoI(); 
void printStrToL();
void printStrToUL();
void printAtoF(); 
void printStrToF();

class PtrTest
{
public:
    PtrTest()
    {
        printf("Allocated %p.\n", this);
    }

    ~PtrTest()
    {
        printf("De-Allocated %p.\n", this);
    }

    void printPtr()
    {
        printf("%p\n", this);
    }
};

class SharedPtrTest
{
public:
    SharedPtrTest()
    {
        printf("[Shared] Allocated %p.\n", this);
    }

    ~SharedPtrTest()
    {
        printf("[Shared] De-Allocated %p.\n", this);
    }

    void printPtr()
    {
        printf("[Shared] %p\n", this);
    }
};

__declspec(dllexport) FARPROC GetProcAddress_CppImpl(HMODULE h, const char* name, std::string const& Signature)
{
    // The signature of T appears twice in the signature of T GetProcAddress<T>(HMODULE, const char*) 
    size_t len = Signature.find("@@YA");
    std::string templateParam = Signature.substr(0, len);
    std::string returnType = Signature.substr(len + 4);
    returnType.resize(templateParam.size()); // Strip off our own arguments (HMODULE and const char*)
    assert(templateParam == returnType);
    // templateParam and returnType are _pointers_ to functions (P6), so adjust to function type (Y)
    std::string funName = "?" + std::string(name) + "@@Y" + templateParam.substr(2);
    return ::GetProcAddress(h, funName.c_str());
}

__declspec(dllexport) FARPROC GetProcAddress_CppImplC0(HMODULE h, const char* name, const char* signature)
{
    const int nameLen = strlen(name);
    const int sigLen = strlen(signature);
    const char* offset = strstr(signature, "@@YA");
    const int len = static_cast<int>(offset - signature);

    char* templateParam = new char[len + 1];
    templateParam[len] = '\0';

    memcpy(templateParam, signature, len);

    char* returnType = new char[sigLen - (len + 4) + 1];
    returnType[sigLen - (len + 4)] = '\0';

    memcpy(returnType, signature + len + 4, sigLen - (len + 4));
    returnType[len] = '\0';

    assert(strcmp(templateParam, returnType) == 0);


    char* funcName = new char[1 + nameLen + 3 + len - 2 + 1];
    funcName[1 + nameLen + 3 + len - 2] = '\0';
    funcName[0] = '?';

    memcpy(funcName + 1, name, nameLen);
    funcName[1 + nameLen] = '@';
    funcName[1 + nameLen + 1] = '@';
    funcName[1 + nameLen + 2] = 'Y';
    memcpy(funcName + 1 + 3 + nameLen, templateParam + 2, len - 2);

    const FARPROC ret = ::GetProcAddress(h, funcName);

    delete[] templateParam;
    delete[] returnType;
    delete[] funcName;

    return ret;
}

__declspec(dllexport) FARPROC GetProcAddress_CppImplC(HMODULE h, const char* name, const char* signature)
{
    const int nameLen = strlen(name);
    const char* offset = strstr(signature, "@@YA");
    const int len = static_cast<int>(offset - signature);

    char* templateParam = new char[len + 1];
    templateParam[len] = '\0';

    memcpy(templateParam, signature, len);

    char* returnType = new char[len + 1];
    returnType[len] = '\0';

    memcpy(returnType, signature + len + 4, len);


    char* funcName = new char[nameLen + len + 3];
    funcName[nameLen + len + 2] = '\0';
    funcName[0] = '?';

    memcpy(funcName + 1, name, nameLen);
    funcName[1 + nameLen] = '@';
    funcName[2 + nameLen] = '@';
    funcName[3 + nameLen] = 'Y';
    memcpy(funcName + 4 + nameLen, templateParam + 2, len - 2);

    const FARPROC ret = ::GetProcAddress(h, funcName);

    delete[] templateParam;
    delete[] returnType;
    delete[] funcName;

    return ret;
}

template <typename T>
T GetProcAddressCPP(HMODULE h, const char* name)
{
    // Get our own signature. We use `const char* name` to keep it simple.
    const char* funcName = __FUNCDNAME__;
    std::string Signature = funcName + 21; // Ignore prefix "??$GetProcAddress@"
    return reinterpret_cast<T>(GetProcAddress_CppImpl(h, name, Signature));
}

template <typename T>
T GetProcAddressCPPC(HMODULE h, const char* name)
{
    // Get our own signature. We use `const char* name` to keep it simple.
    const char* funcName = __FUNCDNAME__;
    const char* signature = funcName + 22; // Ignore prefix "??$GetProcAddress@"

    return reinterpret_cast<T>(GetProcAddress_CppImplC(h, name, signature));
}

template <typename T>
T GetProcAddressCPPI(HMODULE h, const char* name)
{
    // Get our own signature. We use `const char* name` to keep it simple.
    const char* funcDName = __FUNCDNAME__;
    const char* signature = funcDName + 22; // Ignore prefix "??$GetProcAddress@"

    const size_t nameLen = strlen(name);
    const char* offset = strstr(signature, "@@YA");
    const unsigned len = static_cast<unsigned>(offset - signature);

    char* funcName = new char[nameLen + len + 3];
    funcName[nameLen + len + 2] = '\0';
    funcName[0] = '?';

    memcpy(funcName + 1, name, nameLen);
    funcName[1 + nameLen] = '@';
    funcName[2 + nameLen] = '@';
    funcName[3 + nameLen] = 'Y';
    memcpy(funcName + 4 + nameLen, signature + 2, len - 2);

    FARPROC ret = ::GetProcAddress(h, funcName);

    delete[] funcName;

    return reinterpret_cast<T>(ret);
}


// Showing the result

struct Dummy { };

__declspec(dllexport) void foo(const char* s)
{
    std::cout << s;
}

__declspec(dllexport) void foo(int i, Dummy)
{
    std::cout << "Overloaded foo(), got " << i << std::endl;
}

__declspec(dllexport) void foo(std::string const& s)
{
    std::cout << "Overloaded foo(), got " << s << std::endl;
}

__declspec(dllexport) int foo(std::map<std::string, double> volatile&)
{
    std::cout << "Overloaded foo(), complex type\n";
    return 42;
}

__declspec(dllexport) void foo(float)
{

}

__declspec(noinline) __declspec(dllexport) auto getFoo0(HMODULE h)
{
    return GetProcAddressCPPI<void(*)(int, Dummy)>(h, "foo");
}

int main8546984()
{
    HMODULE h = GetModuleHandleW(0);
    foo("Hello, ");
    auto pFoo1 = GetProcAddressCPP<void(*)(const char*)>(h, "foo");
    // This templated version of GetProcAddress is typesafe: You can't pass 
    // a float to pFoo1. That is a compile-time error.
    pFoo1("world\n");
    auto pFoo2 = getFoo0(h);
    pFoo2(42, Dummy()); // Again, typesafe.
    auto pFoo3 = GetProcAddressCPP<void(*)(std::string const&)>(h, "foo");
    pFoo3("std::string overload\n");
    auto pFoo4 = GetProcAddressCPPC<int(*)(std::map<std::string, double> volatile&)>(h, "foo");
    // pFoo4 != NULL, this overload exists.
    auto pFoo5 = GetProcAddressCPPC<void(*)(float)>(h, "foo");
    // pFoo5==NULL - no such overload.
    pFoo5(3.14159265358f);
    getchar();

    return 0;
}

int main__93745()
{
    {
        ScopedPointer<PtrTest> sp(new PtrTest);
        sp->printPtr();
    }

    printf("\n");

    {
        ScopedPointer<PtrTest[]> spa(new PtrTest[3]);
        spa[0]->printPtr();
        spa[1]->printPtr();
        spa[2]->printPtr();
    }

    printf("\n\n");

    {
        ReferencePointer<SharedPtrTest> store;
        ReferencePointer<SharedPtrTest[]> storeA;

        {
            ReferencePointer<SharedPtrTest> rp(new SharedPtrTest);
            rp->printPtr();
            store = rp;
        }

        printf("\n");

        {
            ReferencePointer<SharedPtrTest[]> rpa(new SharedPtrTest[3]);
            rpa[0]->printPtr();
            rpa[1]->printPtr();
            rpa[2]->printPtr();
            storeA = rpa;
        }
       
        printf("\n");
    }

    printf("\n\n");

    {
        ReferencePointer<SharedPtrTest> storeMove;
        ReferencePointer<SharedPtrTest[]> storeAMove;

        {
            storeMove = ReferencePointer<SharedPtrTest>(new SharedPtrTest);
        }

        printf("\n");

        {
            storeAMove = ReferencePointer<SharedPtrTest[]>(new SharedPtrTest[3]);
        }

        printf("\n");
    }

    printf("\n\n");

    getchar();
    return 0;
}

int main0()
{
    printCasting();
    printSizes();
    printAtoI();
    printStrToL();
    printStrToUL();
    printAtoF();
    printStrToF();

    getchar();
    return 0;
}

void printCasting()
{
#define PRINT_CAST(__TYPE, __NAME, __VALUE) int __NAME = __VALUE;                                              \
                                            printf("int(" #__VALUE "): %d\n", __NAME);                         \
                                            printf("long long: %lld\n", (long long) __NAME);                   \
                                            printf("unsigned long long: %llu\n", (unsigned long long) __NAME); \
                                            printf("signed long long: %lld\n", (signed long long) __NAME);     \
                                            printf("short: %d\n", (short) __NAME);                             \
                                            printf("unsigned short: %d\n", (unsigned short) __NAME);           \
                                            printf("signed short: %d\n", (signed short) __NAME);               \
                                            printf("char: %c\n", (char) __NAME);                               \
                                            printf("unsigned char: %u\n", (unsigned char) __NAME);             \
                                            printf("signed char: %d\n", (signed char) __NAME);                 \
                                            printf("\n");
                              
    printf("int\n");
    PRINT_CAST(int, a, 10);
    PRINT_CAST(int, b, 1000);
    PRINT_CAST(int, c, -1000);
    PRINT_CAST(int, d, -10);

    printf("---------------------\n");
    printf("char\n\n");

    PRINT_CAST(char, ac, 10);
    PRINT_CAST(char, bc, 1000);
    PRINT_CAST(char, cc, -1000);
    PRINT_CAST(char, dc, -10);

    printf("---------------------\n");
    printf("unsigned char\n\n");

    PRINT_CAST(unsigned char, auc, 10);
    PRINT_CAST(unsigned char, buc, 1000);
    PRINT_CAST(unsigned char, cuc, -1000);
    PRINT_CAST(unsigned char, duc, -10);

    printf("---------------------\n");
    printf("signed char\n\n");

    PRINT_CAST(signed char, asc, 10);
    PRINT_CAST(signed char, bsc, 1000);
    PRINT_CAST(signed char, csc, -1000);
    PRINT_CAST(signed char, dsc, -10);

    printf("---------------------\n");
    printf("unsigned short\n\n");

    PRINT_CAST(unsigned short, ds, 65535);

    printf("---------------------\n\n");
}

void printSizes()
{
#define PRINT_SIZE(__TYPE) printf("%-32s %llu\n", "sizeof(" #__TYPE "):", sizeof(__TYPE))

#define PRINT__US(__TYPE) PRINT_SIZE(__TYPE); PRINT_SIZE(signed __TYPE); PRINT_SIZE(unsigned __TYPE);

    printf("Basic Types\n");
    PRINT__US(char);
    PRINT__US(short);
    PRINT__US(short int);
    PRINT__US(int);
    PRINT__US(long);
    PRINT__US(long int);
    PRINT__US(long long);
    PRINT__US(long long int);
    PRINT_SIZE(float);
    PRINT_SIZE(double);
    PRINT_SIZE(long double);
    PRINT_SIZE(bool);
    PRINT_SIZE(size_t);
    PRINT_SIZE(wchar_t);

    printf("---------------------\n\n");
    printf("Pointers\n");

    PRINT__US(char*);
    PRINT__US(short*);
    PRINT__US(short int*);
    PRINT__US(int*);
    PRINT__US(long*);
    PRINT__US(long int*);
    PRINT__US(long long*);
    PRINT__US(long long int*);
    PRINT_SIZE(float*);
    PRINT_SIZE(double*);
    PRINT_SIZE(long double*);
    PRINT_SIZE(bool*);
    PRINT_SIZE(size_t*);
    PRINT_SIZE(wchar_t*);
    printf("---------------------\n\n");
}

void printAtoI()
{
#define TEST_ATOI(__NUM_STR) printf("\"%*s : %d\n", PHASE_SHIFT, __NUM_STR "\"", std::atoi(__NUM_STR));

    TEST_ATOI("1");
    TEST_ATOI("0");
    TEST_ATOI("340.846");
    TEST_ATOI("   0");
    TEST_ATOI("   346873   ");
    TEST_ATOI("   346873");
    TEST_ATOI("   346873 adg");
    printf("---------------------\n\n");
}

void printStrToL()
{
#define TEST_STRTOL(__NUM_STR) printf("\"%*s : % ld\n", PHASE_SHIFT, __NUM_STR "\"", std::strtol(__NUM_STR, nullptr, 10));

    TEST_STRTOL("1");
    TEST_STRTOL("0");
    TEST_STRTOL("-340.846");
    TEST_STRTOL("   -0");
    TEST_STRTOL("   -346873   ");
    TEST_STRTOL("   346873");
    TEST_STRTOL("   346873 adg");
    printf("---------------------\n\n");
}

void printStrToUL()
{
#define TEST_STRTOUL(__NUM_STR) printf("\"%*s : %lu\n", PHASE_SHIFT, __NUM_STR "\"", std::strtoul(__NUM_STR, nullptr, 10));

    TEST_STRTOUL("1");
    TEST_STRTOUL("0");
    TEST_STRTOUL("-340.846");
    TEST_STRTOUL("   -0");
    TEST_STRTOUL("   -346873   ");
    TEST_STRTOUL("   346873");
    TEST_STRTOUL("   346873 adg");
    printf("---------------------\n\n");
}

void printAtoF()
{
#define TEST_ATOF(__NUM_STR) printf("\"%*s : %f\n", PHASE_SHIFT, __NUM_STR "\"", std::atof(__NUM_STR));
    
    TEST_ATOF("1.0");
    TEST_ATOF("0.00001");
    TEST_ATOF("0.0000");
    TEST_ATOF("   0.3467");
    TEST_ATOF("   346873.3467967   ");
    TEST_ATOF("   346873.3467adg");
    TEST_ATOF("   346873.3467 adg");
    printf("---------------------\n\n");
}

void printStrToF()
{
#define TEST_STRTOF(__NUM_STR) printf("\"%*s : %f\n", PHASE_SHIFT, __NUM_STR "\"", std::strtof(__NUM_STR, nullptr));

    TEST_STRTOF("1.0");
    TEST_STRTOF("0.00001");
    TEST_STRTOF("0.0000");
    TEST_STRTOF("   0.3467");
    TEST_STRTOF("   346873.3467967   ");
    TEST_STRTOF("  346873.3467adg");
    TEST_STRTOF("   346873.3467 adg");
    printf("---------------------\n\n");
}

void rttiTest1();
void rttiTest2();

int main()
{
    rttiTest1();
    puts("\n\n");
    rttiTest2();
    getchar();
    return 0;
}
