#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#ifndef NUMJOBS
#define NUMJOBS 20
#endif

#ifndef NUMMACHINES
#define NUMMACHINES 20
#endif

using namespace std::chrono;
using namespace std;

unsigned long long int nodesDecomposed = 0;

struct JobLength {
  int index;
  int length;
};

template <unsigned N> struct FSPSolution {
  array<int, N> sequence;
  unsigned makespan;
};

FSPSolution<NUMJOBS> candidate = {array<int, NUMJOBS>{}, INT_MAX};

template <unsigned M, unsigned N> struct FSPNode {
  array<int, N> left;
  unsigned leftNum;
  array<int, N> s1;
  unsigned s1Num;
  array<int, N> s2;
  unsigned s2Num;
  int depth;
  array<int, M> c1;
  array<int, M> c2;
  array<int, M> mSum;
};

template <unsigned M, unsigned N> struct FSPspace {
  int machines;
  int jobs;
  array<array<int, N>, M> operations;
  array<array<int, N>, M> jobForwardSum;
  array<array<int, N>, M> jobBackwardSum;
};

bool compareJobLengths(JobLength j1, JobLength j2) {
  return j1.length > j2.length;
}

unsigned calculateSequence(vector<int> &jobs,
                           const FSPspace<NUMMACHINES, NUMJOBS> &flowshop) {
  std::array<unsigned, NUMMACHINES> completionTimes;
  for (int m = 0; m < flowshop.machines; m++) {
    completionTimes[m] = 0;
  }
  for (auto jobIndex : jobs) {
    completionTimes[0] += flowshop.operations[0][jobIndex];
    for (int m = 1; m < flowshop.machines; m++) {
      completionTimes[m] = max(completionTimes[m - 1], completionTimes[m]) +
                           flowshop.operations[m][jobIndex];
    }
  }
  int result = completionTimes[flowshop.machines - 1];
  return result;
}

tuple<vector<int>, int>
findBestSequence(vector<int> scheduled, int unscheduled,
                 FSPspace<NUMMACHINES, NUMJOBS> &flowshop) {
  vector<int> partial;
  vector<int> bestSequence;
  bestSequence.assign(scheduled.begin(), scheduled.end());
  bestSequence.insert(bestSequence.begin(), unscheduled);
  unsigned best = calculateSequence(bestSequence, flowshop);
  for (int i = 1; i <= scheduled.size(); i++) {
    partial.assign(scheduled.begin(), scheduled.end());
    partial.insert(partial.begin() + i, unscheduled);
    int makespan = calculateSequence(partial, flowshop);
    if (makespan < best) {
      best = makespan;
      bestSequence.assign(partial.begin(), partial.end());
    }
  }
  return make_tuple(bestSequence, best);
}

FSPSolution<NUMJOBS>
initilizeUpperBound(FSPspace<NUMMACHINES, NUMJOBS> &flowshop) {

  vector<JobLength> jobLengths;
  vector<int> schedule;
  for (int j = 0; j < flowshop.jobs; j++) {
    JobLength jobLength;
    jobLength.length = 0;
    jobLength.index = j;
    for (int m = 0; m < flowshop.machines; m++) {
      jobLength.length += flowshop.operations[m][j];
    }
    jobLengths.push_back(jobLength);
  }
  sort(jobLengths.begin(), jobLengths.end(), compareJobLengths);
  schedule.push_back(jobLengths.at(0).index);
  unsigned best;
  for (int i = 1; i < flowshop.jobs; i++) {
    int newBest;
    vector<int> seq;
    tie(seq, newBest) =
        findBestSequence(schedule, jobLengths.at(i).index, flowshop);
    schedule.assign(seq.begin(), seq.end());
    best = newBest;
  }
  array<int, NUMJOBS> solution;
  for (int i = 0; i < schedule.size(); i++) {
    solution[i] = schedule[i];
  }
  return FSPSolution<NUMJOBS>{solution, best};
}

int getMinQ(array<int, NUMJOBS> jobsLeft,
            FSPspace<NUMMACHINES, NUMJOBS> &flowshop, int machine,
            int leftNum) {
  int value = INT_MAX;
  for (int j = 0; j < leftNum; j++) {
    value = min(value, flowshop.jobBackwardSum[machine][jobsLeft[j]]);
  }
  return value;
}

