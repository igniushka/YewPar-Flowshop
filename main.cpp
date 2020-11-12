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
#include <tuple>
#include <vector>
#include <stack> 
using namespace std::chrono; 
using namespace std;

struct JobLength{
   int index;
   int length;
   JobLength(){
   }
};


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


  int getMin(int minimum, int value){
    if (minimum == -1) return value;
    return min(minimum, value);
   }

int bound(FSPspace *flowshop, Node *node){
   vector<int> bounds;

//special case for last machine. no esitmations to add to the bound.
int* estimate = new int[node->left.size()];
int last = flowshop->machines -1;
int bound = node->completion[last];
for (int l = 0; l < node->left.size(); l++){
   int j = node->left[l];
   int op = flowshop->operations[last][j]; //add unsheculed job operations of that machine
   bound += op;
   estimate[l]=(op);
} 
bounds.push_back(bound);

for (int m = flowshop->machines -2; m >= 0; m--){ //starting from the second to last machine
   int min = -1;
   int bound = node->completion[m]; //bound base is the completion time 
   for (int l = 0; l < node->left.size(); l++){ 
      int j = node->left[l];
      int op = flowshop->operations[m][j]; //get 
      int prev = estimate[l];
      bound += op; //add unsheculed job operations of that machine
      min = getMin(min, prev);
      estimate[l] = op + prev;     
   }
   bound += min;
   bounds.push_back(bound);
   }
   delete [] estimate;
   return *max_element(bounds.begin(), bounds.end());
}

bool compareJobLengths(JobLength j1, JobLength j2){
   return j1.length > j2.length;
}

int calculateSequence(vector<int> jobs, FSPspace flowshop){
  int* completionTimes = new int [flowshop.machines];
  for (int m = 0; m < flowshop.machines; m++){
     completionTimes[m] = 0;
  }
   for (int j = 0; j < jobs.size(); j++){
      int jobIndex = jobs[j];
      completionTimes[0]+= flowshop.operations[0][jobIndex];
         for (int m = 1; m<flowshop.machines; m++){
            completionTimes[m] = max(completionTimes[m-1], completionTimes[m]) + flowshop.operations[m][jobIndex];
   }
   }
   int result = completionTimes[flowshop.machines-1];
   delete [] completionTimes;
   return result;
}

tuple<vector<int>, int> findBestSequence(vector<int> scheduled, int unscheduled, FSPspace flowshop){
   vector<int> partial;
   vector<int> bestSequence;
   bestSequence.assign(scheduled.begin(), scheduled.end());
   bestSequence.insert(bestSequence.begin(), unscheduled);
    int best = calculateSequence(bestSequence, flowshop);
    for (int i = 1; i <= scheduled.size(); i++){
       partial.assign(scheduled.begin(), scheduled.end());
       partial.insert(partial.begin()+i, unscheduled);
       int makespan = calculateSequence(partial, flowshop);
       if (makespan < best){
         best = makespan;
         bestSequence.assign(partial.begin(), partial.end());
       }
    }
    return make_tuple(bestSequence, best);
}


tuple<vector<int>, int> initilizeUpperBound(FSPspace flowshop){
   vector<JobLength> jobLengths;
   vector<int> schedule;
   for (int j = 0; j< flowshop.jobs; j++){
      JobLength jobLength;
      jobLength.length = 0;
      jobLength.index = j;
      for (int m = 0; m<flowshop.machines; m++){
         jobLength.length+=flowshop.operations[m][j];
         }

      jobLengths.push_back(jobLength);
   }
   sort(jobLengths.begin(), jobLengths.end(), compareJobLengths);
   schedule.push_back(jobLengths.at(0).index);
   int best;
   for (int i = 1; i < flowshop.jobs; i++){
      int newBest;
      vector<int> seq;
     tie(seq, newBest) = findBestSequence(schedule, jobLengths.at(i).index, flowshop);
      schedule.assign(seq.begin(), seq.end());
      best = newBest;
   }
   return make_tuple(schedule, best);
}

