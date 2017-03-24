#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <cmath>

using namespace std;

#define NUM_THREADS 4096

/*To compile: g++ ugh.cpp -std=c++11 -lpthread -lm
or gcc ugh.cpp -std=c++11 -lpthread -lstdc++ -lm */

int arr[12][4096];
int i=0;
int maxnum=0;
int number_of_integers =0;

int count; //current count of running threads
sem_t sem_main;
pthread_mutex_t mutex;
pthread_cond_t ready_for_next_run;
pthread_t t[NUM_THREADS];
int row=0;

void barrier_function(){
    //lock the mutex
    pthread_mutex_lock(&mutex);
    if (count > 0){
        count--;
        while (count!=0){
            pthread_cond_wait(&ready_for_next_run, &mutex);
        }
        if (count==0){
            pthread_cond_broadcast(&ready_for_next_run);
        }
    } 
    sem_post(&sem_main);
    pthread_mutex_unlock(&mutex);

}

void * call_from_thread(void * i) {
    int first_index = (int)i;
    int second_index = (int)i+1;

    if (arr[row][first_index]>arr[row][second_index]){
        maxnum =  arr[row][first_index];
    } else {
        maxnum = arr[row][second_index];
    }
    arr[row+1][first_index/2] = maxnum;
       
	barrier_function();
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

/*********** READING INPUT FILE **********/

    if (argc != 2) {
        cout<<"Please enter a file name as well \n";
        return -1;
    }
    string line;
    ifstream myfile (argv[1]);
    if (myfile.peek() == std::ifstream::traits_type::eof()) {
        cout << "File is empty \n";
        return -1;
    }

    if (myfile.is_open()){
        while ( getline (myfile,line) )
        {
    		arr[0][number_of_integers] = std::stoi(line);
    		number_of_integers++;
        }
        myfile.close();
    }

    else {
        cout << "Unable to open file \n";
        return -1;
    }
   

 	
/*********** THREADS  ***************/

    count = number_of_integers / 2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&ready_for_next_run, NULL);
   
    //Launch a group of threads
    while (count!=0){
      sem_init(&sem_main, 0,0);
    
        for (int i=0; i<number_of_integers; i++){
            if ((i%2)==0){
                pthread_create(&t[(i/2)], NULL, call_from_thread, (void *) i);
            }

        }
        sem_wait(&sem_main);
        row++;
        number_of_integers = number_of_integers/2;
        //reset count to be the new number of running threads
        count=number_of_integers/2;
        
    } //end for number of runs

   cout << "The largest integer is " << maxnum << "\n";

   return(0);
}
