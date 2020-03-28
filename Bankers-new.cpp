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

int main()
{
  int nResoures, nProcesses;
  cout<<"Enter the no of proess : "<<endl;
  cin >> nProcesses;
  cout<<"Enter the no of Total Resources : "<<endl;
  cin >> nResoures;

  int resources[nResoures];               // for no of available resoures
  cout << "Enter the amount of total resources :"<<endl;
  for(int i = 0; i < nResoures; i++)
  {
    cout<<(char)('A'+i) <<endl;
    cin >> resources[i];
  }

  cout << "Enter the maximum resoures required for each process"<<endl;
  int maxRequired[nProcesses][nResoures];
  for(int i = 0; i<nProcesses; i++)
  {
    for(int j = 0; j<nResoures; j++)
    {
      cout<<"Max need for proess p"<<i+1<<endl;
      cin >> maxRequired[i][j];
    }
  }

  int allocated[nProcesses][nResoures];
  cout << "Enter the allocated resources"<<endl;
  for(int i = 0; i<nProcesses; i++)
  {
    for(int j  = 0; j<nResoures; j++)
    {
      cout<<"Allocationn for process p"<<i+1<<endl;
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
