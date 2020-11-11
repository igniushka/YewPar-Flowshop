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

struct JobLength{
   int index;
   int length;
   JobLength(){
   }
};

struct Node {
   vector<int> left;
   vector<int> s1;
   vector<int> s2;
   int* c1;
   int* c2;
   int lb;
   int* mSum;
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

void deleteNode(Node node){
   delete [] node.mSum;
   delete [] node.c1;
   delete [] node.c2;
}

bool compareNodes(Node node1, Node node2){
   return node1.lb > node2.lb;
}


//RECURSIVE SOLUTION HERE
void solve(FSPspace flowshop){
   auto start = high_resolution_clock::now(); 
   auto branchingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto makespanCountTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto otherBoundOperationsTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundCalculationTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto partialSeq0Time = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());


 
   //initialize problem
   int ub;
   vector<int> solution;
   auto upperBoundstart = high_resolution_clock::now();
   tie(solution, ub) = initilizeUpperBound(flowshop);
   auto upperBoundTime = duration_cast<microseconds>(high_resolution_clock::now() - upperBoundstart);

   cout <<"UB seq: ";
   for (int i = 0; i<solution.size(); i++) cout<<solution[i]<<" ";
   cout <<"UB: " << ub <<"\n";

   vector<Node> problems;
   Node root;
   root.c1 = new int [flowshop.machines];
   root.c2 = new int [flowshop.machines];
   root.mSum = new int [flowshop.machines];
   root.lb = 0; //lb for root doesnt matter as long as it's less than UB

   //set o1, o2 makespans to 0 and initialize sum(Pkj) to 0
   for (int m = 0; m < flowshop.machines; m++){
      root.mSum [m] = 0;
      root.c1 [m] = 0;
      root.c2 [m] = 0;
   }

   //set all sum(Pkj) unsheduled job sum times on all machines
   for (int j = 0; j < flowshop.jobs; j++){ 
      root.left.push_back(j);
      for (int m = 0; m < flowshop.machines; m++){
         root.mSum[m] += flowshop.operations[m][j];
      }
   } 

