#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include "pthread.h"

int mutex_signal = 0;
char a;
pthread_mutex_t mut;

static void read_function()
{
    while(1)
    {
        pthread_mutex_lock(&mut);
        if(mutex_signal==0)
        {
            a = 'a';
            printf("reading...");
            mutex_signal =1;
        }
        pthread_mutex_unlock(&mut);
    }
}

static void write_function()
{
    while(1)
    {
        pthread_mutex_lock(&mut);
        if(mutex_signal==1)
        {
            a = '\0';
            printf("writing...");
            mutex_signal = 0;
        }
        pthread_mutex_unlock(&mux);
    }
}

int main(int argc,char *argv[])
{
    pthread_t pthd;
    pthread_mutex_init(&mut,NULL);
    pthread_create(&pthd, NULL,(void *)&read_function, NULL);
    write_function();
    return 0;
}
