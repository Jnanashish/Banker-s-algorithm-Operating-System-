#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

pthread_mutex_t lockResources;
pthread_cond_t condition;


bool getSafeSeq();
void* processCode(void* );

int *safeSeq;
int nProcessRan = 0;
int nResources, nProcesses;
int *resources;
int **allocated;
int **maxRequired;
int **need;
void sampletest();
int main()
{
  cout<<"Press 1 for smaple test case"<<endl;
  cout<<"Press 2 for mulithreading test"<<endl;
  int num;
  cin >> num;
  if(num == 1)
  {
    int n = 5, m = 3, i, j, k;
    int alloc[5][3] = { { 0, 1, 0 }, 
                        { 2, 0, 0 }, 
                        { 3, 0, 2 }, 
                        { 2, 1, 1 }, 
                        { 0, 0, 2 } }; 

    int max[5][3] = { { 7, 5, 3 }, 
                      { 3, 2, 2 }, 
                      { 9, 0, 2 }, 
                      { 2, 2, 2 }, 
                      { 4, 3, 3 } }; 

    int avail[3] = { 3, 3, 2 }; 

    int f[n], ans[n], ind = 0;
    for (k = 0; k < n; k++) {
        f[k] = 0;
    }
    int need[n][m];
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    }
    int y = 0;
    for (k = 0; k < 5; k++) {
        for (i = 0; i < n; i++) {
            if (f[i] == 0) {

                int flag = 0;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]){
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0) {
                    ans[ind++] = i;
                    for (y = 0; y < m; y++)
                        avail[y] += alloc[i][y];
                    f[i] = 1;
                }
            }
        }
    }
    cout<<"Matrix for The resources and allocation"<<endl;
    cout<<endl;
    cout<<"  Process  |  Allocation  |  Max need  |  Available  |  Remaining need  "<<endl;
    cout<<"  A  B  C  |    A  B  C   |  A  B  C   |   A  B  C   |      A  B  C     "<<endl;
    cout<<"     p1    |    0  1  0   |  7  5  3   |   3  3  2   |      7  4  3     "<<endl;
    cout<<"     p2    |    2  0  0   |  3  2  2   |   5  3  2   |      1  2  2     "<<endl;
    cout<<"     p3    |    3  0  2   |  9  0  2   |   7  4  3   |      6  0  0     "<<endl;
    cout<<"     p4    |    2  1  1   |  4  2  2   |   7  4  5   |      2  1  1     "<<endl;
    cout<<"     p5    |    0  0  2   |  5  3  3   |   10 5  7   |      5  3  1     "<<endl;
    cout<<endl;
    
    cout << "Following is the SAFE Sequence" << endl;
    for (i = 0; i < n - 1; i++)
        cout << " P" << ans[i] << " ->";
    cout << " P" << ans[n - 1] <<endl;
    
  }
  else if(num == 2)
{
  int nResoures, nProcesses;
  cout<<"Enter the no of proess : "<<endl;
  cin >> nProcesses;
  cout<<"Enter the no of Total Resources : "<<endl;
  cin >> nResoures;

  int resources[nResoures];               // for no of available resoures
  cout << "Enter the amount of total resources A  B  C:"<<endl;
  for(int i = 0; i < nResoures; i++)
  {
    cout<<(char)('A'+i) <<endl;
    cin >> resources[i];
  }

  cout << "Enter the maximum resoures required for each process"<<endl;
  int maxRequired[nProcesses][nResoures];
  for(int i = 0; i<nProcesses; i++)
  {
  	cout<<"Max need for proess p"<<i+1<< "  A  B  c"<<endl;
    for(int j = 0; j<nResoures; j++)
    {  
      cin >> maxRequired[i][j];
    }
  }

  int allocated[nProcesses][nResoures];
  cout << "Enter the allocated resources"<<endl;
  for(int i = 0; i<nProcesses; i++)
  {
  	cout<<"Allocationn for process p"<<i+1<< "  A  B  c"<<endl;
    for(int j  = 0; j<nResoures; j++)
    {
      cin >> allocated[i][j];
    }
  }

  int need[nProcesses][nResoures];
  for(int i = 0; i<nProcesses; i++)
  {
    for(int j = 0; j<nResoures; j++)
    {
      need[i][j] = maxRequired[i][j] - allocated[i][j];
    }
  } 
  safeSeq = (int *)malloc(nProcesses * sizeof(*safeSeq));
        for(int i=0; i<nProcesses; i++) safeSeq[i] = -1;

        if(!getSafeSeq()) {
                printf("\nUnsafe State! The processes leads the system to a unsafe state.\n\n");
                exit(-1);
        }

        printf("\n\nSafe Sequence Found : ");
        for(int i=0; i<nProcesses; i++) {
                printf("%-3d", safeSeq[i]+1);
        }

        printf("\nExecuting Processes...\n\n");
        sleep(1);

        pthread_t processes[nProcesses];
        pthread_attr_t attr;
        pthread_attr_init(&attr);

	int processNumber[nProcesses];
	for(int i=0; i<nProcesses; i++)
	    processNumber[i] = i;

        for(int i=0; i<nProcesses; i++)
        pthread_create(&processes[i], &attr, processCode, (void *)(&processNumber[i]));

        for(int i=0; i<nProcesses; i++)
                pthread_join(processes[i], NULL);

        printf("\nAll Processes Finished\n");

        free(resources);
        for(int i=0; i<nProcesses; i++) {
                free(allocated[i]);
                free(maxRequired[i]);
		            free(need[i]);
        }
        free(allocated);
        free(maxRequired);
      	free(need);
        free(safeSeq);
}
}

