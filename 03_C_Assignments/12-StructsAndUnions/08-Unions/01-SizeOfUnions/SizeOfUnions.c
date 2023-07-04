#include <stdio.h>

struct MyStruct
{
    int i;
    float f;
    double d;
    char c;
};

union MyUnion
{
    int i;
    float f;
    double d;
    char c;
};

int main(void)
{
    //variable declarations
    struct MyStruct s;
    union MyUnion u;

    //code
    //'%lu' - unsigned long integer values was used in previous versions of VS & '%zu' for unsigned integer is used now
    printf("\n\n");
    printf("Size Of MyStruct = %zu\n", sizeof(s)); //used %zu instead of %lu for printing the value of "unsigned long"
    printf("\n\n");
    printf("Size Of MyUnion = %zu\n", sizeof(u));
    return(0);
}
