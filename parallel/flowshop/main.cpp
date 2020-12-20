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


#ifndef NUMJOBS
#define NUMJOBS 20
#endif

#ifndef NUMMACHINES
#define NUMMACHINES 20
#endif


using namespace std::chrono; 
using namespace std;

   auto start = high_resolution_clock::now(); 
   auto branchingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundingTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto makespanCountTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto otherBoundOperationsTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundCalculationTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto partialSeq0Time = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());  
   auto boundAtime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());
   auto boundBTime = duration_cast<microseconds>(high_resolution_clock::now() - high_resolution_clock::now());

   unsigned long long int nodesDecomposed = 0;

struct JobLength{
   int index;
   int length;
   JobLength(){
   }
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

template <unsigned M>
struct FSPNode {
  friend class boost::serialization::access;
  FSPSolution sol;
   vector<int> left;
   vector<int> s1;
   vector<int> s2;
   int lb;
   int depth;
   array<int, M> c1;
   array<int, M> c2;
   array<int, M> mSum;

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
template <unsigned M, unsigned N>
struct FSPspace {
  int machines;
  int jobs;
  array<array<int, N>, M> operations;
  array<array<int, N>, M> jobForwardSum;
  array<array<int, N>, M> jobBackwardSum;

  template <class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & machines;
    ar & jobs;
    ar & operations;
    ar & jobBackwardSum;
    ar & jobForwardSum;
  }

};

bool compareJobLengths(JobLength j1, JobLength j2){
   return j1.length > j2.length;
}

unsigned calculateSequence(vector<int>* jobs, const FSPspace<NUMMACHINES, NUMJOBS>* flowshop){
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

tuple<vector<int>, int> findBestSequence(vector<int> scheduled, int unscheduled, FSPspace<NUMMACHINES, NUMJOBS> *flowshop){
   vector<int> partial;
   vector<int> bestSequence;
   bestSequence.assign(scheduled.begin(), scheduled.end());
   bestSequence.insert(bestSequence.begin(), unscheduled);
    unsigned best = calculateSequence(&bestSequence, flowshop);
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

   FSPSolution initilizeUpperBound(FSPspace<NUMMACHINES, NUMJOBS>* flowshop){

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

int getMinQ(vector<int> &jobsLeft, FSPspace<NUMMACHINES, NUMJOBS> &flowshop, int machine){
   vector<int>values;
   for (auto j: jobsLeft){
      values.push_back(flowshop.jobBackwardSum[machine][j]);
   }
   return *min_element(values.begin(), values.end());
}

int getMinR(vector<int> &jobsLeft, FSPspace<NUMMACHINES, NUMJOBS>  &flowshop, int machine){
   vector<int>values;
   for (auto j: jobsLeft){
      values.push_back(flowshop.jobForwardSum[machine][j]);
   }
   return *min_element(values.begin(), values.end());
}

void makeSolution(const FSPspace<NUMMACHINES, NUMJOBS> & space, FSPNode<NUMMACHINES> & node){
   for (int job: node.s2){
      node.s1.push_back(job);
          node.c1[0] = node.c1[0] + space.operations[0][job];
            for (int m = 1; m < space.machines; m++){
               node.c1[m] = max(node.c1[m-1], node.c1[m]) + space.operations[m][job];
            }
   }
   unsigned makaespan = node.c1[space.machines -1];
   node.sol = FSPSolution{node.s1, makaespan};
}

struct GenNode : YewPar::NodeGenerator<FSPNode<NUMMACHINES>, FSPspace<NUMMACHINES, NUMJOBS>> {
  std::vector<int> items;
  int pos;
  std::reference_wrapper<const FSPspace<NUMMACHINES, NUMJOBS>> space;
  std::reference_wrapper<const FSPNode<NUMMACHINES>> n;

  GenNode (const FSPspace<NUMMACHINES, NUMJOBS> & space, const FSPNode<NUMMACHINES> & n) :
      pos(0), space(std::cref(space)), n(std::cref(n)) {
      nodesDecomposed++;
    this->numChildren = n.left.size();
  }

  FSPNode<NUMMACHINES> next() override {

auto node = n.get();
auto flowshop = space.get();
            int job = node.left[pos];
            int depth = node.depth + 1;
            vector<int>bounds;
            array<int, NUMMACHINES> newMsum;
            vector<int>left = node.left;
            left.erase(left.begin()+pos);
            for (int m = 0; m<flowshop.machines; m++){
               newMsum[m] = node.mSum[m] - flowshop.operations[m][job];
            }
            if (depth % 2 == 1){ // (o1 j, o2)
            array<int, NUMMACHINES> c1;
            c1[0] = node.c1[0] + flowshop.operations[0][job];
            //Machine 1 bound
            if (node.s2.empty()){
               bounds.push_back(newMsum[0] + getMinQ(left, flowshop, 0));
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
               FSPNode<NUMMACHINES> child;
               child.s2 = node.s2;
               child.s1 = node.s1;
               child.s1.push_back(job);
                child.c1=c1;
                child.c2 = node.c2;
               child.mSum = newMsum;
               child.left = left;
               child.lb =lb;
               child.depth = depth;
               if(child.left.size() == 0){
                   makeSolution(flowshop, child);
               } else {
                  child.sol.makespan = INT_MAX;
               }
               ++pos;
               return child;
            }else{  // (o1, j o2)
               array<int, NUMMACHINES> c2;
               c2[flowshop.machines-1] = node.c2[flowshop.machines-1] + flowshop.operations[flowshop.machines-1][job];

               //machine m bound
               if (node.s1.empty()){
                  bounds.push_back(newMsum[flowshop.machines-1] + getMinQ(left, flowshop, flowshop.machines-1));
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
               FSPNode<NUMMACHINES> child;
               child.s1 = node.s1;
               child.s2 = node.s2;
               child.s2.insert(child.s2.begin(), job);
               child.c2 = c2;
               child.c1 = node.c1;  //can rely on parent c1
               child.mSum = newMsum;
               child.left = left;
               child.lb = lb;
               child.depth = depth;
               if(child.left.size() == 0){
                   makeSolution(flowshop, child);
               } else {
                  child.sol.makespan = INT_MAX;
               }
               ++pos;
               return child; 
         } 


   //  cout<<"LB: "<< child.lb<<"\n";
  }
};

unsigned lowerBound(const FSPspace<NUMMACHINES, NUMJOBS> & space, const FSPNode<NUMMACHINES> & n) {
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


FSPspace<NUMMACHINES, NUMJOBS> parseFile(string fileName){
   int* chunks;
   ifstream file;
   file.open(fileName);
   FSPspace<NUMMACHINES, NUMJOBS> flowshop;
   if (file){ 
       if (file.is_open()){
         string line;
         getline(file, line);  // get the first line containing two values: number of jobs and number of machines
            chunks = parseLine(line, 2);
            flowshop.jobs = chunks[0];
            flowshop.machines = chunks[1];
            delete[] chunks;
            cout<< "jobs: " <<flowshop.jobs << " machines:" << flowshop.machines << "\n";
            for (int m = 0; m < flowshop.machines; m++){
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
   FSPspace<NUMMACHINES, NUMJOBS> space = parseFile(inputFile);
   FSPNode<NUMMACHINES> root;
   FSPSolution initial = initilizeUpperBound(&space);
   vector<int> rootSol = {};
   root.sol = initilizeUpperBound(&space);
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
   auto sol = root;
   auto executionStart = high_resolution_clock::now();
   YewPar::Skeletons::API::Params<unsigned> searchParameters;
   searchParameters.initialBound = initial.makespan;

   if (skeletonType == "seq") {
      cout<<"Sequential skeleton\n";

      sol = YewPar::Skeletons::Seq<GenNode,
                                   YewPar::Skeletons::API::Optimisation,
                                   YewPar::Skeletons::API::BoundFunction<upperBound_func>,
                                   YewPar::Skeletons::API::ObjectiveComparison<std::less<unsigned>>>
            ::search(space, root, searchParameters);
    
  } else if (skeletonType == "stacksteal") {
      cout<<"stacksteal skeleton\n";

    searchParameters.stealAll = static_cast<bool>(opts.count("chunked"));
    sol = YewPar::Skeletons::StackStealing<GenNode,
                                           YewPar::Skeletons::API::Optimisation,
                                           YewPar::Skeletons::API::BoundFunction<upperBound_func>,
                                           YewPar::Skeletons::API::ObjectiveComparison<std::less<unsigned>>>
        ::search(space, root, searchParameters);
  } 
   else if (skeletonType == "depthbounded") {
     auto spawnDepth = opts["spawn-depth"].as<unsigned>();
     cout<<"depthbounded skeleton with depth: "<<spawnDepth<<"\n";

    searchParameters.spawnDepth = spawnDepth;
    sol = YewPar::Skeletons::DepthBounded<GenNode,
                                         YewPar::Skeletons::API::Optimisation,
                                         YewPar::Skeletons::API::BoundFunction<upperBound_func>,
                                         YewPar::Skeletons::API::ObjectiveComparison<std::less<unsigned>>>
               ::search(space, root, searchParameters);
} else if (skeletonType == "budget") {
  auto budget = opts["backtrack-budget"].as<unsigned>();
    cout<<"budget skeleton with budget: "<<budget<<"\n";

    searchParameters.backtrackBudget = budget;
    sol = YewPar::Skeletons::Budget<GenNode,
                                    YewPar::Skeletons::API::Optimisation,
                                    YewPar::Skeletons::API::BoundFunction<upperBound_func>,
                                    YewPar::Skeletons::API::ObjectiveComparison<std::less<unsigned>>>
        ::search(space, root, searchParameters);
  }


         auto executionTime = duration_cast<microseconds>(high_resolution_clock::now() - executionStart);
         cout << "Optimal makespan: " << sol.sol.makespan <<"\n" << "Optimal job scheduling order: "; ;
      for(int i =0; i < space.jobs; i++) cout << sol.sol.sequence[i] + 1 << " ";
         cout << "\n";
      cout << "Execution time: " << executionTime.count() << " microseconds" << endl;
      cout << "Nodes decomposed: " << nodesDecomposed << endl;
      return hpx::finalize();
   }


int main(int argc, char* argv[]) {
  boost::program_options::options_description
    desc_commandline("Usage: " HPX_APPLICATION_STRING " [options]");

  desc_commandline.add_options()
      ( "skeleton",
        boost::program_options::value<std::string>()->default_value("seq"),
        "Which skeleton to use: seq, depthbounded, stacksteal, budget, or ordered"
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


