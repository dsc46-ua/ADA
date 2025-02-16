//DANIEL SEGURADO CLIMENT 48791680D
/*
ADA. 2023-24
Práctica 2:"Empirical analysis by means of program-steps account of two sorting algorithms: Middle-Quicksort and Heapsort."
*/
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <iomanip>


using namespace std;

//--------------------------------------------------------------
// Middle Quick Sort
void middle_QuickSort(int *v, long left, long right, int &steps) {
    steps++;
    long i, j;
    int pivot;
    if (left < right) {
        i = left; 
        j = right;
        pivot = v[(i + j) / 2];
        do {
            while (v[i] < pivot) {
                i++;
                steps++;
            }
            while (v[j] > pivot) {
                j--;
                steps++;
            }
            if (i <= j) {
                swap(v[i], v[j]);
                i++; 
                j--;
            }
            steps++;
        } while (i <= j);
        if (left < j) {
            middle_QuickSort(v, left, j, steps);
        }
        if (i < right) {
            middle_QuickSort(v, i, right, steps);
        }
    }
    steps++;
}

//--------------------------------------------------------------                
//HEAPSORT
// Procedure sink used by Heapsort algorith
// Sink an element (indexed by i) in a tree to keep the heap property. n is the size of the heap. 

void sink(int *v, size_t n, size_t i, int &steps)
{
steps++;
size_t largest;
size_t l, r; //left and right child

do{
    largest = i;  // Initialize largest as root
    l = 2*i + 1;  // left = 2*i + 1
    r = 2*i + 2;  // right = 2*i + 2
 
    // Is left child larger than root?
    if (l < n && v[l] > v[largest])
        largest = l;
        
 
    // Is right child larger than largest so far
    if (r < n && v[r] > v[largest])
        largest = r;

 
    // If largest is still root then the process is done
    if (largest == i) break;
    
	// If not, swap new largest with current node i and repeat the process with childs. 
    swap(v[i], v[largest]);
    i=largest;
    steps++;
   } while (true);
}
 
//--------------------------------------------------------------                
// Heapsort algorithm (ascending sorting)
void heapSort(int *v, size_t n, int &steps)
{
    steps++;
    // Build a MAX-HEAP with the input array
    for (size_t i = n / 2 - 1; true ; i--){
        sink(v, n, i, steps);
        steps++;
        if (i==0) break; //as size_t is unsigned type
	}	

  	// A this point we have a HEAP on the input array, let's take advantage of this to sort the array:
    // One by swap the first element, which is the largest (max-heap), with the last element of the vector and rebuild heap by sinking the new placed element on the begin of vector.  
    for (size_t i=n-1; i>0; i--)
    {
		// Move current root to the end.
        swap(v[0], v[i]);
 		// Now largest element is at the end but do not know if the element just moved to the beginning is well placed, so a sink process is required.
		// Note that, at the same time, the HEAP size is reduced one element by one.
        sink(v, i, 0, steps);
		// The procedure ends when HEAP has only one element, the lower of the input array
        steps++;
    }
}

void randomArray(int *v, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        v[i] = rand();
    }
}

void sortedArray(int *v, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        v[i] = i;
    }
}

void reverseSortedArray(int *v, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        v[i] = n - i;
    }
}

int main() {
    int steps;
    srand(0);
    cout << "#QUICKSORT VERSUS HEAPSORT" << endl;
    cout << "#Average processing Msteps (millions of program steps)" << endl;
    cout << "#Number of samples (arrays of integer): 30" << endl;
    cout << "#       RANDOM ARRAYS    SORTED ARRAYS REVERSE SORTED ARRAYS" << endl;
    cout << "#     ---------------- ---------------- ----------------" << endl;
    cout << "#Size QuickSrt|HeapSrt|QuickSrt|HeapSrt|QuickSrt|HeapSrt" << endl;
    cout << "=========================================================" << endl;

    for (int exp = 15; exp <= 20; ++exp) {
        size_t size = pow(2, exp);
        float totalStepsQuickSortRandom = 0;
        float totalStepsHeapSortRandom = 0;
        float totalStepsQuickSortSorted = 0;
        float totalStepsHeapSortSorted = 0;
        float totalStepsQuickSortReverse = 0;
        float totalStepsHeapSortReverse = 0;

        for (int sample = 0; sample < 30; ++sample) {
            int *arrayRandom = new int[size];
            int *arraySorted = new int[size];
            int *arrayReverseSorted = new int[size];
            steps = 0;

            randomArray(arrayRandom, size);
            sortedArray(arraySorted, size);
            reverseSortedArray(arrayReverseSorted, size);

            // QuickSort en arreglo aleatorio
            middle_QuickSort(arrayRandom, 0, size - 1, steps);
            totalStepsQuickSortRandom += steps;
            steps = 0;

            // QuickSort en arreglo ordenado
            middle_QuickSort(arraySorted, 0, size - 1, steps);
            totalStepsQuickSortSorted += steps;
            steps = 0;

            // QuickSort en arreglo ordenado en reversa
            middle_QuickSort(arrayReverseSorted, 0, size - 1, steps);
            totalStepsQuickSortReverse += steps;
            steps = 0;

            // HeapSort en arreglo aleatorio
            heapSort(arrayRandom, size, steps);
            totalStepsHeapSortRandom += steps;
            steps = 0;

            // HeapSort en arreglo ordenado
            heapSort(arraySorted, size, steps);
            totalStepsHeapSortSorted += steps;
            steps = 0;

            // HeapSort en arreglo ordenado en reversa
            heapSort(arrayReverseSorted, size, steps);
            totalStepsHeapSortReverse += steps;
            steps = 0;

            delete[] arrayRandom;
            delete[] arraySorted;
            delete[] arrayReverseSorted;
        }

        totalStepsQuickSortRandom /= 30000000;
        totalStepsQuickSortSorted /= 30000000;
        totalStepsQuickSortReverse /= 30000000;
        totalStepsHeapSortRandom /= 30000000;
        totalStepsHeapSortSorted /= 30000000;
        totalStepsHeapSortReverse /= 30000000;

        cout << size << setprecision(5) << "\t" << totalStepsQuickSortRandom << "\t" << totalStepsHeapSortRandom
             << "\t" << totalStepsQuickSortSorted << "\t" << totalStepsHeapSortSorted
             << "\t" << totalStepsQuickSortReverse << "\t" << totalStepsHeapSortReverse << endl;
    }

    return 0;
}