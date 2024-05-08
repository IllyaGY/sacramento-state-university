#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>


using namespace std;                                            // For ECS environment, gcc sort.cpp -lstdc++ worked for me. It generated an a.out file which works perfectly.

const int MAX_SIZE = 10000000;

// Set this to true if you wish the arrays to be printed.
const bool OUTPUT_DATA = false;


void ReadInput(string& sortAlg, int& size);

void GenerateSortedData(int data[], int size);
void GenerateNearlySortedData(int data[], int size);
void GenerateReverselySortedData(int data[], int size);
void GenerateRandomData(int data[], int size);

void Sort(int data[], int size, string sortAlg, char* dataType);

void InsertionSort(int data[], int size);
void MergeSort(int data[], int lo, int hi);
void QuickSort(int data[], int lo, int hi, int rec, int &arec);
void LibSort(int data[], int size);


void Swap(int& x, int& y);

bool IsSorted(int data[], int size);
void printData(int data[], int size, string title);


int main(void)
{
	int size;
	string sortAlg;
	ReadInput(sortAlg, size);

	int* data = new int[size];
	if (data == NULL)
	{
		cout << "\n Memory allocation error." << endl;
		exit(1);
	}

	GenerateSortedData(data, size);
	Sort(data, size, sortAlg, "Sorted Data");

	GenerateNearlySortedData(data, size);
	Sort(data, size, sortAlg, "Nearly Sorted Data");

	GenerateReverselySortedData(data, size);
	Sort(data, size, sortAlg, "Reversely Sorted Data");

	GenerateRandomData(data, size);
	Sort(data, size, sortAlg, "Random Data");

	cout << "\nProgram Completed Successfully." << endl;;

	delete[] data;

	return 0;
}
/********************************************************************/




void ReadInput(string& sortAlg, int& size)
{
	cout << "  I:\tInsertion Sort" << endl;
	cout << "  M:\tMergeSort" << endl;
	cout << "  Q:\tQuickSort" << endl;
	cout << "  L:\tLibSort" << endl;
	cout << "Enter sorting algorithm: ";
	cin >> sortAlg;
	string sortAlgName;


	if (sortAlg == "I")
		sortAlgName = "Insertion Sort";
	else if (sortAlg == "M")
		sortAlgName = "MergeSort";
	else if (sortAlg == "Q")
		sortAlgName = "QuickSort";
	else if (sortAlg == "L")
		sortAlgName = "LibSort";
	else {
		cout << "\nUnrecognized sorting algorithm Code: " << sortAlg << endl;
		exit(1);
	}

	cout << "Enter input size: ";
	cin >> size;

	cout << "\nSorting Algorithm: " << sortAlgName;
	cout << "\nInput Size = " << size << endl;
	cout << endl;

}
/******************************************************************************/




