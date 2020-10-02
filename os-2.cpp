#include<iostream>
#include<cstdio>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<cstring>
#include <time.h>
#include <stdlib.h> 
using namespace std;

#define cycleNumber 10
#define servicemenNumber 3
#define paymentRoom 1
pthread_mutex_t mutex[servicemenNumber];
pthread_mutex_t entry;
sem_t sem[servicemenNumber];
sem_t payment_room_capacity;

void payment(void* arg);
void* Servicing(void* arg);
void Exit(void* arg);

int main()
{
    int res;

    for(int i=0;i<servicemenNumber;i++)
    {
        res = sem_init(&sem[i],0,1);
        if(res != 0)
        {
            printf("Failed\n");
        }   
    }
      res = sem_init(&payment_room_capacity,0,paymentRoom);
    {
        if(res != 0)
        {
            printf("Failed\n");
        }  
    } 

    for(int i=0;i<servicemenNumber;i++)
    {
        res = pthread_mutex_init(&mutex[i],NULL);
        if(res != 0)
        {
            printf("Failed\n");
        }  
    }

   
     res = pthread_mutex_init(&entry,NULL);
        if(res != 0)
        {
            printf("Failed\n");
        }    

    pthread_t cycles[cycleNumber];
    for(int i = 0; i < cycleNumber; i++){
        char *id = new char[3];
        strcpy(id,to_string(i+1).c_str());

        res = pthread_create(&cycles[i],NULL,Servicing,(void *)id);

        if(res != 0){
            printf("Thread creation failed\n");
        }
    }

    for(int i = 0; i < cycleNumber; i++){
        void *result;
        pthread_join(cycles[i],&result);
        printf("%s",(char*)result);
    }

    for(int i=0;i<servicemenNumber;i++)
    {
        res = sem_destroy(&sem[i]);
        if(res != 0){
            printf("Failed\n");
        }  
    }
    return 0;
}
/// ....................................Payment Function ................................
void payment(void* arg)
{
    sem_wait(&payment_room_capacity);
    printf("%s started paying the service bill \n",(char*)arg);
    sleep(rand()%2);
    printf("%s finished paying the service bill \n",(char*)arg);
    sem_post(&payment_room_capacity);
    pthread_mutex_lock(&entry);
    // for(int i=0;i<servicemenNumber;i++){
    //     sem_wait(&sem[i]);

    // }
    Exit(arg);
  

}
/// ...................................Service Function ...................................
void* Servicing(void* arg){
    for(int i=0;i<servicemenNumber;i++){
        if(i==0)
        {
            sem_wait(&sem[i]);
            pthread_mutex_lock(&mutex[i]);
            pthread_mutex_lock(&entry);
        } 
        printf("%s started taking service from serviceman %d \n",(char*)arg,i+1);
        sleep(rand()%4);
        printf("%s finished taking service from serviceman %d \n",(char*)arg,i+1);
        if(i == 0){
            pthread_mutex_unlock(&entry);
        }
        if(i==(servicemenNumber-1))
        {
            pthread_mutex_unlock(&mutex[i]);
            sem_post(&sem[i]);
            payment(arg);
            pthread_mutex_unlock(&entry);
        }
        else
        {
            sem_wait(&sem[i+1]);
            pthread_mutex_unlock(&mutex[i+1]);
            pthread_mutex_unlock(&mutex[i]);
            sem_post(&sem[i]); 
        }
        
    }
    pthread_exit((void*)strcat((char*)arg," has  departed \n"));
}
void Exit(void* arg){
    // for(int i = servicemenNumber;i>0;i--){
    //     sem_wait(&sem[i-1]);
    //     pthread_mutex_lock(&mutex[i-1]);
    //     srand(time(0));
    //     sleep(rand()%2);
    //     if(i==1){
    //         printf("%s is departing \n",(char*)arg);

    //     }
    //     else{
    //         sem_wait(&sem[i-2]);
    //         pthread_mutex_unlock(&mutex[i-2]);
    //         pthread_mutex_unlock(&mutex[i-1]);
    //         sem_post(&sem[i-1]); 

    //     }

    // }
    for(int i=0;i<servicemenNumber;i++){
        sem_wait(&sem[i]);
        pthread_mutex_lock(&mutex[i]);
        
    }
    srand(time(0));
    sleep(rand()%2);
    for(int i=0;i<servicemenNumber;i++){
        pthread_mutex_unlock(&mutex[i]);
        sem_post(&sem[i]);
    }

}