bool getSafeSeq()
   {
        int tempRes[nResources];
        for(int i=0; i<nResources; i++) tempRes[i] = resources[i];

        bool finished[nProcesses];
        for(int i=0; i<nProcesses; i++) finished[i] = false;
        int nfinished=0;
        while(nfinished < nProcesses) {
                bool safe = false;

                for(int i=0; i<nProcesses; i++) {
                        if(!finished[i]) {
                                bool possible = true;

                                for(int j=0; j<nResources; j++)
                                        if(need[i][j] > tempRes[j]) {
                                                possible = false;
                                                break;
                                        }

                                if(possible) {
                                        for(int j=0; j<nResources; j++)
                                                tempRes[j] += allocated[i][j];
                                        safeSeq[nfinished] = i;
                                        finished[i] = true;
                                        ++nfinished;
                                        safe = true;
                                }
                        }
                }

                if(!safe) {
                        for(int k=0; k<nProcesses; k++) safeSeq[k] = -1;
                        return false;
        }
        return true;
    }
}

void* processCode(void *arg)
    {
        int p = *((int *) arg);

        pthread_mutex_lock(&lockResources);


        while(p != safeSeq[nProcessRan])
                pthread_cond_wait(&condition, &lockResources);


        printf("\n--> Process %d", p+1);
        printf("\n\tAllocated : ");
        for(int i=0; i<nResources; i++)
                printf("%3d", allocated[p][i]);

        printf("\n\tNeeded    : ");
        for(int i=0; i<nResources; i++)
                printf("%3d", need[p][i]);

        printf("\n\tAvailable : ");
        for(int i=0; i<nResources; i++)
                printf("%3d", resources[i]);

        printf("\n"); sleep(1);

        printf("\tResource Allocated!");
        printf("\n"); sleep(1);
        printf("\tProcess Code Running...");
        printf("\n"); sleep(rand()%3 + 2);
        printf("\tProcess Code Completed...");
        printf("\n"); sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n"); sleep(1);
        printf("\tResource Released!");

	      for(int i=0; i<nResources; i++)
                 resources[i] += allocated[p][i];

        printf("\n\tNow Available : ");
        for(int i=0; i<nResources; i++)
                printf("%3d", resources[i]);
        printf("\n\n");

        sleep(1);
        nProcessRan++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lockResources);
	    pthread_exit(NULL);
 }
