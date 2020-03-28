#include<iostream>
using namespace std;

int main()
{
  int p, r;   // p is the no of process and  r is no of resoures
  cout<<"Enter the no of proess : "<<endl;
  cin >> p;
  cout<<"Enter the no of Resources : "<<endl;
  cin >> r;

  int available[r];  // for no of available resoures
  cout << "Enter the available resources---"<<endl;
  for(int i = 0; i < r; i++)
  {
    cout<<(char)('A'+i) <<endl;
    cin >> available[i];
  }
  cout << "Enter the maximum resoures required for each process"<<endl;
  int max[p][r], allocation[p][r];
  for(int i = 0; i<p; i++)
  {
    for(int j = 0; j<r; j++)
    {
      cout<<"Max need for proess p"<<i+1<<endl;
      cin >> max[i][j];
    }
  }

  cout << "Enter the allocated resources"<<endl;
  //int max[p][r], allocation[p][r];
  for(int i = 0; i<p; i++)
  {
    for(int j  = 0; j<r; j++)
    {
      cout<<"Allocationn for process p"<<i+1<<endl;
      cin >> allocation[i][j];
    }
  }

  int remain_need[p][r];
  for(int i = 0; i<p; i++)
  {
    for(int j = 0; j<r; j++)
    {
      remain_need[i][j] = max[i][j] - allocation[i][j];
    }
  }

  for(int i = 0; i<p; i++)
  {
    for(int j = 0; j<r; j++)
    {
      cout<<remain_need[i][j]<<endl;
    }
  }

  int buffer[p] = {false};

  for(int i = 0; i < p; i++)
  {
    for(int j = 0; j<r; j++)
    {
      if(buffer[i] == false && (remain_need[i][j]) < (allocation[i][j]))
      {
        available[i] = available[i] + allocation[i][j];
        buffer[i] = true;
      }
    }

  }
  int sum;
  for(int i = 0; i<p; i++)
  {
    if(buffer[i] == true)
    sum++;
  }
  if(sum == p)
  {
    cout<<"The process is in safe states"<<endl;
  }



}
