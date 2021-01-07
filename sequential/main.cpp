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
#include <stack> 
using namespace std::chrono; 
using namespace std;

   auto start = high_resolution_clock::now(); 
   auto boundingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto makespanCountTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto otherBoundOperationsTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundCalculationTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto partialSeq0Time = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundATime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundBTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());

   unsigned long long int nodesDecomposed = 0;
   int ub = 999999;
  

struct JobLength{
   int index;
   int length;
   JobLength(){
   }
};

struct PartialNode{
   int lb;
};

struct Node {
   vector<int> left;
   vector<int> s1;
   vector<int> s2;
   int* c1;
   int* c2;
   int lb;
   int* mSum;
   int depth;
};

struct FSPspace {
  int machines;
  int jobs;
  int** operations;
  int** jobForwardSum;
  int** jobBackwardSum;
};


bool compareJobLengths(JobLength j1, JobLength j2){
   return j1.length > j2.length;
}

int calculateSequence(vector<int>* jobs, FSPspace* flowshop){
  int* completionTimes = new int [flowshop->machines];
  for (int m = 0; m < flowshop->machines; m++){
     completionTimes[m] = 0;
  }
   for (auto jobIndex: *jobs){
      completionTimes[0]+= flowshop->operations[0][jobIndex];
         for (int m = 1; m<flowshop->machines; m++){
            completionTimes[m] = max(completionTimes[m-1], completionTimes[m]) + flowshop->operations[m][jobIndex];
   }
   }
   int result = completionTimes[flowshop->machines-1];
   delete [] completionTimes;
   return result;
}

tuple<vector<int>, int> findBestSequence(vector<int> scheduled, int unscheduled, FSPspace *flowshop){
   vector<int> partial;
   vector<int> bestSequence;
   bestSequence.assign(scheduled.begin(), scheduled.end());
   bestSequence.insert(bestSequence.begin(), unscheduled);
    int best = calculateSequence(&bestSequence, flowshop);
    for (int i = 1; i <= scheduled.size(); i++){
       partial.assign(scheduled.begin(), scheduled.end());
       partial.insert(partial.begin()+i, unscheduled);
       int makespan = calculateSequence(&partial, flowshop);
       if (makespan < best){
         best = makespan;
         bestSequence.assign(partial.begin(), partial.end());
       }
    }
    return make_tuple(bestSequence, best);
}


tuple<vector<int>, int> initilizeUpperBound(FSPspace* flowshop){
   vector<JobLength> jobLengths;
   vector<int> schedule;
   for (int j = 0; j< flowshop->jobs; j++){
      JobLength jobLength;
      jobLength.length = 0;
      jobLength.index = j;
      for (int m = 0; m<flowshop->machines; m++){
         jobLength.length+=flowshop->operations[m][j];
         }
      jobLengths.push_back(jobLength);
   }
   sort(jobLengths.begin(), jobLengths.end(), compareJobLengths);
   schedule.push_back(jobLengths.at(0).index);
   int best;
   for (int i = 1; i < flowshop->jobs; i++){
      int newBest;
      vector<int> seq;
     tie(seq, newBest) = findBestSequence(schedule, jobLengths.at(i).index, flowshop);
      schedule.assign(seq.begin(), seq.end());
      best = newBest;
   }
   return make_tuple(schedule, best);
}

int getMinQ(vector<int> *jobsLeft, FSPspace *flowshop, int machine){
   vector<int>values;
   for (auto j: *jobsLeft){
      values.push_back(flowshop->jobBackwardSum[machine][j]);
   }
   return *min_element(values.begin(), values.end());
}

int getMinR(vector<int> *jobsLeft, FSPspace  *flowshop, int machine){
   vector<int>values;
   for (auto j: *jobsLeft){
      values.push_back(flowshop->jobForwardSum[machine][j]);
   }
   return *min_element(values.begin(), values.end());
}

bool compareNodes(Node *node1, Node *node2){
   return node1->lb > node2->lb;
}

void deleteNode(Node* node){ 
   delete [] node->c1;
   delete [] node->c2;
   delete [] node->mSum;
   delete(node);
}

