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
// int current_count = 0; //current count to compare with condition variable
sem_t sem_main;

//This function will be called from a thread
void * call_from_thread(void * thread_id) {
    i = 0;
    int index=0;
    cout << "Launched by thread "<< (int)thread_id << "\n";
 
    if (arr_is_all_zero == false){
        //array 1 (arr) has the values. write result of comparison to array 2 (newarr).
        for (i = 0; i< number_of_integers ; i+=2){
            maxnum = 0;
            /*figure out which index to look at (thread 0 at index 0, thread 1 at index 2, 
            thread 2 at index 4, etc.) */
            if (i == (int)thread_id * 2){
                //cout << i << "thread: "<< thread_id * 2<< endl;
                if (arr[i] > arr[i+1]){
                    maxnum = arr[i];
                } else {
                    maxnum = arr[i+1];
                }
                newarr[index] = maxnum;
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


    for (i=0; i<number_of_threads; i++){
        cout << "new arr "<< newarr[i] << endl;
       // cout << "arr2 should be 0: "<<new_arr_is_all_zero << endl;
    }
    // for (i=0; i<number_of_integers;i++){
    // //     cout << arr[i] << "should be 0 for 1st run" << endl;
    // //    // cout << "arr should be 1: "<<arr_is_all_zero << endl;
    //     cout << "arr "<< arr[i] << endl;
    // }
    return NULL;
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
    cout << double(number_of_integers) << endl;
    cout << log2(number_of_integers);
    cout << number_of_runs;
    //std::thread t[number_of_threads];
    pthread_t t[number_of_threads];
    sem_init(&sem_main, 0,0);
   
    //Launch a group of threads
    for (int i = 0; i < number_of_threads; ++i) {
        //t[i] = std::thread(call_from_thread, i);
        pthread_create(&t[i], NULL, call_from_thread, (void *) i);
        //  cout << "arr is zero? " << arr_is_all_zero << endl;
        // cout << "new_arr is zero? " << new_arr_is_all_zero << endl;

    }

    //Change this later
    //Join the threads with the main thread
    for (int i = 0; i < number_of_threads; ++i) {
        //t[i].join();
        pthread_join(t[i], NULL);
    }


   // cout << "Max is " << maxnum << "\n";

    return 0;
}
