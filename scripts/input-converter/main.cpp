#include<iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <list>
#include <string>
using namespace std;

#define NUMJOBS 20
#define NUMMACHINES 20

int invalidFormatError(string message){
   cout<<"input file has invalid format: "<<message<<"\n";
   exit(EXIT_FAILURE);
}

int* convertListStingToInt(list<string> chunks){
   int size = chunks.size();
   int* nums = new int[size];
   list<string>::iterator it;
   for (int i = 0; i < size; i++){
      nums[i] = stoi(chunks.front());
      chunks.pop_front();
      }
   return nums;
}

int* parseLine(string line, int expected){
      istringstream stream(line);
      list<string> chunks;
      copy(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>(), std::back_inserter(chunks));
      if (expected != chunks.size()){
         cout<<"expected: " << expected << " actual: " <<chunks.size()<<". ";
         invalidFormatError("invalid number of arguments");
      }
      return convertListStingToInt(chunks);
}


void parseFile(string fileName){
   int* chunks;
   int jobs = 0;
   int machines = 0;
   array<array<int, NUMJOBS>, NUMMACHINES> operations;
   ifstream file;
   file.open(fileName);
   if (file){ 
       if (file.is_open()){
         string line;
         getline(file, line);  // get the first line containing two values: number of jobs and number of machines
            chunks = parseLine(line, 2);
            jobs = chunks[0];
            machines = chunks[1];
            delete[] chunks;
            for (int j = 0; j < jobs; j++){
                  getline(file, line); 
                  chunks = parseLine(line, machines * 2);
                  for (int m = 0; m < machines; m++){
                     int index = (2 * m) + 1;
                     operations[j][m] = chunks[index];
                  }
                  delete[] chunks;
            }
            cout<< jobs << " " << machines << "\n";
            for (int m = 0; m < machines; m++){
               for (int j = 0;  j < jobs; j++){
                  cout<<  operations[j][m] << " ";
            }
               cout<<"\n";
            }

      } else {
      printf("could not open file\n");
      exit(EXIT_FAILURE);
      }
   } else {
      printf("file doesnt exist\n");
      exit(EXIT_FAILURE);
   }
}

int main(int argc, char* argv[]) {   
   if (argc < 2){
      printf("input file not provided\n");
      exit(EXIT_FAILURE);
   }
   auto inputFile = argv[1];
   parseFile(inputFile);
}