Node* boundAndCreateNode(Node *node, FSPspace *flowshop, int j){
            auto boundingStart = high_resolution_clock::now();
            auto boundAStart = high_resolution_clock::now();
            int job = node->left[j];
            int depth = node->depth + 1;
            auto startBound=high_resolution_clock::now();
            auto otherOPTime = high_resolution_clock::now();
            vector<int>bounds;
            int *newMsum = new int [flowshop->machines];
            vector<int>left = node->left;
            left.erase(left.begin()+j);


            for (int m = 0; m<flowshop->machines; m++){
               newMsum[m] = node->mSum[m] - flowshop->operations[m][job];
            }

            if (depth % 2 == 1){ // (o1 j, o2)
            int *c1 = new int [flowshop->machines];
            c1[0] = node->c1[0] + flowshop->operations[0][job];

            //Machine 1 bound
            if (node->s2.empty()){
               auto seq0start = high_resolution_clock::now();
               bounds.push_back(newMsum[0] + getMinQ(&left, flowshop, 0));
               partialSeq0Time += duration_cast<microseconds>(high_resolution_clock::now() - seq0start);
            } else {
               bounds.push_back(newMsum[0] + node->c2[0]); 
            }
            //bounds for the rest of machines
            for (int m = 1; m < flowshop->machines; m++){
               //update c1 with the new job times
               c1[m] = max(c1[m-1], node->c1[m]) + flowshop->operations[m][job];
               //if childs o2 is empty
               if (node->s2.empty()){
                  bounds.push_back(c1[m] + newMsum[m] + getMinQ(&left, flowshop, m));
               } else {
                  bounds.push_back(c1[m] + newMsum[m] + node->c2[m]);
               }
            }
            int lb = *max_element(bounds.begin(), bounds.end());
               boundATime+=duration_cast<microseconds>(high_resolution_clock::now() - boundAStart);
            cout<<"LB: "<< lb<<"\n";
            if (lb < ub){
               auto boundBStart = high_resolution_clock::now();
               Node* child = new Node;
               child->s2 = node->s2;
               child->s1 = node->s1;
               child->s1.push_back(job);
               child->c1=c1;
               child->c2 = new int [flowshop->machines];
               std::memcpy(child->c2, node->c2, sizeof(int)*flowshop->machines); //can rely on parent c1
               child->mSum = newMsum;
               child->left = left;
               child->lb =lb;
               child->depth = depth;
               boundBTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundBStart);
               boundingTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundingStart);
               return child;
            } else {
               cout<<"PRUNED\n";
               delete [] newMsum;
               delete [] c1;
               boundingTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundingStart);
               return NULL;
            }

            }else{  // (o1, j o2)
               int *c2 = new int [flowshop->machines];
               c2[flowshop->machines-1] = node->c2[flowshop->machines-1] + flowshop->operations[flowshop->machines-1][job];

               //machine m bound
               if (node->s1.empty()){
                  auto seq0start = high_resolution_clock::now();
                  bounds.push_back(newMsum[flowshop->machines-1] + getMinQ(&left, flowshop, flowshop->machines-1));
                  partialSeq0Time += duration_cast<microseconds>(high_resolution_clock::now() - seq0start);
               } else {
                  bounds.push_back(newMsum[flowshop->machines-1] + node->c1[flowshop->machines-1]); 
                  }
            //machines m-1 -> 1 bounds
            for (int m = flowshop->machines-2; m >= 0; m--){
               //update c2 with the new job times
               c2[m] = max(c2[m+1], node->c2[m]) + flowshop->operations[m][job];
               //if childs o1 is empty
               if (node->s1.empty()){
                  bounds.push_back(c2[m] + newMsum[m] + getMinR(&left, flowshop, m));
               } else {
                  bounds.push_back(c2[m] + newMsum[m] + node->c1[m]);
               }
            }
            int lb = *max_element(bounds.begin(), bounds.end());
               boundATime+=duration_cast<microseconds>(high_resolution_clock::now() - boundAStart);
               cout<<"LB: "<< lb<<"\n";
            if (lb < ub){
               auto boundBStart = high_resolution_clock::now();
               Node* child = new Node;
               child->s1 = node->s1;
               child->s2 = node->s2;
               child->s2.insert(child->s2.begin(), job);
               child->c2 = c2;
               child->c1 = new int [flowshop->machines];
               std::memcpy(child->c1, node->c1, sizeof(int)*flowshop->machines); //can rely on parent c2
               child->mSum = newMsum;
               child->left = left;
               child->lb = lb;
               child->depth = depth;
               boundBTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundBStart);
               boundingTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundingStart);
               return child; 
            } else {
               cout<<"PRUNED\n";
               delete [] newMsum;
               delete [] c2;
                boundingTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundingStart);
               return NULL;
            }

         } 
}


