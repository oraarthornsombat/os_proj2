#include <iostream>
#include <thread>
#include <string>
#include <fstream>

using namespace std;

//static const int num_threads = 2;

/*To compile: g++ thread.cpp -std=c++11 -lpthread
or gcc thread.cpp -std=c++11 -lpthread -lstdc++ */

/********** GLOBAL VARIABLES ***********/
int arr [4096];
int newarr[4096];
int arrcount =0;
int i=0;
int maxnum=0;
int num_threads;

//This function will be called from a thread
void call_from_thread(int thread_num) {
    i = 0;
    int index=0;
    cout << "Launched by thread "<< thread_num << "\n";
 
    for (i = 0; i< arrcount ; i+=2){
        maxnum = 0;
        if (i == thread_num * 2){
            //cout << i << "thread: "<< thread_num * 2<< endl;
            if (arr[i] > arr[i+1]){
                maxnum = arr[i];
            } else {
                maxnum = arr[i+1];
            }
            //cout << maxnum;
            newarr[index] = maxnum;
        }
        index++;
    }
    for (i=0; i<5; i++){
        cout << newarr[i] << endl;
    }
    
   
    // for (i =0; i<arrcount ; i++){
    //     if (arr[i] > maxnum){
    //         maxnum = arr[i];
    //     }
        
    // }
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
          arrcount = i;
        }
        myfile.close();
        num_threads = arrcount/2;
        //cout << num_threads;
    }

    else {
        cout << "Unable to open file \n";
        return -1;
    }

 
/*********** THREADS  ***************/
    std::thread t[num_threads];

    //Launch a group of threads
    for (int i = 0; i < num_threads; ++i) {
        t[i] = std::thread(call_from_thread, i);

    }

    //cout << "Launched from the main\n";

    //Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i) {
        t[i].join();
    }

    cout << "Max is " << maxnum << "\n";

    return 0;
}
