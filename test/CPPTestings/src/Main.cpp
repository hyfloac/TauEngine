#include <cstdio>

void printSizes();
void printCasting();

int main()
{
    printSizes();
    printCasting();

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
#define PRINT_SIZE(__TYPE) printf("sizeof(" #__TYPE "): %llu\n", sizeof(__TYPE))

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
