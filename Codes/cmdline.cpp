#include <stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int i, n, sum = 0;
    for (i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
        n = atoi(argv[i]);//atoi is used for type cast
        sum = sum + n;
    }
    printf("Sum::%d\n",sum);
    return 0;
}

