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
#include <bits/stdc++.h>
#include <vector>
using namespace std;


struct Node {
   vector<int> scheduled;
   vector<int> left;
   vector<int> completion;
};

struct FSPspace {
  int machines;
  int jobs;
  int** operations;
};

int bound(FSPspace flowshop, Node node){
   vector<int> bounds;
   // vector<int> outTimes;
   //initialize out times
   // for (int s = 0; s < node.scheduled.size(); s++)  outTimes.push_back(0);

   for(int m = 0; m < flowshop.machines; m++){        //for all machines
      
      int bound = node.completion[m];
      vector<int> estimates;
      for (int l = 0; l < node.left.size(); l++){
         int j = node.left[l];
         bound += flowshop.operations[m][j];
         // only compute the future operations of unsheduled if the machine is not last
         if (m != flowshop.machines -1){
            int unscheduledOperations = 0;
            for (int op = m+1; op < flowshop.machines; op++){
               unscheduledOperations += flowshop.operations[op][j];
            }
            estimates.push_back(unscheduledOperations);
         }
      }

      if(!estimates.empty()){
         bound += *min_element(estimates.begin(), estimates.end());
      }
      bounds.push_back(bound);
   }
   return *max_element(bounds.begin(), bounds.end());
}

void solve(FSPspace flowshop){
   //initialize problem
   int ub = -1;
   vector<int> solution;
   vector<Node> problems;
   Node root;
   for (int i = 0; i < flowshop.jobs; i++) root.left.push_back(i);
   for (int m = 0; m < flowshop.machines; m++) root.completion.push_back(0);

   problems.push_back(root);
   //solve problem recursively
   int count = 0;
   while (problems.size() > 0){
      Node node = problems.back();
      problems.pop_back();

      // cout<< "nodes processed: " << count <<"\n";
      // count++;

      // cout << "scheduled: [ ";
      // for (int i =0; i<node.scheduled.size(); i++) cout << node.scheduled[i] << " ";
      // cout << "], left: [ ";
      // for (int i =0; i<node.left.size(); i++) cout << node.left[i] << " ";
      // cout <<"]\n";

      // if there are more than 1 jobs left unscheduled and uper bound is known
      if (node.left.size() > 1){
         if (ub != -1){
            int lb = bound(flowshop, node); //get lb
            if (lb < ub){
               for (int i =0; i < node.left.size(); i++){
                  Node child;
                  int newJob = node.left[i];
                  child.scheduled.assign(node.scheduled.begin(), node.scheduled.end());
                  child.scheduled.push_back(newJob); // schedule i'th element
                  child.left.assign(node.left.begin(), node.left.end()); // assign unscheduled jobs from parent
                  child.left.erase(child.left.begin()+i); // erase i'th element
                  child.completion.push_back(node.completion[0]+flowshop.operations[0][newJob]);
                  for (int m = 1; m < flowshop.machines; m++) child.completion.push_back(max(child.completion[m-1],node.completion[m]) +flowshop.operations[m][newJob]);
                  problems.push_back(child);

               }
            } 
            // else {cout<<"PRUNED\n";}
         } else {
            for (int i =0; i < node.left.size(); i++){
               Node child;
               int newJob = node.left[i];
               child.scheduled.assign(node.scheduled.begin(), node.scheduled.end());
               child.scheduled.push_back(newJob); // schedule i'th element
               child.left.assign(node.left.begin(), node.left.end()); // assign unscheduled jobs from parent
               child.left.erase(child.left.begin()+i); // erase i'th element
               child.completion.push_back(node.completion[0]+flowshop.operations[0][newJob]);
               for (int m = 1; m < flowshop.machines; m++) child.completion.push_back(max(child.completion[m-1],node.completion[m]) +flowshop.operations[m][newJob]);
               problems.push_back(child);            
            }
         }

      } else { // if one node is left

          int candidateBound = bound(flowshop, node);
          if (candidateBound < ub || ub == -1){
            ub = candidateBound;
            solution.assign(node.scheduled.begin(), node.scheduled.end());
            solution.push_back(node.left.back());
            cout<<"NEW SOLUTION: "<<ub<<"\n";
            }
         } 
      }
      cout << "Optimal makespan: " << ub <<"\n" << "Optimal job scheduling order: "; ;
      for(int i =0; i < solution.size(); i++) cout << solution[i] + 1 << " ";
         cout << "\n";
   }


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

FSPspace parseFile(string fileName){
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
            return flowshop;

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
      FSPspace flowshop = parseFile(argv[1]);
      solve(flowshop);
}
