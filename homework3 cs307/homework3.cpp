//Melis Doğan 26661
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <queue>
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
    int id;
    int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size


bool isReleased = true;


void my_malloc(int thread_id, int size)
{
    //This function will add the struct to the queue
    node temp;
    temp.id = thread_id;
    temp.size = size;
    myqueue.push(temp);
}


void * server_function(void *)
{
    //This function should grant or decline a thread depending on memory size.
    int MEMORY_INDEX = 0;
    while (isReleased==true) {
        pthread_mutex_lock(&sharedLock);
        while(!myqueue.empty()){
            node * tempNode = new node();
            tempNode =&myqueue.front();
            myqueue.pop();
            if(tempNode->size < MEMORY_SIZE-MEMORY_INDEX)
            {
                thread_message[tempNode->id] = MEMORY_INDEX;
                MEMORY_INDEX += tempNode->size;
            }
            else
            {
                thread_message[tempNode->id] = -1;
            }
            sem_post(&semlist[tempNode->id]);
        }
        pthread_mutex_unlock(&sharedLock);
    }
    return 0;
}

void * thread_function(void * id)
{
    //This function will create a random size, and call my_malloc
    //Block
    //Then fill the memory with id's or give an error prompt
    pthread_mutex_lock(&sharedLock);
    int * id_p = (int*) id;
    int random_size = rand() % 1000/6;
      if(random_size < MEMORY_SIZE/6){
          my_malloc(*id_p,random_size);
          pthread_mutex_unlock(&sharedLock);
          sem_wait(&semlist[*id_p]); //block

          if(thread_message[*id_p] == -1) //fill or give message
          {
              cout << "No available memory"<<*id_p<< endl;
          }
          else{
              
              for (size_t i = 0; i < random_size; i++)
                  memory[i] = '1';
                  thread_message[*id_p]+= random_size;
          }
      }
      else
          pthread_mutex_unlock(&sharedLock);
      return 0;
}

void init()
{
    pthread_mutex_lock(&sharedLock);    //lock
    for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
    {sem_init(&semlist[i],0,0);}
    for (int i = 0; i < MEMORY_SIZE; i++)    //initialize memory
      {char zero = '0'; memory[i] = zero;}
       pthread_create(&server,NULL,server_function,NULL); //start server
    pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory()
{
 // You need to print the whole memory array here.
    cout << "Memory Dump:" << endl;
    for (size_t i = 0; i < MEMORY_SIZE; i++)
    {
        cout << memory[i];
    }
}

int main (int argc, char *argv[])
 {

     srand(time(NULL));

     pthread_t threads[NUM_THREADS];
     int thread_id[NUM_THREADS];
       for (int i = 0; i < NUM_THREADS; i++)
        {
           thread_id[i] = i;
        }
       init();
       for (int i = 0; i < NUM_THREADS; i++) //joins threads
       {
           pthread_create( &threads[i], NULL, thread_function, (void *) &thread_id[i]);
       }

       for (size_t i = 0; i < NUM_THREADS; i++)
       {
           pthread_join(threads[i], NULL);
       }
     
     dump_memory(); // this will print out the memory
     
     printf("\nMemory Indexes:\n" );
     for (int i = 0; i < NUM_THREADS; i++)
     {
         printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
     }
     printf("\nTerminating...\n");
 }

