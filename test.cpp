#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include <vector>

using namespace std;


int main(int argc, char* argv[]) {   
   vector<int> a;
   a.push_back(1);
   a.push_back(7);
   a.push_back(2);
   a.push_back(9);
   sort(a.rbegin(), a.rend());
    for (int i : a){
        cout<<i<<" ";
    }
    cout<<"\n";
}

