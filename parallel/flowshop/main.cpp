#include <climits>
#include <cstdio>
#include <cstdlib>
#include <array>
#include <sstream>
#include <iterator>
#include <list>
#include <bits/stdc++.h>
#include <vector>
#include <stack> 
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <regex>
#include <exception>
#include <chrono>
#include <hpx/hpx_init.hpp>
#include <hpx/include/iostreams.hpp>
#include "YewPar.hpp"
#include "util/func.hpp"
#include "skeletons/Seq.hpp"
#include "skeletons/DepthBounded.hpp"
#include "skeletons/Ordered.hpp"
#include "skeletons/Budget.hpp"
#include "skeletons/StackStealing.hpp"
#include <boost/serialization/access.hpp>

using namespace std::chrono; 
using namespace std;

   auto start = high_resolution_clock::now(); 
   auto branchingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto makespanCountTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto otherBoundOperationsTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundCalculationTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto partialSeq0Time = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   int nodesProcessed = 0;
   int ub = 999999;
  

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
   int depth;
};

struct FSPSolution {
  std::vector<int> sequence;
  unsigned makespan;

  template <class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & sequence;
    ar & makespan;
  }
};

struct FSPNode {
  friend class boost::serialization::access;
  FSPSolution sol;
   vector<int> left;
   vector<int> s1;
   vector<int> s2;
   int lb;
   int depth;
   int* c1;
   int* c2;
   int* mSum;
      // array<int, M> c1;
   // array<int, M> c2;
   // array<int, M> mSum;

  int getObj() const {
   return sol.makespan;
  }

  template <class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & sol;
    ar & left;
    ar & s1;
    ar & s2;
  }
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