void solve(FSPspace flowshop){

   auto start = high_resolution_clock::now(); 
   auto branchingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
 
   //initialize problem

   //calculate ub
   int ub = 0;
   vector<int> solution;
   auto upperBoundstart = high_resolution_clock::now();
   tie(solution, ub) = initilizeUpperBound(flowshop);
   auto upperBoundTime = duration_cast<microseconds>(high_resolution_clock::now() - upperBoundstart);
   cout <<"UB seq: ";
   for (int i = 0; i<solution.size(); i++) cout<<solution[i]<<" ";
   cout <<"UB: " << ub <<"\n";



   stack<Node> problems;
   Node root;
   for (int i = 0; i < flowshop.jobs; i++) root.left.push_back(i);
   for (int m = 0; m < flowshop.machines; m++) root.completion.push_back(0);

   problems.push(root);
   //solve problem recursively
   int nodesDecomposed = 0;
   while (problems.size() > 0){
      Node node = problems.top();
      problems.pop();

      nodesDecomposed++;
      // cout<< "nodes processed: " << count <<"\n";
      // count++;

      // cout << "scheduled: [ ";
      // for (int i =0; i<node.scheduled.size(); i++) cout << node.scheduled[i] << " ";
      // cout << "], left: [ ";
      // for (int i =0; i<node.left.size(); i++) cout << node.left[i] << " ";
      // cout <<"]\n";

      // if there are more than 1 jobs left unscheduled and uper bound is known
      if (node.left.size() > 1){
         auto boundStart = high_resolution_clock::now();
            int lb = bound(&flowshop, &node); //get lb
            boundingTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundStart);
            if (lb < ub){
               auto branchStart = high_resolution_clock::now(); 
               for (int i =0; i < node.left.size(); i++){
                  Node child;
                  int newJob = node.left[i];
                  child.scheduled.assign(node.scheduled.begin(), node.scheduled.end());
                  child.scheduled.push_back(newJob); // schedule i'th element
                  child.left.assign(node.left.begin(), node.left.end()); // assign unscheduled jobs from parent
                  child.left.erase(child.left.begin()+i); // erase i'th element
                  child.completion.push_back(node.completion[0]+flowshop.operations[0][newJob]);
                  for (int m = 1; m < flowshop.machines; m++) child.completion.push_back(max(child.completion[m-1],node.completion[m]) +flowshop.operations[m][newJob]);
                  problems.push(child);
               }
              branchingTime +=duration_cast<microseconds>(high_resolution_clock::now() - branchStart);
            } 
            // else {cout<<"PRUNED\n";}
      } else { // if one node is left
      int makespan = 0;
      int job = node.left.back();
         for (int m = 1; m < flowshop.machines; m++){
            makespan = max(makespan, node.completion[m]) +flowshop.operations[m][job];
         } 
         // int candidateBound = upperBound(flowshop, node);

          if (makespan < ub || ub == -1){
            ub = makespan;
            solution.assign(node.scheduled.begin(), node.scheduled.end());
            solution.push_back(node.left.back());
            cout<<"NEW SOLUTION: "<<ub<<"\n";
            }
         } 
      }
      auto stop = high_resolution_clock::now(); 
      auto duration = duration_cast<microseconds>(stop - start); 
  
      cout << "Execution time: " << duration.count() << " microseconds" << endl; 
      cout << "Nodes decomposed: " << nodesDecomposed << endl; 
      cout << "Branching time: " << branchingTime.count() << " microseconds" << endl; 
      cout << "Bounding time: " << boundingTime.count() << " microseconds" << endl; 
      cout << "Initial UB Bounding time: " << upperBoundTime.count() << " microseconds" << endl; 
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
            // for (int m = 0; m < flowshop.machines; m++){
            //    for (int j = 0;  j < flowshop.jobs; j++){
            //       cout<<  flowshop.operations[m] [j] << " ";
            // }
            //    cout<<"\n";
            // }
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
