#include <stdio.h>
#include <stdlib.h>

struct MyData
{
    int i;
    float f;
    double d;
};

int main(void)
{
    //function prototype
    void ChangeValues(struct MyData *);

    //variable declaration
    struct MyData *pData = NULL;

    //code
    printf("\n\n");

    pData = (struct MyData *)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("Failed To allocated memory to 'struct MyData'!!! exitting now\n\n");
        exit(0);
    }
    else
        printf("Successfully Allocated memory to 'struct MyData'!!!\n\n");

    //assign data values
    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.2995;

    //display
    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf\n", pData->d);

    ChangeValues(pData);

    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf\n", pData->d);

    if(pData)
    {
        free(pData);
        pData = NULL;
        printf("memory allocated to 'struct MyData' has been successfully freed !!!\n\n");
    }
    return(0);
}

void ChangeValues(struct MyData *pParam_Data)
{
    pParam_Data->i = 9;
    pParam_Data->f = 8.2f;
    pParam_Data->d = 6.1998;

}