unsigned calculateSequence(vector<int>* jobs, const FSPspace* flowshop){
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


// tuple<vector<int>, int> initilizeUpperBound(FSPspace* flowshop){
   FSPSolution initilizeUpperBound(FSPspace* flowshop){

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
   unsigned best;
   for (int i = 1; i < flowshop->jobs; i++){
      int newBest;
      vector<int> seq;
     tie(seq, newBest) = findBestSequence(schedule, jobLengths.at(i).index, flowshop);
      schedule.assign(seq.begin(), seq.end());
      best = newBest;
   }
   // return make_tuple(schedule, best);
   return FSPSolution{schedule, best};
}

int getMinQ(vector<int> &jobsLeft, FSPspace &flowshop, int machine){
   vector<int>values;
   for (auto j: jobsLeft){
      values.push_back(flowshop.jobBackwardSum[machine][j]);
   }
   return *min_element(values.begin(), values.end());
}

int getMinR(vector<int> &jobsLeft, FSPspace  &flowshop, int machine){
   vector<int>values;
   for (auto j: jobsLeft){
      values.push_back(flowshop.jobForwardSum[machine][j]);
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

FSPSolution makeSolution(const FSPspace & space, const FSPNode & node){
   vector<int> candidate;
   candidate.assign(node.s1.begin(), node.s2.end());
   std::copy(node.s2.begin(), node.s2.end(), std::back_inserter(candidate));
   return FSPSolution{candidate, calculateSequence(&candidate, &space)};
}

FSPNode boundAndCreateNode(FSPNode &node, FSPspace &flowshop, int j){
            int job = node.left[j];
            int depth = node.depth + 1;
            auto startBound=high_resolution_clock::now();
            auto otherOPTime = high_resolution_clock::now();
            vector<int>bounds;
            int *newMsum = new int [flowshop.machines];
            vector<int>left = node.left;
            left.erase(left.begin()+j);


            for (int m = 0; m<flowshop.machines; m++){
               newMsum[m] = node.mSum[m] - flowshop.operations[m][job];
            }

            if (depth % 2 == 1){ // (o1 j, o2)
            int *c1 = new int [flowshop.machines];
            c1[0] = node.c1[0] + flowshop.operations[0][job];

            //Machine 1 bound
            if (node.s2.empty()){
               auto seq0start = high_resolution_clock::now();
               bounds.push_back(newMsum[0] + getMinQ(left, flowshop, 0));
               partialSeq0Time += duration_cast<microseconds>(high_resolution_clock::now() - seq0start);
            } else {
               bounds.push_back(newMsum[0] + node.c2[0]); 
            }
            //bounds for the rest of machines
            for (int m = 1; m < flowshop.machines; m++){
               //update c1 with the new job times
               c1[m] = max(c1[m-1], node.c1[m]) + flowshop.operations[m][job];
               //if childs o2 is empty
               if (node.s2.empty()){
                  bounds.push_back(c1[m] + newMsum[m] + getMinQ(left, flowshop, m));
               } else {
                  bounds.push_back(c1[m] + newMsum[m] + node.c2[m]);
               }
            }
            int lb = *max_element(bounds.begin(), bounds.end());
            // if (lb < ub){
               FSPNode child;
               child.s2 = node.s2;
               child.s1 = node.s1;
               child.s1.push_back(job);
               child.c1=c1;
               child.c2 = new int [flowshop.machines];
               std::memcpy(child.c2, node.c2, sizeof(int)*flowshop.machines); //can rely on parent c1
               child.mSum = newMsum;
               child.left = left;
               child.lb =lb;
               child.depth = depth;
               if(child.left.size() == 0){
                   child.sol = makeSolution(flowshop, child);
               } else {
                  child.sol.makespan = INT_MAX;
               }
               return child;
            // } else {
            //    delete [] newMsum;
            //    delete [] c1;
            //    return NULL;
            // }

            }else{  // (o1, j o2)
               int *c2 = new int [flowshop.machines];
               c2[flowshop.machines-1] = node.c2[flowshop.machines-1] + flowshop.operations[flowshop.machines-1][job];

               //machine m bound
               if (node.s1.empty()){
                  auto seq0start = high_resolution_clock::now();
                  bounds.push_back(newMsum[flowshop.machines-1] + getMinQ(left, flowshop, flowshop.machines-1));
                  partialSeq0Time += duration_cast<microseconds>(high_resolution_clock::now() - seq0start);
               } else {
                  bounds.push_back(newMsum[flowshop.machines-1] + node.c1[flowshop.machines-1]); 
                  }
            //machines m-1 -> 1 bounds
            for (int m = flowshop.machines-2; m >= 0; m--){
               //update c2 with the new job times
               c2[m] = max(c2[m+1], node.c2[m]) + flowshop.operations[m][job];
               //if childs o1 is empty
               if (node.s1.empty()){
                  bounds.push_back(c2[m] + newMsum[m] + getMinR(left, flowshop, m));
               } else {
                  bounds.push_back(c2[m] + newMsum[m] + node.c1[m]);
               }
            }
            int lb = *max_element(bounds.begin(), bounds.end());
            // if (lb < ub){
               FSPNode child;
               child.s1 = node.s1;
               child.s2 = node.s2;
               child.s2.insert(child.s2.begin(), job);
               child.c2 = c2;
               child.c1 = new int [flowshop.machines];
               std::memcpy(child.c1, node.c1, sizeof(int)*flowshop.machines); //can rely on parent c2
               child.mSum = newMsum;
               child.left = left;
               child.lb = lb;
               child.depth = depth;
               if(child.left.size() == 0){
                   child.sol = makeSolution(flowshop, child);
               } else {
                  child.sol.makespan = INT_MAX;
               }
               return child; 
            // } else {
            //    delete [] newMsum;
            //    delete [] c2;
            //    return NULL;
            // }

         } 
}




// template <unsigned numItems>
struct GenNode : YewPar::NodeGenerator<FSPNode, FSPspace> {
  std::vector<int> items;
  int pos;
  std::reference_wrapper<const FSPspace > space;
  std::reference_wrapper<const FSPNode> n;

  GenNode (const FSPspace & space, const FSPNode & n) :
      pos(0), space(std::cref(space)), n(std::cref(n)) {
            cout<<"INIT GENNODE\n";
    this->numChildren = n.left.size();

  }

  FSPNode next() override {

auto parent = n.get();
auto flowshop = space.get();
   FSPNode child = boundAndCreateNode(parent, flowshop, pos);
    ++pos;
    cout<<"RETURNING CHILD UB: "<< child.sol.makespan << " LB: "<< child.lb<<"\n";
    return child;
  }
};


unsigned lowerBound(const FSPspace & space, const FSPNode & n) {
   //TODO implement the LB calculation here
  return n.lb;
}



typedef func<decltype(&lowerBound), &lowerBound> upperBound_func;

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
            delete[] chunks;
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
                  delete[] chunks;
            }

            //set jobForwarsSum[0] and jobBackward[m-1] initial 0
            for (int j = 0; j< flowshop.jobs; j++){
               flowshop.jobForwardSum [0][j] = 0;
               flowshop.jobBackwardSum[flowshop.machines-1][j] = 0;
            }
            for (int j = 0; j<flowshop.jobs; j++){
               for (int f = 1; f <flowshop.machines; f++){
                  flowshop.jobForwardSum[f][j] = flowshop.jobForwardSum[f-1][j] + flowshop.operations[f-1][j];
               }
               for (int b = flowshop.machines-2; b>=0; b--){
                  flowshop.jobBackwardSum[b][j] = flowshop.jobBackwardSum[b+1][j] + flowshop.operations[b+1][j];
               }
            }
            for (int m = 0; m < flowshop.machines; m++){
               for (int j = 0;  j < flowshop.jobs; j++){
                  cout<<  flowshop.operations[m] [j] << " ";
            }
               cout<<"\n";
            }
            cout<<"TEST1\n";
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

int hpx_main(boost::program_options::variables_map & opts) {
  auto inputFile = opts["input-file"].as<std::string>();
   auto skeletonType = opts["skeleton"].as<std::string>();
   FSPspace space = parseFile(inputFile);
   cout<<"TEST2\n";
   FSPNode root;
   FSPSolution initial = initilizeUpperBound(&space);
   vector<int> rootSol = {};
   // root.sol = {rootSol, 0};
   root.sol = initilizeUpperBound(&space);
   cout<<"INITIAL MAKESPAN SET UP \n";
   root.c1 = new int [space.machines];
   root.c2 = new int [space.machines];
   root.mSum = new int [space.machines];
   root.lb = 0; //lb for root doesnt matter as long as it's less than UB
   root.depth = 0;
   //set o1, o2 makespans to 0 and initialize sum(Pkj) to 0
   for (int m = 0; m < space.machines; m++){
      root.mSum [m] = 0;
      root.c1 [m] = 0;
      root.c2 [m] = 0;
   }

   //set all sum(Pkj) unsheduled job sum times on all machines
   for (int j = 0; j < space.jobs; j++){ 
      root.left.push_back(j);
      for (int m = 0; m < space.machines; m++){
         root.mSum[m] += space.operations[m][j];
      }
   } 
   cout<<"ROOT SET UP \n";
   auto sol = root;

     if (skeletonType == "seq") {
      YewPar::Skeletons::API::Params<unsigned> searchParameters;
      searchParameters.initialBound = initial.makespan;

      sol = YewPar::Skeletons::Seq<GenNode,
                                   YewPar::Skeletons::API::Decision,
                                   YewPar::Skeletons::API::BoundFunction<upperBound_func>,
                                   YewPar::Skeletons::API::ObjectiveComparison<std::less<unsigned>>>
            ::search(space, root);
    
  }

         cout << "Optimal makespan: " << sol.sol.makespan <<"\n" << "Optimal job scheduling order: "; ;
      for(int i =0; i < space.jobs; i++) cout << sol.sol.sequence[i] + 1 << " ";
         cout << "\n";

               // solve(flowshop);
    for (int m = 0; m<space.machines; m++){
        delete[]  (space.jobBackwardSum[m]);
        delete[] (space.jobForwardSum[m]);
        delete[] (space.operations[m]);
      }
    delete[]  (space.jobBackwardSum);
    delete[] (space.jobForwardSum);
    delete[] (space.operations);
   }


int main(int argc, char* argv[]) {
  boost::program_options::options_description
    desc_commandline("Usage: " HPX_APPLICATION_STRING " [options]");

  desc_commandline.add_options()
    ( "skeleton",
      boost::program_options::value<std::string>()->default_value("seq"),
      "Which skeleton to use: seq, depthbound, stacksteal, budget, or ordered"
    )
    ( "input-file,f",
      boost::program_options::value<std::string>()->required(),
      "Input problem"
    )
    ( "backtrack-budget,b",
      boost::program_options::value<unsigned>()->default_value(500),
      "Number of backtracks before spawning work"
    )
    ("chunked", "Use chunking with stack stealing")
    ( "spawn-depth,d",
      boost::program_options::value<unsigned>()->default_value(0),
      "Depth in the tree to spawn until (for parallel skeletons only)"
    );

  YewPar::registerPerformanceCounters();

  return hpx::init(desc_commandline, argc, argv);
}