void GenerateSortedData(int data[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		data[i] = i * 3 + 5;
}
/*****************************************************************************/



void GenerateNearlySortedData(int data[], int size)
{
	int i;

	GenerateSortedData(data, size);

	for (i = 0; i < size; i++)
		if (i % 10 == 0)
			if (i + 1 < size)
				data[i] = data[i + 1] + 7;
}
/*****************************************************************************/



void GenerateReverselySortedData(int data[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		data[i] = (size - i) * 2 + 3;
}
/*****************************************************************************/



void GenerateRandomData(int data[], int size)
{
	int i;

	for (i = 0; i < size; i++)
		data[i] = rand();            
}
/*****************************************************************************/


void Sort(int data[], int size, string sortAlg, char* dataType)
{
	cout << endl << dataType << ":";


	if (OUTPUT_DATA)
		printData(data, size, "Data before sorting:");


    int recDepth = 0;
	// Sorting is about to begin ... start the timer!
	clock_t start = clock();



	if (sortAlg == "I")
		InsertionSort(data, size);
	else if (sortAlg == "M")
		MergeSort(data, 0, size - 1);
	else if (sortAlg == "Q")
		QuickSort(data, 0, size - 1, 0 , recDepth);
	else if (sortAlg == "L")
		LibSort(data, size);
	else
	{
		cout << "Invalid sorting algorithm!" << endl;
		exit(1);
	}

	// Sorting has finished ... stop the timer!
	clock_t end = clock();
	double elapsed = (((double)(end - start)) / CLOCKS_PER_SEC) * 1000;



	if (OUTPUT_DATA)
		printData(data, size, "Data after sorting:");


	if (IsSorted(data, size))
	{
		cout << "\nCorrectly sorted " << size << " elements in " << elapsed << "ms";
        cout << "\nRecursion Depth(Only for QuickSort): " << recDepth << endl;                 //Modified the code to output the recursion depth ONLY FOR Quick Sort
        recDepth = 0;
	}
	else
		cout << "ERROR!: INCORRECT SORTING!" << endl;
	cout << "\n-------------------------------------------------------------\n";
}
/*****************************************************************************/




bool IsSorted(int data[], int size)
{
	int i;

	for (i = 0; i < (size - 1); i++)
	{
		if (data[i] > data[i + 1])
			return false;
	}
	return true;
}
/*****************************************************************************/




void InsertionSort(int data[], int size)
{
	for (int i = 1; i < size; i++) {
		int key = data[i];
		int j = i - 1;
		for (; j >= 0; ) {
			{
				if (data[j] > key) {

					data[j + 1] = data[j];
					j--;
				}
				else break;
			}
		}
		data[j + 1] = key;
	}

}
/*****************************************************************************/


void Combine(int data[], int lo, int mid, int hi) {								//Combine for MergeSort
	int sizeLeft = mid - lo + 1;                //Size of the left array
	int sizeRight = hi - mid;                   //Size of the right array
	int* left = new int[sizeLeft + 1];          //Creating both arrays with their sizes but +1
	int* right = new int[sizeRight + 1];
	int leftStep = 0;                           //Steps
	int rightStep = 0;
	for (int n = 0; n < sizeLeft; n++) {        //Filling out both the left and right arrays with the values from data[]
        *(left + n) = data[n + lo];
		if (n < sizeRight)
			*(right + n) = data[n + mid + 1];
	}
	*(left + sizeLeft) = *(right + sizeRight - 1) + 1;  //For each array we put an extra element that is greater by one
	*(right + sizeRight) = *(left + sizeLeft - 1) + 1;  //than the n'th element in the opposite array
	for (int k = lo; k <= hi; k++) {
		if (left[leftStep] > right[rightStep]) {
			data[k] = *(right+rightStep);
			rightStep++;
		}
		else {
			data[k] = *(left + leftStep);
			leftStep++;
		}
	}

	delete[] left;
	delete[] right;
}
/****************************************************************************************/ 
									
void MergeSort(int data[], int lo, int hi)								//MergeSort
{
	if (lo == hi) return;
	int mid = (hi + lo) / 2;
	MergeSort(data, lo, mid);
	MergeSort(data, mid+1, hi);
	Combine(data, lo, mid, hi);

}
/*****************************************************************************/


int PartitionLastPivot(int data[], int lo, int hi) {							//Last element partitioning
	int lessPivot = lo-1;
	for (int i = lo; i < hi; i++) {
		if (data[i] < data[hi]) {
			Swap(data[i], data[lessPivot + 1]);
			lessPivot++;
		}


	}
	Swap(data[lessPivot + 1], data[hi]);


	return lessPivot + 1;

}

/***************************************************************************/
int PartitionRand(int data[], int lo, int hi) {							 //Random element Partitioning
	int randIndex = rand() %(hi-lo + 1) + lo;                    // Random number from lo(inclusive) to hi(inclusive)
	Swap(data[randIndex], data[hi]);
	int lessPivot = lo-1;
	for (int i = lo; i < hi; i++) {
		if (data[i] < data[hi]) {
			Swap(data[i], data[lessPivot + 1]);
			lessPivot++;
		}


	}
	Swap(data[lessPivot + 1], data[hi]);


	return lessPivot + 1;

}
/*******************************************************************/ 			//Swaps the median of three with the last element in data array
void medianSwap(int data[], int median[], int hi){
	int medianN = median[0]; // Assume lo is the median initially
    	if ((data[median[0]] < data[median[1]] && data[median[1]] < data[median[2]]) ||  (data[median[2]] < data[median[1]] && data[median[1]] < data[median[0]]))
        	medianN = median[1];
    	else if ((data[median[0]] < data[median[2]] && data[median[2]] < data[median[1]]) || (data[median[1]] < data[median[2]] && data[median[2]] < data[median[0]]))
        	medianN = median[2];
	Swap(data[hi], data[medianN]);

}
/***************************************************************/			//Partitioning using the median of three
int PartitionMedian(int data[], int lo, int hi) {
	int median[3];		//Array for the 3 random indexes
	for(int i = 0; i < 3; i++)
		median[i] = rand()%(hi-lo+1) +lo;
	
	medianSwap(data, median, hi); 	
	int lessPivot = lo - 1;
	for (int i = lo; i < hi; i++) {
		if (data[i] < data[hi]) {
			Swap(data[i], data[lessPivot + 1]);
			lessPivot++;
		}


	}
	Swap(data[lessPivot + 1], data[hi]);


	return lessPivot + 1;

}

/**************************************************************/                //Quick Sort



void QuickSort(int data[], int lo, int hi, int rec, int &arec)  //The base rec(recursion depth) value is 0, arec is used to get the maximum
{                                                               //recursion depth as the algorithm finishes

    if (lo >= hi){
        if (rec > arec) arec = rec;
        return;
    }
   /*
    else if(hi - lo < 40){                                // For Insertion Sort implementation, also uncomment the PartitionRand
        InsertionSort(data+lo, hi-lo+1);		  // Also uncomment for the extra credit part
        return;
    }
    */


    //int q = PartitionLastPivot(data, lo, hi);               // Partitioning when the last element is chosen as the pivot


    //int q = PartitionMedian(data, lo, hi);                // For Median of Three Implementation

    int q = PartitionRand(data, lo, hi);                    //Partitioning when a random element is chosen as the pivot

    QuickSort(data, lo, q-1,rec+1, arec);
    QuickSort(data, q+1, hi, rec+1, arec);




//EXTRA CREDIT: Implementing both the median of three and the Insertion Sort gives slightly better results compared to the Library Sort. 
//The first reason is because using the media of three gives us more chance of picking a true median value in the array. 
//Therefore, rather than picking it only once, we pick it 3 times to get a bit closer to the true median.
// Next, since the insertion sort has better performance time with shorter arrays we can avoid doing the recursion and expensive partitioning. 
// Moreover, by using Insertion Sort we avoid addition memory usage, and overhead.
 






    

}

/*****************************************************************************/


void LibSort(int data[], int size)
{

	//Write your code here
	//Your code should simply call the STL sorting function
    sort(data, data+size);
}
/*****************************************************************************/




void Swap(int& x, int& y)
{
	int temp = x;
	x = y;
	y = temp;
}
/*****************************************************************************/




void printData(int data[], int size, string title) {
	int i;

	cout << endl << title << endl;
	for (i = 0; i < size; i++)
	{
		cout << data[i] << " ";
		if (i % 10 == 9 && size > 10)
			cout << endl;
	}
}

