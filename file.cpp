/**
   Description: Generate a file with 100 numbers between 1-999
   Author: Aahana Sapra
   Date: 03/18/2026
 */


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>


using namespace std;


int main() {
  // Initialize int arr to default val of 0
  const int ARR_SIZE = 100;
  int arr[ARR_SIZE]{};

  ofstream outFile("numbers.txt"); // Create and open output file

  srand(time(0)); // Seed random number generator
  
  // Generate random numbers between 1-999 and write to file
  int min = 1;
  int max = 999;
  for (int i = 0; i < ARR_SIZE; i++) {
    arr[i] = min + (rand() % (max - min + 1));
    outFile << arr[i] << " ";
  }
  
  outFile.close();
  
  return 0;
}
