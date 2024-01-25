#include<iostream>
#include<sstream>
#include<fstream>
#include<omp.h>
#include<chrono>
#include<time.h>
#include<cstdlib>
#include<ctime>

using namespace std;

// a class to time things using the high resolution timer class
// to use,  call start() before the code you want to time.  Call
// stop() after the code you want to time.  call getTime() to return
// the elapsed time in seconds.
// to reuse the instance,  call reset(), then time something else

class stopwatch{
private:
        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        bool timing;
public:
        stopwatch( ): timing( false ) {
                t1 = std::chrono::high_resolution_clock::time_point::min();
                t2 = std::chrono::high_resolution_clock::time_point::min();
        }
        void start( ) {
                if( !timing ) {
                        timing = true;
                        t1 = std::chrono::high_resolution_clock::now();
                }
        }
        void stop( ) {
                if( timing ) {
                        t2 = std::chrono::high_resolution_clock::now();
                        timing = false;
                }
        }
        void reset( ) {
                t1 = std::chrono::high_resolution_clock::time_point::min();
                t2 = std::chrono::high_resolution_clock::time_point::min();;
                timing = false;
        }
        // will return the elapsed time in seconds as a double
        double getTime( ) {
                std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1);
                return elapsed.count();
	}
};

// create an array of length size of random numbers
// returns a pointer to the array
// seed: seeds the random number generator
int * randNumArray( const int size, const int seed ) {

	srand( seed );
	int * array = new int[ size ];
	for( int i = 0; i < size; i ++ ) {
		array[i] = std::rand() % 1000000;
	}
	return array;
}

void customMerge(int arr[], int first, int middle, int last) {
    int leftSize = middle - first + 1;
    int rightSize = last - middle;

    int* leftArray = new int[leftSize];
    int* rightArray = new int[rightSize];

    // Copying data to temporary arrays leftArray[] and rightArray[]
    for (int i = 0; i < leftSize; i++) {
        leftArray[i] = arr[first + i];
    }
    for (int i = 0; i < rightSize; i++) {
        rightArray[i] = arr[middle + 1 + i];
    }

    int leftIndex = 0;
    int rightIndex = 0;
    int mergedIndex = first;

    // Merging the two subarrays back into the original array
    while (leftIndex < leftSize && rightIndex < rightSize) {
        if (leftArray[leftIndex] <= rightArray[rightIndex]) {
            arr[mergedIndex] = leftArray[leftIndex];
            leftIndex++;
        } else {
            arr[mergedIndex] = rightArray[rightIndex];
            rightIndex++;
        }
        mergedIndex++;
    }

    // Copying the remaining elements of leftArray[], if any
    while (leftIndex < leftSize) {
        arr[mergedIndex] = leftArray[leftIndex];
        leftIndex++;
        mergedIndex++;
    }

    // Copyingthe remaining elements of rightArray[], if any
    while (rightIndex < rightSize) {
        arr[mergedIndex] = rightArray[rightIndex];
        rightIndex++;
        mergedIndex++;
    }

    // Cleaning up temporary arrays
    delete[] leftArray;
    delete[] rightArray;
}


void MergeSort(int arr[], int first, int last) {
    int middle;
    if (first < last) {
        middle = (first + last) / 2;
        if ((last - first) <= 2) {
            MergeSort(arr, first, middle);
            MergeSort(arr, middle + 1, last);
        } else {
            MergeSort(arr, first, middle);
            MergeSort(arr, middle + 1, last);
        }
          customMerge(arr, first, middle, last);
     }
}

void parallelMergeSort(int arr[], int first, int last) {
    int middle;
    if (first < last) {
        middle = (first + last) / 2;
        if ((last - first) <= 2) {
            // Base case: if the array size is small enough, sort sequentially
            MergeSort(arr, first, middle);
            MergeSort(arr, middle + 1, last);
        } else {
           
            #pragma omp parallel sections
            {
                #pragma omp section
                MergeSort(arr, first, middle);

                #pragma omp section
                MergeSort(arr, middle + 1, last);
            }
            #pragma omp barrier // Adding barrier synchronization 
        }
        // Merging the two sorted halves
        customMerge(arr, first, middle, last);
    }
}


int main( int argc, char** argv ) {

	int * array;  // the poitner to the array of rands
	int size, seed, numThreads; // values for the size of the array
			// and the seed for generating
			// random numbers

			// check the command line args
	if( argc < 4 ){
		std::cerr << "usage: " 
			<< argv[0] 
			<< " [amount of random nums to generate] [number of threads to use] [seed value for rand]" 
			<< std::endl;
		exit( -1 );
	}

	// convert cstrings to ints
	{
		std::stringstream ss1( argv[1] );
		ss1 >> size;
	}
	{
		std::stringstream ss1( argv[2] );
		ss1 >> numThreads;
	}
	{
		std::stringstream ss1( argv[3] );
		ss1 >> seed;
	}

	// get the random numbers
	array = randNumArray( size, seed );
	// tell omp how many threads to use
    omp_set_num_threads( numThreads );

	stopwatch S1;
	S1.start();

	// **************************
	// **************************
	// **************************
	parallelMergeSort( array, 0, size-1);
	// **************************
	// **************************
	// **************************

	S1.stop();
	// print out the time
	std::cerr << "elapsed time: " << S1.getTime( ) << std::endl;

	// delete the heap memory
	delete [] array;

}

