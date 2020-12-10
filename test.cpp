#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <tuple>
#include <vector>

using namespace std;


 int johnsonsRule() {
   // vector <value, jobIndex> seq;
   int operations [2] [5] = {{10, 6, 8, 8, 12},{6, 12, 9, 10, 5}};
   vector<int> jobsLeft = {1, 4, 3, 0, 2};
   vector<tuple<int,int>> seq1;
   vector<tuple<int,int>> seq2;

   for (int jobIndex: jobsLeft){
      if (operations[0][jobIndex] <= operations[1][jobIndex]){
         seq1.push_back(make_tuple(operations[0][jobIndex], jobIndex));
      } else {
         seq2.push_back(make_tuple(operations[1][jobIndex], jobIndex));
      }
   }

   //sort both sequences
   sort(seq1.begin(), seq1.end());
   //reverse sort
   sort(seq2.rbegin(), seq2.rend());

   int completion [2] = {0, 0};
   int length, jobIndex;

   //set initial sequence, length = m1 length
   for (auto tuple : seq1){
      tie(length, jobIndex) = tuple;
      cout <<jobIndex + 1<<" ";
      completion[0]+=length;
      completion[1] = max(completion[0], completion[1]) + operations[1][jobIndex];
   }
   //set final sequence, length = m2 length
   for (auto tuple : seq2){
      tie(length, jobIndex) = tuple;
      cout <<jobIndex + 1<<" ";
      completion[0]+=operations[0][jobIndex];
      completion[1] = max(completion[0], completion[1]) + length;
   }
   cout<<"\n Johnsons number: "<<completion[1]<<"\n";
   return  completion[1];
}

int main(int argc, char* argv[]) {   
   int makespan = johnsonsRule();
}

