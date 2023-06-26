#include <stdio.h> //'stdio.h' contains declaration of 'printf()' 

// **** USER DEFINED FUNCTIONS : METHOD OF CALLING FUNCTOION 2 ****
// **** CALLING ONLY TWO FUNCTIONS DIRECTLY IN main(), REST OF THE FUNCTIONS TRACE THEIR CALL INDIRECTLY TO main() ****

int main(int argc, char *argv[], char *envp[])
{
    //function prototypes
    void display_information(void);
    void Function_Country(void);

    //code
    display_information(); //function call
    Function_Country(); //function call

    return(0);
}

// **** User-Defined Functions Definitions... ****
void display_information(void) //function definition
{
    //function prototypes
    void Function_My(void);
    void Function_Name(void);
    void Function_Is(void);
    void Function_Vishwa(void);
    void Function_Mahesh(void);
    void Function_Khude(void);
    void Function_OfAMC(void);

    //code

    // *** FUNCTION CALLS ***
    Function_My();
    Function_Name();
    Function_Is();
    Function_Vishwa();
    Function_Mahesh();
    Function_Khude();
    Function_OfAMC();
}

void Function_My(void) //function definition
{
    //code
    printf("\n\n");

    printf("My");
}

void Function_Name(void) //function definiton
{
    //code
    printf("\n\n");

    printf("Name");
}

void Function_Is(void) //function defintion
{
    //code
    printf("\n\n");

    printf("Is");
}

void Function_Vishwa(void) //function definition
{
    //code
    printf("\n\n");

    printf("Vishwa");
}

void Function_Mahesh(void) //function definition
{
    //code
    printf("\n\n");

    printf("Mahesh");
}

void Function_Khude(void) //function definition
{
    //code
    printf("\n\n");

    printf("Khude");
}

void Function_OfAMC(void) //function definition
{
    //code
    printf("\n\n");
    
    printf("Of ASTROMEDICOMP");
}

void Function_Country(void) //function definition
{
    printf("\n\n");

    printf("I Live In India.");

    printf("\n\n");
}
