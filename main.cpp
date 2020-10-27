#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <fstream>
#include <array>

#include <sstream>
#include <algorithm>
#include <iterator>
#include <list>
#include <string>
using namespace std;

struct FSPspace {
  int machines;
  int jobs;
  int** operations;
};

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
   ifstream file;
   file.open(fileName);
   FSPspace flowshop;
   if (file){ 
      printf("file exists\n");
       if (file.is_open()){
         string line;
         getline(file, line);  // get the first line containing two values: number of jobs and number of machines
            chunks = parseLine(line, 2);
            flowshop.jobs = chunks[0];
            flowshop.machines = chunks[1];
            flowshop.operations = new int* [flowshop.machines];
            cout<< "jobs: " <<flowshop.jobs << " machines:" << flowshop.machines << "\n";
            for (int m = 0; m < flowshop.machines; m++){
                  flowshop.operations[m] = new int [flowshop.jobs];
                  getline(file, line); 
                  chunks = parseLine(line, flowshop.jobs);
                  for (int j = 0; j< flowshop.jobs; j++){
                   flowshop.operations[m][j] = chunks[j];
                  }
            }
            for (int m = 0; m < flowshop.machines; m++){
               for (int j = 0;  j < flowshop.jobs; j++){
                  cout<<  flowshop.operations[m] [j] << " ";
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
         parseFile(argv[1]);
}
