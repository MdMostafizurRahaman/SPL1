#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int a = 0;

void *thread(void * th)
{
    int *id = (int *)th;
    static int b = 0;

    a = 10;
    b = 20;
    sleep(1);

    printf("The id of thread is:%d\n", *id);
    printf("The value of a is:%d\n", a);
    printf("The value of b is:%d\n", b);

    return NULL;
}

int main()
{
    pthread_t thread1;

    printf("Before thread use\n");

    for(int i = 0; i < 3; i++)
    {
        pthread_create(&thread1, NULL, thread, (void*)&thread1);
        pthread_join(thread1, NULL);
    }
    
    

    printf("After thread use\n");
}