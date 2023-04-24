#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *thread(void * th)
{
    int *id = (int *)th;
    sleep(3);
    printf("First thread use:%d",*id);
    printf("\n");
    return NULL;
}

int main()
{
    pthread_t thread1;

    printf("Before thread use\n");

    for(int i = 0; i < 10; i++)
    {
        pthread_create(&thread1, NULL, thread, (void*)&thread1);
        pthread_join(thread1, NULL);
    }
    
    

    printf("After thread use\n");
}