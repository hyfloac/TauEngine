#include <cstdio>
#include <cstdlib>

#define PHASE_SHIFT (-24)

void printCasting();
void printSizes();
void printAtoI(); 
void printStrToL();
void printStrToUL();
void printAtoF(); 
void printStrToF();

int main()
{
    printCasting();
    printSizes();
    printAtoI();
    printStrToL();
    printStrToUL();
    printAtoF();
    printStrToF();

    getchar();
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

