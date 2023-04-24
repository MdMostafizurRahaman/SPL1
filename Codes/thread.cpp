#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *thread(void * th)
{
    sleep(3);
    printf("First thread use\n");
    return NULL;
}

int main()
{
    pthread_t thread1;

    printf("Before thread use\n");

    pthread_create(&thread1, NULL, thread, NULL);
    pthread_join(thread1, NULL);

    printf("After thread use\n");
}