int getMinR(array<int, NUMJOBS> jobsLeft,
            FSPspace<NUMMACHINES, NUMJOBS> &flowshop, int machine,
            int leftNum) {
  int value = INT_MAX;
  for (int j = 0; j < leftNum; j++) {
    value = min(value, flowshop.jobForwardSum[machine][jobsLeft[j]]);
  }
  return value;
}

void makeSolution(const FSPspace<NUMMACHINES, NUMJOBS> &space,
                  FSPNode<NUMMACHINES, NUMJOBS> &node) {
  for (int s2Index = space.jobs - node.s2Num; s2Index < space.jobs; s2Index++) {
    int job = node.s2[s2Index];
    node.s1[s2Index] = job;
    node.c1[0] = node.c1[0] + space.operations[0][job];
    for (int m = 1; m < space.machines; m++) {
      node.c1[m] = max(node.c1[m - 1], node.c1[m]) + space.operations[m][job];
    }
  }

  unsigned makaespan = node.c1[space.machines - 1];
  candidate = FSPSolution<NUMJOBS>{node.s1, makaespan};
}

int invalidFormatError(string message) {
  cout << "input file has invalid format: " << message << "\n";
  exit(EXIT_FAILURE);
}

int *convertListStingToInt(list<string> chunks) {
  int size = chunks.size();
  int *nums = new int[size];
  list<string>::iterator it;
  for (int i = 0; i < size; i++) {
    nums[i] = stoi(chunks.front());
    chunks.pop_front();
  }
  return nums;
}

int *parseLine(string line, int expected) {
  istringstream stream(line);
  list<string> chunks;
  copy(std::istream_iterator<std::string>(stream),
       std::istream_iterator<std::string>(), std::back_inserter(chunks));
  if (expected != chunks.size()) {
    cout << "expected: " << expected << " actual: " << chunks.size() << ". ";
    invalidFormatError("invalid number of arguments");
  }
  return convertListStingToInt(chunks);
}

