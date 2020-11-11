#include<iostream>
#include<vector>
using namespace std;
int main()
{
    //Given Vectors
    vector<int>v1 = {4,3,2,1};
    vector<int>v2 = {-1,0,7};
    //insert function used to insert elements in 
    //range of second and third parameter of v2 
    //into v1 from index equal to first parameter
    // v1.insert(v1.end(),v2.begin(),v2.end());
    std::copy (b.begin(), b.end(), std::back_inserter(a));
    //printing the concatenated vector v1
    for(auto it: v1){
        cout<<it<<" ";
    }
    
    return 0;
}