   problems.push_back(root);
   //solve problem recursively
   int count = 0;
   while (problems.size() > 0){
      count ++;
   //  cout<<count<<"\n";
      Node node = problems.back();
      problems.pop_back();



      vector<Node> newProblems;

      // if there are more than 1 jobs left unscheduled and uper bound is known
      if (node.left.size() > 1){
        if(node.lb < ub){
            for (int j = 0; j<node.left.size(); j++){
            int job = node.left[j];
            auto startBound=high_resolution_clock::now();
            auto otherOPTime = high_resolution_clock::now();
            Node child1; // (o1 j, o2)
            Node child2; // (o1, j o2)
            vector<int>c1bounds;
            vector<int>c2bounds;

            
            //set o1 and o2 on both nodes
            child1.s1.assign(node.s1.begin(), node.s1.end());
            child1.s2.assign(node.s2.begin(), node.s2.end()); //can rely on parent s2
            child2.s1.assign(node.s1.begin(), node.s1.end());
            child2.s2.assign(node.s2.begin(), node.s2.end());

            

            //set up left jobs
            child1.left.assign(node.left.begin(), node.left.end());
            child1.left.erase(child1.left.begin()+j);
            child2.left.assign(child1.left.begin(), child1.left.end());


            //insert jobs to both childred
            child1.s1.push_back(job);
            child2.s2.insert(child2.s2.begin(), job);

            // set up sum(Pkj) on all machines. subtract the selected job from sum
            child1.mSum = new int [flowshop.machines];
            child2.mSum = new int [flowshop.machines];
            otherBoundOperationsTime+=duration_cast<microseconds>(high_resolution_clock::now() - otherOPTime);

            auto boundCalcStart = high_resolution_clock::now();

            for (int m = 0; m<flowshop.machines; m++){
               child1.mSum[m] = node.mSum[m] - flowshop.operations[m][job];
               child2.mSum[m] = node.mSum[m] - flowshop.operations[m][job];
            }
            boundCalcStart = high_resolution_clock::now();
            boundCalculationTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundCalcStart);

            otherOPTime=  high_resolution_clock::now();

            //set up c1 and c2 for Forward child
            child1.c1 = new int [flowshop.machines];
            child1.c2 = new int [flowshop.machines];
            std::memcpy(child1.c2, node.c2, sizeof(int)*flowshop.machines); //can rely on parent c2
            child1.c1[0] = node.c1[0] + flowshop.operations[0][job];

            //set up c1 and c2 for Backward child
            child2.c2 = new int [flowshop.machines];
            child2.c1 = new int [flowshop.machines];
            std::memcpy(child2.c1, node.c1, sizeof(int)*flowshop.machines); //can rely on parent c2
            child2.c2[flowshop.machines-1] = node.c2[flowshop.machines-1] + flowshop.operations[flowshop.machines-1][job];

            otherBoundOperationsTime+=duration_cast<microseconds>(high_resolution_clock::now() - otherOPTime);

            boundCalcStart = high_resolution_clock::now();

            // machine 1 bound for Forward child
            if (child1.s2.empty()){
               auto seq0start = high_resolution_clock::now();
               c1bounds.push_back(child1.mSum[0] + getMinQ(&child1.left, &flowshop, 0));
               partialSeq0Time += duration_cast<microseconds>(high_resolution_clock::now() - seq0start);
            } else {
               c1bounds.push_back(child1.mSum[0] + child1.c2[0]); 
            }
            // machine m bound for Backward child
            if (child2.s1.empty()){
               auto seq0start = high_resolution_clock::now();
               c1bounds.push_back(child2.mSum[flowshop.machines-1] + getMinQ(&child2.left, &flowshop, flowshop.machines-1));
               partialSeq0Time += duration_cast<microseconds>(high_resolution_clock::now() - seq0start);
            } else {
               c1bounds.push_back(child2.mSum[flowshop.machines-1] + child2.c1[flowshop.machines-1]); 
            }

            // machine 2 -> m bounds for Forward child
            for (int m = 1; m < flowshop.machines; m++){
               //update c1 with the new job times
               child1.c1[m] = max(child1.c1[m-1], node.c1[m]) + flowshop.operations[m][job];
               // init bound for machine m
               int bound = child1.c1[m] + child1.mSum[m];
               //if childs o2 is empty
               if (child1.s2.empty()){
                  c1bounds.push_back(bound + getMinQ(&child1.left, &flowshop, m));
               } else {
                  c1bounds.push_back(bound + child1.c2[m]);
               }
            }

            // machine m-1 -> 1 bounds for Backward child
            for (int m = flowshop.machines-2; m >= 0; m--){
               //update c2 with the new job times
               child2.c2[m] = max(child2.c2[m+1], node.c2[m]) + flowshop.operations[m][job];
               // init bound for machine m
               int bound = child2.c2[m] + child2.mSum[m];
               //if childs o1 is empty
               if (child2.s1.empty()){
                  c2bounds.push_back(bound + getMinR(&child2.left, &flowshop, m));
               } else {
                  c2bounds.push_back(bound + child2.c1[m]);
               }
            } 
               
            // get max lb for Forward and Backward child
            child1.lb = *max_element(c1bounds.begin(), c1bounds.end());
            child2.lb = *max_element(c2bounds.begin(), c2bounds.end());


            boundCalculationTime+=duration_cast<microseconds>(high_resolution_clock::now() - boundCalcStart);


            boundingTime+=duration_cast<microseconds>(high_resolution_clock::now() - startBound);


            // Select child with lesser LB and compare it with UB
            if (child1.lb < child2.lb){
               // deleteNode(child2);
               if (child1.lb < ub){
                  newProblems.push_back(child1);
               } else {
                  // deleteNode(child1);
                  }
            } else{
               // deleteNode(child1);
               if (child2.lb < ub){
                  newProblems.push_back(child2);
               } else {
                  // deleteNode(child2);
                  }
            } 
         }


         // sort the nodes in descenging lb order and append it to problem list
         
        auto otherOPTime = high_resolution_clock::now();


         sort(newProblems.begin(), newProblems.end(), compareNodes);

         // for (auto p: newProblems){
         //    cout << p.lb <<" ";
         // } cout <<"\n";

         std::copy (newProblems.begin(), newProblems.end(), std::back_inserter(problems));

  

         otherBoundOperationsTime+=duration_cast<microseconds>(high_resolution_clock::now() - otherOPTime);


        }
      } else { // if one node is left
      vector<int> candiate;
      candiate.assign(node.s1.begin(), node.s1.end());
      candiate.push_back(node.left.front());
      std::copy(node.s2.begin(), node.s2.end(), std::back_inserter(candiate));
      auto upperBoundstart = high_resolution_clock::now();
      int makespan = calculateSequence(candiate, flowshop);
      makespanCountTime+=duration_cast<microseconds>(high_resolution_clock::now() - upperBoundstart);
      if (makespan < ub){
            ub = makespan;
            solution.assign(candiate.begin(), candiate.end());
            solution.push_back(node.left.back());
            // for (auto job: solution) cout<<job<<" ";
            cout<<"\nNEW SOLUTION: "<<ub<<"\n";
            // exit(0);
            }
         } 
      }
      auto stop = high_resolution_clock::now(); 
      auto duration = duration_cast<microseconds>(stop - start); 
  
      cout << "Execution time: " << duration.count() << " microseconds" << endl; 
      cout << "Makespan counting time:" <<makespanCountTime.count() << " microseconds" << endl; 
      cout << "Branching time: " << branchingTime.count() << " microseconds" << endl; 
      cout << "Bounding time: " << boundingTime.count() << " microseconds" << endl; 
      cout << "Bounding calculation time: " << boundCalculationTime.count() << " microseconds" << endl; 
      cout << "Bounding partial seq calculation time: " << partialSeq0Time.count() << " microseconds" << endl;       
      cout << "Bounding other OP time: " << otherBoundOperationsTime.count() << " microseconds" << endl; 
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
            flowshop.jobForwardSum = new int* [flowshop.machines];
            flowshop.jobBackwardSum = new int* [flowshop.machines];
            cout<< "jobs: " <<flowshop.jobs << " machines:" << flowshop.machines << "\n";
            for (int m = 0; m < flowshop.machines; m++){
                  flowshop.operations[m] = new int [flowshop.jobs];
                  flowshop.jobForwardSum[m] = new int [flowshop.jobs];
                  flowshop.jobBackwardSum[m] = new int [flowshop.jobs];
                  getline(file, line); 
                  chunks = parseLine(line, flowshop.jobs);
                  for (int j = 0; j< flowshop.jobs; j++){
                     flowshop.operations[m][j] = chunks[j];
                  }
            }

            //set jobForwarsSum[0] and jobBackward[m-1] initial 0
            for (int j = 0; j< flowshop.jobs; j++){
               flowshop.jobForwardSum [0][j] = 0;
               flowshop.jobBackwardSum[flowshop.machines-1][j] = 0;
            }
            for (int j = 0; j<flowshop.jobs; j++){
               for (int f = 1; f <flowshop.machines; f++){
                  flowshop.jobForwardSum[f][j] =  flowshop.jobForwardSum[f-1][j] + flowshop.operations[f-1][j];
               }
               for (int b = flowshop.machines-2; b>=0; b--){
                  flowshop.jobForwardSum[b][j] =  flowshop.jobForwardSum[b+1][j] + flowshop.operations[b+1][j];
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
