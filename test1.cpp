#include<iostream>
#include<vector>
using namespace std;
int main()
{
  int *a =new int [5];
    int *b;

  for (int i = 0; i<5; i++){
      a[i]=i;
  }
  b = a;

    for (int i = 0; i<5; i++){
      cout<<b[i]<<" ";
  }  cout<<"\n";
  delete [] b;
    return 0;
}