FSPspace<NUMMACHINES, NUMJOBS> parseFile(string fileName) {
  int *chunks;
  ifstream file;
  file.open(fileName);
  FSPspace<NUMMACHINES, NUMJOBS> flowshop;
  if (file) {
    if (file.is_open()) {
      string line;
      getline(file, line); // get the first line containing two values: number
                           // of jobs and number of machines
      chunks = parseLine(line, 2);
      flowshop.jobs = chunks[0];
      flowshop.machines = chunks[1];
      delete[] chunks;
      cout << "jobs: " << flowshop.jobs << " machines:" << flowshop.machines
           << "\n";
      for (int m = 0; m < flowshop.machines; m++) {
        getline(file, line);
        chunks = parseLine(line, flowshop.jobs);
        for (int j = 0; j < flowshop.jobs; j++) {
          flowshop.operations[m][j] = chunks[j];
        }
        delete[] chunks;
      }

      // set jobForwarsSum[0] and jobBackward[m-1] initial 0
      for (int j = 0; j < flowshop.jobs; j++) {
        flowshop.jobForwardSum[0][j] = 0;
        flowshop.jobBackwardSum[flowshop.machines - 1][j] = 0;
      }
      for (int j = 0; j < flowshop.jobs; j++) {
        for (int f = 1; f < flowshop.machines; f++) {
          flowshop.jobForwardSum[f][j] =
              flowshop.jobForwardSum[f - 1][j] + flowshop.operations[f - 1][j];
        }
        for (int b = flowshop.machines - 2; b >= 0; b--) {
          flowshop.jobBackwardSum[b][j] =
              flowshop.jobBackwardSum[b + 1][j] + flowshop.operations[b + 1][j];
        }
      }
      for (int m = 0; m < flowshop.machines; m++) {
        for (int j = 0; j < flowshop.jobs; j++) {
          cout << flowshop.operations[m][j] << " ";
        }
        cout << "\n";
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

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("input file not provided\n");
    exit(EXIT_FAILURE);
  }
  auto inputFile = argv[1];
  FSPspace<NUMMACHINES, NUMJOBS> flowshop = parseFile(inputFile);
  auto executionStart = std::chrono::steady_clock::now();
  FSPNode<NUMMACHINES, NUMJOBS> root;
  // set o1, o2 makespans to 0 and initialize sum(Pkj) to 0
  for (int m = 0; m < flowshop.machines; m++) {
    root.mSum[m] = 0;
    root.c1[m] = 0;
    root.c2[m] = 0;
  }
  // set all sum(Pkj) unsheduled job sum times on all machines
  for (int j = 0; j < flowshop.jobs; j++) {
    root.left[j] = j;
    for (int m = 0; m < flowshop.machines; m++) {
      root.mSum[m] += flowshop.operations[m][j];
    }
  }
  FSPSolution<NUMJOBS> sol = initilizeUpperBound(flowshop);
  root.leftNum = flowshop.jobs;
  root.s1Num = 0;
  root.s2Num = 0;
  root.depth = 0;
  vector<reference_wrapper<FSPNode<NUMMACHINES, NUMJOBS>>> problems;
  problems.push_back(root);
  while (problems.size() > 0) {
    cout<<"problems: "<<problems.size()<<"\n";

    FSPNode<NUMMACHINES, NUMJOBS> node = problems.back().get();
    problems.pop_back();

    for (int pos = 0; pos < node.leftNum; pos++) {
      int job = node.left[pos];
      int depth = node.depth + 1;
      int bound = INT_MIN;
      array<int, NUMMACHINES> newMsum;

      int leftNum = node.leftNum - 1;
      array<int, NUMJOBS> left;
      int newIndex = 0;
      for (int i = 0; i < node.leftNum; i++) {
        if (i != pos) {
          left[newIndex] = node.left[i];
          newIndex++;
        }
      }
      for (int m = 0; m < flowshop.machines; m++) {
        newMsum[m] = node.mSum[m] - flowshop.operations[m][job];
      }
      if (depth % 2 == 1) { // (o1 j, o2)
        array<int, NUMMACHINES> c1;
        c1[0] = node.c1[0] + flowshop.operations[0][job];
        // Machine 1 bound
        if (node.s2Num == 0) {
          auto candidateBound = newMsum[0] + getMinQ(left, flowshop, 0, leftNum);
          bound = max(bound, candidateBound);
          cout<<"bound "<< candidateBound <<" = "<<newMsum[0]<<" + "<<getMinQ(left, flowshop, 0, leftNum)<<"\n";

        } else {
          auto candidateBound =  newMsum[0] + node.c2[0];
          bound = max(bound, candidateBound);
          cout<<"bound "<< candidateBound <<" = "<<newMsum[0]<<" + "<<node.c2[0]<<"\n";
        }
          // bounds for the rest of machines
          for (int m = 1; m < flowshop.machines; m++) {
            // update c1 with the new job times
            c1[m] = max(c1[m - 1], node.c1[m]) + flowshop.operations[m][job];
            // if childs o2 is empty
            if (node.s2Num == 0) {
              auto candidateBound =  c1[m] + newMsum[m] +getMinQ(left, flowshop, m, leftNum);
              bound = max(bound,candidateBound);
              cout<<"bound "<< candidateBound <<" = "<<c1[m]<<" + "<<newMsum[m]<<" + "<<getMinQ(left, flowshop, m, leftNum)<<"\n";

            } else {
              auto candidateBound =  c1[m] + newMsum[m] + node.c2[m];
              bound = max(bound, candidateBound);
              cout<<"bound "<< candidateBound <<" = "<<c1[m]<<" + "<<newMsum[m]<<" + "<<node.c2[m]<<"\n";
            }
          }
          cout<<"Final bound "<<bound<<" pos: "<<pos<<" depth: "<<depth<<"\n";
          if (bound < sol.makespan) {
          FSPNode<NUMMACHINES, NUMJOBS> child;
          child.s2 = node.s2;
          child.s2Num = node.s2Num;
          child.s1 = node.s1;
          child.s1[node.s1Num] = job;
          child.s1Num = node.s1Num + 1;
          child.c1 = c1;
          child.c2 = node.c2;
          child.mSum = newMsum;
          child.left = left;
          child.leftNum = leftNum;
          child.depth = depth;
          if (child.leftNum == 0) {
            makeSolution(flowshop, child);
            cout << "candidate makespan" << candidate.makespan <<"\n";
            if (candidate.makespan < sol.makespan) {
              cout<<"New Sol"<<"\n";
              sol = candidate;
            }
          } else {
            problems.push_back(child);
            cout<<"Child pushed, problems: "<<problems.size()<<"\n";
          }
        }

      } else { // (o1, j o2)
        array<int, NUMMACHINES> c2;
        c2[flowshop.machines - 1] =
            node.c2[flowshop.machines - 1] +
            flowshop.operations[flowshop.machines - 1][job];

        // machine m bound
        if (node.s1Num == 0) {
          auto candidateBound = newMsum[flowshop.machines - 1] + getMinR(left, flowshop, 0, leftNum);
          bound = max(bound, candidateBound);
          cout<<"bound "<< candidateBound <<" = "<<newMsum[flowshop.machines - 1]<<" + "<< getMinR(left, flowshop, 0, leftNum)<<"\n";
        } else {
          auto candidateBound = newMsum[flowshop.machines - 1] + node.c1[flowshop.machines - 1];
          bound = max(bound, candidateBound);
          cout<<"bound "<< candidateBound <<" = "<<newMsum[flowshop.machines - 1]<<" + "<<  node.c1[flowshop.machines - 1]<<"\n";

        }
        // machines m-1 -> 1 bounds
        for (int m = flowshop.machines - 2; m >= 0; m--) {
          // update c2 with the new job times
          c2[m] = max(c2[m + 1], node.c2[m]) + flowshop.operations[m][job];
          // if childs o1 is empty
          if (node.s1Num == 0) {
          auto candidateBound = c2[m] + newMsum[m] +getMinR(left, flowshop, m, leftNum);
            bound = max(bound, candidateBound);
          cout<<"bound "<< candidateBound <<" = "<<c2[m]<<" + "<<newMsum[m]<<" + "<< getMinR(left, flowshop, m, leftNum)<<"\n";

                                   
          } else {
            auto candidateBound = c2[m] + newMsum[m] + node.c1[m];
            bound = max(bound, candidateBound);
            cout<<"bound "<< candidateBound <<" = "<<c2[m]<<" + "<<newMsum[m]<<" + "<< node.c1[m]<<"\n";

          }
        }
        cout<<"Final bound "<<bound<<" pos: "<<pos<<" depth: "<<depth<<"\n";
        if (bound < sol.makespan) {
          FSPNode<NUMMACHINES, NUMJOBS> child;
          child.s1 = node.s1;
          child.s1Num = node.s1Num;
          child.s2 = node.s2;
          child.s2[(flowshop.jobs - 1) - node.s2Num] = job;
          child.s2Num = node.s2Num + 1;
          child.c2 = c2;
          child.c1 = node.c1; // can rely on parent c1
          child.mSum = newMsum;
          child.left = left;
          child.leftNum = leftNum;
          child.depth = depth;
          if (child.leftNum == 0) {
            makeSolution(flowshop, child);
            cout << "candidate makespan" << candidate.makespan <<"\n";
            if (candidate.makespan < sol.makespan) {
              cout<<"New Sol"<<"\n";
              sol = candidate;
            }
          } else {
            problems.push_back(child);
            cout<<"Child pushed, problems: "<<problems.size()<<"\n";
          }
        }
      }
    }
  }

    auto executionTime = duration_cast<microseconds>(
        std::chrono::steady_clock::now() - executionStart);
    cout << "Optimal makespan: " << sol.makespan << "\n"
         << "Optimal job scheduling order: ";
    for (int i = 0; i < flowshop.jobs; i++)
      cout << sol.sequence[i] + 1 << " ";
    cout << "\n";
    cout << "Execution time: " << executionTime.count() << " microseconds"
         << endl;
    cout << "Nodes decomposed: " << nodesDecomposed << endl;
    return 0;
  
}