#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10

struct Employee
{
    char name[NAME_LENGTH];
    int age;
    char sex;
    float salary;
    char marital_status;
};

int main(void)
{
    //function prototype
    void MyGetString(char[], int);

    //variable declaration
    struct Employee *pEmployeeRecord = NULL;
    int num_employees, i;

    //code
    printf("\n\n");
    printf("Enter Number of Employess whose Details You want to record :");
    scanf("%d", &num_employees);

    printf("\n\n");
    pEmployeeRecord = (struct Employee *)malloc(sizeof(struct Employee) * num_employees);
    if (pEmployeeRecord == NULL)
    {
        printf("Failed to allocate memory for %d employess !!! exitting now\n\n", num_employees);
        exit(0);
    }
    else
        printf("Successfully allocated memory for %d employees !!!\n\n", num_employees);

    //user input initialization of array of 'struct Employees'
    for (i = 0; i < num_employees; i++)
    {
        printf("\n\n\n\n");
        printf("***Data Entry For Employee Number %d***\n", (i + 1));

        printf("\n\n");
        printf("Enter Employee Name :");
        MyGetString(pEmployeeRecord[i].name, NAME_LENGTH);

        printf("\n\n\n");
        printf("Enter Employee's Age (in years):");
        scanf("%d", &pEmployeeRecord[i].age);

        printf("\n\n");
        printf("Enter Employee's Sex (M/m for male, F/f for female) :");
        pEmployeeRecord[i].sex = getch();
        printf("%c", pEmployeeRecord[i].sex);
        pEmployeeRecord[i].sex = toupper(pEmployeeRecord[i].sex);

        printf("\n\n\n");
        printf("Enter Employee's Salary (in indian rupees) :");
        scanf("%f", &pEmployeeRecord[i].salary);

        printf("\n\n");
        printf("Is the Employee maried? (Y/y for yes, N/n for no) :");
        pEmployeeRecord[i].marital_status = getch();
        printf("%c", pEmployeeRecord[i].marital_status);
        pEmployeeRecord[i].marital_status = toupper(pEmployeeRecord[i].marital_status);
    }

    //display
    printf("\n\n\n\n");
    printf("***Displaying employee records****\n\n");
    for (i = 0; i < num_employees; i++)
    {
        printf("***Employee Number %d***\n\n", (i +1));
        printf("Name    : %s\n", pEmployeeRecord[i].name);
        printf("Age    : %d years\n", pEmployeeRecord[i].age);

        if (pEmployeeRecord[i].sex == 'M')
            printf("Sex : Male\n");
        else if(pEmployeeRecord[i].sex == 'F')
            printf("Sex : Female\n");
        else
            printf("Sex : Invalid Data Entered\n");

        printf("Salary  : Rs. %f\n", pEmployeeRecord[i].salary);

        if (pEmployeeRecord[i].marital_status == 'Y')
            printf("Marital Status : Married\n");
        else if(pEmployeeRecord[i].marital_status == 'N')
            printf("Marital Status  : Unmarried\n");
        else
            printf("Marital Status  : Invalid Data Entered\n");
        
        printf("\n\n");    
    }

    if (pEmployeeRecord)
    {
        free(pEmployeeRecord);
        pEmployeeRecord = NULL;
        printf("Memory allocated to %d employees has been succcessfully freed !!!\n\n", num_employees);
    }
    
    return(0);
}

void MyGetString(char str[], int str_size)
{
    int i;
    char ch = '\0';

    i = 0;
    do
    {
        ch = getch();
        str[i] = ch;
        printf("%c", str[i]);
        i++;
    } while ((ch != '\r') && (i < str_size));

    if (i == str_size)
        str[i - 1] = '\0';
    else
        str[i] = '\0';
}

