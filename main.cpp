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
  int operations [0] [0];
};

int invalidFormatError(string message){
   cout<<"input file has invalid format: "<<message<<"\n";
   exit(EXIT_FAILURE);
}

list<int> convertListStingToInt(list<string> chunks){
   list<int> nums;
   list<string>::iterator it;
   for (it = chunks.begin(); it != chunks.end(); ++it){
      nums.push_back(stoi(*it));
      }
   return nums;
}

list<int> parseLine(string line){
      istringstream stream(line);
      list<string> chunks;
      copy(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>(), std::back_inserter(chunks));
      return convertListStingToInt(chunks);
}

void parseFile(string fileName){
   ifstream file;
   file.open(fileName);
   int machines, jobs;
   if (file){ 
      printf("file exists\n");
       if (file.is_open()){
         string line;
         getline(file, line);
         list<string> chunks = parseLine(line);
         if( chunks.size() != 2) {
            invalidFormatError("firstline does not contain 2 arguments");
         } else {
            jobs = stoi(chunks.front());
            chunks.pop_front();
            machines = stoi(chunks.front());
            cout<< "jobs: " <<jobs << " machines:" << machines << "\n";
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






