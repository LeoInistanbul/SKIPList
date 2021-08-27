#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "../skiplist.h"

#define NUM_THREADS 1
#define TEST_COUNT 100000

SkipList<int,std::string> skiplist(18);

void* insertElement(void* threadid){
    long tid;
    tid=(long)threadid;
    std::cout<<tid<<std::endl;
    int tmp= TEST_COUNT/NUM_THREADS;
    for(int i=tid*tmp,count =0;count<tmp;i++){
        count ++;
        skiplist.insert_element(rand()%TEST_COUNT,"a");
    }
    pthread_exit(NULL);
}

void *getElement(void* threadid){
    long tid;
    tid=(long)threadid;
    std::cout << tid<<std::endl;
    int tmp=TEST_COUNT/NUM_THREADS;
    for(int i=tid*tmp,count=0;count<tmp;i++){
        count++;
        skiplist.search_element(rand()% TEST_COUNT);
    }
    pthread_exit(NULL);
}

int main(){
    srand (time(NULL));{
        pthread_t threads[NUM_THREADS];
        int rc,i;
        auto start = std::chrono::high_resolution_clock::now();

        for(int i=0;i<NUM_THREADS;i++){
            std::cout <<"main():creating thread,"<<i<<std::endl;
        } 
    }
}
