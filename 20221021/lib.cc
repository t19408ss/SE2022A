#include <iostream>
#include "common.h"
#include <chrono>
#include <vector>
using namespace std;

void printTime(std::chrono::system_clock::time_point start,
               std::chrono::system_clock::time_point end)
{
  auto dur = end - start;   
  auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  std::cout << msec << " ms \n";   
}

void check(int *array, const int size)
{
  int prev = array[0];
  for (int i = 1; i < size; i++) {
    int cur = array[i];
    if (prev > cur) {
      cout << "Sort failure: " << prev << ", " << cur << endl;
      exit(0);
    }
  }
}

void display(int *array, int size) {
  for(int i = 0; i < size; i++)
    cout << array[i] << " ";
  cout << endl;
}

void display_vec(std::vector<int>& array) {
  for(int i = 0; i < (int)array.size(); i++)
    cout << array[i] << " ";
  cout << endl;
}

void check_vec(std::vector<int>& array)
{
  int prev = array[0];
  for (int i = 1; i < (int)array.size(); i++) {
    int cur = array[i];
    if (prev > cur) {
      cout << "Sort failure: " << prev << ", " << cur << endl;
      exit(0);
    }
  }

  //display_vec(array);
}

