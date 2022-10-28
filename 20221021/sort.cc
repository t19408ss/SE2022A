#include <iostream>
#include <bits/stdc++.h>
#include "common.h"
#include <chrono>
#include <vector>
#define RANGE (1000*1000*10)
using namespace std;

std::vector<int> Input;         
std::vector<int> OriginalInput; 

Measure measure;

void init(int argc, char *argv[])
{
  if (argc != 2) {
    std::cout << "Usage: ./program datasize" << std::endl;
    exit(0);
  }
  int size = atoi(argv[1]);

  for(int i = 0; i < size; i++) {
    OriginalInput.push_back(i);
  }
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(OriginalInput.begin(), OriginalInput.end(), engine);
}

void resetInput()
{
  Input.clear();
  for(int i = 0; i < (int)OriginalInput.size(); i++) {
    int value = OriginalInput[i]; 
    Input.push_back(value);
  }
}

void iCannotBelieve()
{
  std::vector<int>& array = Input;
  // begin
  resetInput();

  measure.start();
  for (int i = 0; i < (int)array.size(); i++) {
    printf("[%d  ] ",i);
    for (int i = 0; i < (int)array.size(); i++) {
      printf("%d ", array[i]);
    } printf("\n");
    for (int j = 0; j < (int)array.size(); j++) {    
      if (array[i] < array[j]) {
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
        
        printf("[%d:%d] ",i,j);
        for (int k = 0; k < (int)array.size(); k++) {
          printf("%d ", array[k]);
        } printf("\n");

      }
    }
  }
  
  measure.end();
  check_vec(array);
  measure.print_elapsed_time("ICan'tBelieveItCanSort: ");
}

void insertSort()
{
  std::vector<int>& array = Input;
  
  // begin
  resetInput();
  measure.start();

  for (int j = 1; j < (int)array.size(); j++) {
    int key = array[j];
    int i = j - 1;
    while ((i >= 0) && (array[i] > key)) {
      array[i + 1] = array[i];
      i--;
    }
    array[i + 1] = key;
  }
  
  // end
  measure.end();
  check_vec(array);
  measure.print_elapsed_time("Insert: ");
}

// A utility function to swap two elements
void swap(int* a, int* b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (std::vector<int>& arr, int low, int high)
{
  int x = arr[high];
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (arr[j] <= x) {
      i++;
      swap(arr[i], arr[j]);
    }
  }
  swap(arr[i + 1], arr[high]);
  return i + 1;
}

/* The main function that implements QuickSort
arr[] --> Input to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(std::vector<int>& arr, int low, int high)
{
  if (low < high) {
    int mid = partition(arr, low, high);
    quickSort(arr, low, mid - 1);
    quickSort(arr, mid + 1, high);
  }
}

void quickSortStart() {
  vector<int>& arr = Input;
  resetInput();
  measure.start();

  quickSort(arr, 0, arr.size() - 1);

  measure.end();
  check_vec(arr);
  measure.print_elapsed_time("Quick: ");
}

// The main function that sort
void countSort()
{
  vector<int>& arr = Input;
  resetInput();
  int* output = new int[arr.size()];
 
  // Create a count array to store count of individual
  // number and initialize count array as 0
  int* count = new int[RANGE + 1];
  memset(count, 0, sizeof(int) * (RANGE+1));

  measure.start();

  for (int j = 0; j < (int)arr.size(); j++) {
    count[arr[j]]++;
  }
  for (int i = 0; i <= RANGE; i++) {
    count[i] += count[i - 1];
  }
  for (int j = 0; j < (int)arr.size(); j++) {
    output[count[arr[j]]] = arr[j];
    count[arr[j]]--;
  }
  for (int j = 0; j < (int)arr.size(); j++) {
    arr[j] = output[j];
  }

  measure.end();

  free(count);
  free(output);
  check_vec(arr);
  measure.print_elapsed_time("Count: ");
}

// A utility function to get maximum value in arr[]
int getMax(std::vector<int>& arr, int n)
{
  int mx = arr[0];
  for (int i = 1; i < n; i++)
    if (arr[i] > mx)
      mx = arr[i];
  return mx;
}


void defaultSort()
{
  vector<int>& arr = Input;
  resetInput();
  measure.start();
  std::sort(arr.begin(), arr.end());
  measure.end();
  check_vec(arr);
  measure.print_elapsed_time("std::sort: ");
}

// A utility function to print an array
void print(int arr[], int n)
{
  for (int i = 0; i < n; i++)
    cout << arr[i] << " ";
}

int main(int argc, char *argv[])
{
  init(argc, argv);

  insertSort();
  quickSortStart();
  countSort();
  defaultSort();

  return 0;
}
