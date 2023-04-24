#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t locked;
int a;

void *processFunction(void *th)
{
    pthread_mutex_lock(&locked);
    int i = 0;
    a++;

    while(i < 6)
    {
        printf("%d\n",i);
        sleep(2);
        i++;
    }
    printf("Loop finished\n");

    pthread_mutex_unlock(&locked);
    return 0;
}


int main()
{
    pthread_t thread1, thread2;

    if(pthread_mutex_init(&locked, NULL) != 0)
    {
        printf("Mutex was created unsuccessfully\n");
        exit(0);
    }
    a = 0;
    pthread_create(&thread1, NULL, processFunction, NULL);
    pthread_create(&thread2, NULL, processFunction, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    
    printf("After thread use\n");
    //return 0;
}