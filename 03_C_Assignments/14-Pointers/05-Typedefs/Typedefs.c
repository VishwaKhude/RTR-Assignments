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
    int i_size;
    int f_size;
    int d_size;
    int struct_MyData_size;
    int pointer_to_struct_MyData_size;

    typedef struct MyData* MyDtaPtr;

    MyDtaPtr pData;

    //code
    printf("\n\n");

    pData = (MyDtaPtr)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("Failed to allocate memory to 'struct MyData' !!!\n\n");
        exit(0);
    }
    else
        printf("Successfully allocated memory to 'struct MyData' !!!\n\n");

    pData->i = 30;
    pData->f = 11.45f;
    pData->d = 1.2995;

    printf("\n\n");
    printf("Data members of 'struct MyData' are : \n\n");
    printf("i = %d\n", pData->i);
    printf("f = %f\n", pData->f);
    printf("d = %lf\n", pData->d);

    //size
    i_size = sizeof(pData->i);
    f_size = sizeof(pData->f);
    d_size = sizeof(pData->d);

    //display size
    printf("\n\n");
    printf("Sizes of data members of 'struct MyData' are : \n\n");
    printf("size of i = %d bytes\n", i_size);
    printf("size of f = %d bytes\n", f_size);
    printf("size of d = %d bytes\n", d_size);

    //entire struct size
    struct_MyData_size = sizeof(struct MyData);
    pointer_to_struct_MyData_size = sizeof(MyDtaPtr);

    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes\n\n", struct_MyData_size);
    printf("Size of pointer to 'struct MyData' : %d bytes\n\n", pointer_to_struct_MyData_size);

    if(pData)
    {
        free(pData);
        pData = NULL;
        printf("Memory allocated to 'struct MyData' has been successfully freed !!!\n\n");
    }
    
    return(0);
}