//RECURSIVE SOLUTION HERE
void solve(FSPspace* flowshop){

   //initialize problem
   vector<int> solution;
   auto upperBoundstart = high_resolution_clock::now();
   //  tie(solution, ub) = initilizeUpperBound(flowshop);
   auto upperBoundTime = duration_cast<microseconds>(high_resolution_clock::now() - upperBoundstart);

   cout <<"UB seq: ";
   for (int i = 0; i<solution.size(); i++) cout<<solution[i]<<" ";
   cout <<"UB: " << ub <<"\n";

   vector<Node*> problems;
   Node *root = new Node;
   root->c1 = new int [flowshop->machines];
   root->c2 = new int [flowshop->machines];
   root->mSum = new int [flowshop->machines];
   root->lb = 0; //lb for root doesnt matter as long as it's less than UB
   root->depth = 0;

   //set o1, o2 makespans to 0 and initialize sum(Pkj) to 0
   for (int m = 0; m < flowshop->machines; m++){
      root->mSum [m] = 0;
      root->c1 [m] = 0;
      root->c2 [m] = 0;
   }

   //set all sum(Pkj) unsheduled job sum times on all machines
   for (int j = 0; j < flowshop->jobs; j++){ 
      root->left.push_back(j);
      for (int m = 0; m < flowshop->machines; m++){
         root->mSum[m] += flowshop->operations[m][j];
      }
   } 

   problems.push_back(root);
   //solve problem recursively
   while (problems.size() > 0){

      Node* node = problems.back();
      problems.pop_back();

      vector<Node*> newProblems;

      // if there are more than 1 jobs left
      if (node->left.size() > 1){
        if(node->lb < ub){
           nodesDecomposed++;
            for (int j = 0; j<node->left.size(); j++){
               Node *child = boundAndCreateNode(node, flowshop, j);
               if (child!=NULL){
                  // newProblems.push_back(child);
                  newProblems.insert(newProblems.begin(), child);
               }else{
                  delete(child);
               }
         }

      // sort the nodes in descenging lb order and append it to problem list
        auto otherOPTime = high_resolution_clock::now();
        if (!newProblems.empty()){
         // sort(newProblems.begin(), newProblems.end(), compareNodes);
         std::copy (newProblems.begin(), newProblems.end(), std::back_inserter(problems));
        }
         deleteNode(node);
         otherBoundOperationsTime+=duration_cast<microseconds>(high_resolution_clock::now() - otherOPTime);
        } else {
            cout<<"PRUNED LATER\n";
           deleteNode(node);
        }
      } else { // if one node is left
      vector<int> candiate;
      candiate.assign(node->s1.begin(), node->s1.end());
      candiate.push_back(node->left.front());
      std::copy(node->s2.begin(), node->s2.end(), std::back_inserter(candiate));
      auto upperBoundstart = high_resolution_clock::now();
      int makespan = calculateSequence(&candiate, flowshop);
      makespanCountTime+=duration_cast<microseconds>(high_resolution_clock::now() - upperBoundstart);
      deleteNode(node);
      if (makespan < ub){
            ub = makespan;
            solution.assign(candiate.begin(), candiate.end());
            }
         } 
      }
      auto stop = high_resolution_clock::now(); 
      auto duration = duration_cast<microseconds>(stop - start); 
  
      cout << "Execution time: " << duration.count() << " microseconds" << endl;
      cout << "Nodes decomposed: " << nodesDecomposed << endl; 
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


FSPspace* parseFile(string fileName){
   int* chunks;
   ifstream file;
   file.open(fileName);
   FSPspace* flowshop = new FSPspace;
   if (file){ 
       if (file.is_open()){
         string line;
         getline(file, line);  // get the first line containing two values: number of jobs and number of machines
            chunks = parseLine(line, 2);
            flowshop->jobs = chunks[0];
            flowshop->machines = chunks[1];
            delete[] chunks;
            flowshop->operations = new int* [flowshop->machines];
            flowshop->jobForwardSum = new int* [flowshop->machines];
            flowshop->jobBackwardSum = new int* [flowshop->machines];
            cout<< "jobs: " <<flowshop->jobs << " machines:" << flowshop->machines << "\n";
            for (int m = 0; m < flowshop->machines; m++){
                  flowshop->operations[m] = new int [flowshop->jobs];
                  flowshop->jobForwardSum[m] = new int [flowshop->jobs];
                  flowshop->jobBackwardSum[m] = new int [flowshop->jobs];
                  getline(file, line); 
                  chunks = parseLine(line, flowshop->jobs);
                  for (int j = 0; j< flowshop->jobs; j++){
                     flowshop->operations[m][j] = chunks[j];
                  }
                  delete[] chunks;
            }

            //set jobForwarsSum[0] and jobBackward[m-1] initial 0
            for (int j = 0; j< flowshop->jobs; j++){
               flowshop->jobForwardSum [0][j] = 0;
               flowshop->jobBackwardSum[flowshop->machines-1][j] = 0;
            }
            for (int j = 0; j<flowshop->jobs; j++){
               for (int f = 1; f <flowshop->machines; f++){
                  flowshop->jobForwardSum[f][j] = flowshop->jobForwardSum[f-1][j] + flowshop->operations[f-1][j];
               }
               for (int b = flowshop->machines-2; b>=0; b--){
                  flowshop->jobBackwardSum[b][j] = flowshop->jobBackwardSum[b+1][j] + flowshop->operations[b+1][j];
               }
            }
      


            for (int m = 0; m < flowshop->machines; m++){
               for (int j = 0;  j < flowshop->jobs; j++){
                  cout<<  flowshop->operations[m] [j] << " ";
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
      FSPspace* flowshop = parseFile(argv[1]);
      solve(flowshop);
    for (int m = 0; m<flowshop->machines; m++){
        delete[]  (flowshop->jobBackwardSum[m]);
        delete[] (flowshop->jobForwardSum[m]);
        delete[] (flowshop->operations[m]);
      }
    delete[]  (flowshop->jobBackwardSum);
    delete[] (flowshop->jobForwardSum);
    delete[] (flowshop->operations);
    delete(flowshop);
}
