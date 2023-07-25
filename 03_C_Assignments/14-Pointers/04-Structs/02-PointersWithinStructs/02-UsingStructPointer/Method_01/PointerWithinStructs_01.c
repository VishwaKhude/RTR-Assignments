#include <stdio.h>
#include <stdlib.h>

struct MyData
{
    int *ptr_i;
    int i;

    float *ptr_f;
    float f;

    double *ptr_d;
    double d;

};

int main(void)
{
    //variabe declaration
    struct MyData *pData = NULL;

    //code
    printf("\n\n");
    pData = (struct MyData *)malloc(sizeof(struct MyData));
    if (pData == NULL)
    {
        printf("failed to allocate memory to 'struct MyData' !!! exitting now\n\n");
        exit(0);
    }
    else
        printf("Successfully allocated memory to 'struct MyData' !!!\n\n");

   (*pData).i = 9;
   (*pData).ptr_i = &(*pData).i;

   (*pData).f = 11.45f;
   (*pData).ptr_f = &(*pData).f;

   (*pData).d = 30.12995;
   (*pData).ptr_d = &(*pData).d;

   printf("\n\n");
   printf("i = %d\n", *((*pData).ptr_i));
   printf("address of 'i' = %p\n", (*pData).ptr_i);

   printf("\n\n");
   printf("f = %f\n", *((*pData).ptr_f));
   printf("address of 'f' = %p\n", (*pData).ptr_f);

   printf("\n\n");
   printf("d = %lf\n", *((*pData).ptr_d));
   printf("address of 'd' = %p\n", (*pData).ptr_d);

    if(pData)
    {
        free(pData);
        pData = NULL;
        printf("Memory allocated to 'struct MyData' has been successfully freed !!!\n\n");
    }
    
    return(0);
}

