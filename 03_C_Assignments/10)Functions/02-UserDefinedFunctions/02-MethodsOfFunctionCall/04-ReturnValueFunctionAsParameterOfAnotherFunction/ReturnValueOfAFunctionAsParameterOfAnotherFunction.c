#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    //function prototypes
    int MyAddition(int, int);

    //variable declarations
    int r;
    int num_01, num_02;
    int num_03, num_04;

    //code
    num_01 = 10;
    num_02 = 20;
    num_03 = 30;
    num_04 = 40;

    r = MyAddition(MyAddition(num_01, num_02), MyAddition(num_03, num_04)); // return value of MyAddition() is sent as parameter to another call to MyAddition()
    
    printf("\n\n");
    printf("%d + %d + %d + %d = %d\n", num_01, num_02, num_03, num_04, r);
    printf("\n\n");

    return(0);
}

// **** Function Definition Of MyAddition() ****
int MyAddition(int a, int b) //function definition
{
    // variable declaration
    int sum;

    //code
    sum = a + b;
    return(sum);
}
