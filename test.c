#include <stdio.h>

int ArrayMax(int *arr)
{
    int size = sizeof(*arr) / sizeof(int);
    printf("size = %d\n", size);
    int max = arr[0];
    for (int i = 0; i < size; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}
int main()
{   
    int arr[10] ={ 0, 1, 2, 3, 6, 7, 0, 2, 5  };
    printf("%d", ArrayMax(arr));
}