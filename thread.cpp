#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <cmath>
// #include <mutex>
// #include <condition_variable>

using namespace std;

//static const int num_threads = 2;

/*To compile: g++ thread.cpp -std=c++11 -lpthread -lm
or gcc thread.cpp -std=c++11 -lpthread -lstdc++ -lm */

/********** GLOBAL VARIABLES ***********/
int arr [4096];
int newarr[4096] = {0};
int i=0;
int maxnum=0;
int number_of_threads;
int number_of_runs;
int number_of_integers =0;
bool arr_is_all_zero = false;
bool new_arr_is_all_zero = true;
// std::mutex mtx; // mutex for critical section
// std::condition_variable cv; //condition variable for critical section
int count; //current count of running threads
sem_t sem_main;
pthread_mutex_t mutex;
pthread_cond_t ready_for_next_run;


void barrier_function(){
    //lock the mutex
   // cout<<count<<endl;
    // cout << "arr is all zero: "<<arr_is_all_zero<< endl;
    // cout << "new arr is all zero: "<<new_arr_is_all_zero<< endl;
    pthread_mutex_lock(&mutex);
        // cout << "number of threads: "<< number_of_threads << endl;
        // cout << "number of integers to compare: "<< number_of_integers << endl;
        // cout << "count: "<<count<<endl;

    if (count > 0){
         count--;
         while (count!=0){
            cout << "stuck here?"<<endl;
             pthread_cond_wait(&ready_for_next_run, &mutex);
         }
    }

    // //trial
    // if (arr_is_all_zero==false){
    //         //last thread of the current run to come through
    //         for (i=0; i<4096;i++){
    //             //rewrite whole arr to be zeroes, to use for storing next round of comparisons
    //             arr[i] = 0;
    //         }
    //         //now new_arr has the values, and arr is all zeroes
    //         arr_is_all_zero = true;
    //         new_arr_is_all_zero = false;
    // } else if (arr_is_all_zero==true){
    //         //last thread of the current run to come through
    //         for (i=0; i<4096;i++){
    //         //rewrite whole new_arr to be zeroes, to use for storing next round of comparisons
    //             newarr[i] = 0;
    //         }
    //         //now arr has the values, and new_arr is all zeroes
    //         arr_is_all_zero = false;
    //         new_arr_is_all_zero = true;
    // }
    cout << "got out!"<<endl;
    pthread_cond_broadcast(&ready_for_next_run);
    sem_post(&sem_main);
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

}

//This function will be called from a thread
void * call_from_thread(void * thread_id) {

    i = 0;
    int index=0;
    cout << "Launched by thread "<< (int)thread_id << "\n";
    cout << "num of ints "<<number_of_integers<<endl;
 
    if (arr_is_all_zero == false){
        //array 1 (arr) has the values. write result of comparison to array 2 (newarr).
        for (i = 0; i< number_of_integers ; i+=2){
            maxnum = 0;
            /*figure out which index to look at (thread 0 at index 0, thread 1 at index 2, 
            thread 2 at index 4, etc.) */
            if (i == (int)thread_id){
                //cout << i << "thread: "<< thread_id * 2<< endl;
                if (arr[i] > arr[i+1]){
                    maxnum = arr[i];
                } else {
                    maxnum = arr[i+1];
                }
                newarr[index] = maxnum;
                cout << "maxnum is "<<maxnum<<endl;
            }
       
            index++;
        }
        // if (thread_id ==0){
        //     //last thread of the current run to come through
        //     for (i=0; i<number_of_integers;i++){
        //         //rewrite whole arr to be zeroes, to use for storing next round of comparisons
        //         arr[i] = 0;
        //     }
        //     //now new_arr has the values, and arr is all zeroes
        //     arr_is_all_zero = true;
        //     new_arr_is_all_zero = false;
        // }


    } else if (arr_is_all_zero == true) {
        cout << "doesn't enter here yet";
        // array 2 (newarr) has the values. write result of comparison to array 1 (arr).
         for (i = 0; i< number_of_integers ; i+=2){
            maxnum = 0;
            /*figure out which index to look at (thread 0 at index 0, thread 1 at index 2, 
            thread 2 at index 4, etc.) */
            if (i == int(thread_id) * 2){
                //cout << i << "thread: "<< thread_id * 2<< endl;
                if (newarr[i] > newarr[i+1]){
                    maxnum = newarr[i];
                } else {
                    maxnum = newarr[i+1];
                }
                arr[index] = maxnum;
            }

            index++;
        }

        // if (thread_id ==0){
        //     //last thread of the current run to come through
        //     for (i=0; i<number_of_integers;i++){
        //     //rewrite whole new_arr to be zeroes, to use for storing next round of comparisons
        //         newarr[i] = 0;
        //     }
        //     //now arr has the values, and new_arr is all zeroes
        //     arr_is_all_zero = false;
        //     new_arr_is_all_zero = true;
        // }
    }

    //printing
    for (i=0; i<number_of_threads; i++){
        cout << "new arr "<< newarr[i] << endl;
       // cout << "arr2 should be 0: "<<new_arr_is_all_zero << endl;
    }
    // for (i=0; i<number_of_integers;i++){
    // //     cout << arr[i] << "should be 0 for 1st run" << endl;
    // //    // cout << "arr should be 1: "<<arr_is_all_zero << endl;
    //     cout << "arr "<< arr[i] << endl;
    // }
    //cout << count <<endl;
    barrier_function();
    //sem_post(&sem_main);
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
    if (myfile.is_open()){
        while ( getline (myfile,line) )
        {
          arr[i] = std::stoi(line);
          i++;
          number_of_integers = i;
        }
        myfile.close();
    }

    else {
        cout << "Unable to open file \n";
        return -1;
    }

 
/*********** THREADS  ***************/

    number_of_threads = number_of_integers / 2;
    number_of_runs = log2(number_of_integers);
    //std::thread t[number_of_threads];
    pthread_t t[number_of_threads];
    sem_init(&sem_main, 0,0);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&ready_for_next_run, NULL);
   

   pthread_create(&t[0], NULL, call_from_thread, NULL);

    //Launch a group of threads
    for (int j=0; j<number_of_runs; j++){
        for (int i = 1; i < number_of_threads+1; ++i) {
        //for (int i=0; i<number_of_threads; i++){
            //t[i] = std::thread(call_from_thread, i);
            pthread_create(&t[i], NULL, call_from_thread, (void *) i);
            sem_wait(&sem_main);
            // number_of_integers = number_of_integers/2;
            // number_of_threads=number_of_integers/2;
            count = number_of_threads;
            //  cout << "arr is zero? " << arr_is_all_zero << endl;
            // cout << "new_arr is zero? " << new_arr_is_all_zero << endl;

        }
    }

    //Change this later
    //Join the threads with the main thread
    // for (int i = 0; i < number_of_threads; ++i) {
    //     //t[i].join();
    //     pthread_join(t[i], NULL);
    // }


   // cout << "Max is " << maxnum << "\n";

    return 0;
}
