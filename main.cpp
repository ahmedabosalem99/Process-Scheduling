#include <D:\Documents\C++\asd\stdc++.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#include <time.h>
#include <math.h>
// #include<bits/stdc++.h>
using namespace std;
#define ll long long
#define nl "\n"
#define FIO ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

//___________prototypes_______________________________________________________________________________________________________________________
void MLFQ();
void RR( );
void STCF_algorithm();
void SJF();
void FCFS();

int n , schedule,iot,nprocess;
//_____________________________________________________________________________________________________________________________________________________________________________

//_________________RR global__________________________________________________________________________________________________________________________________________________________

# define N 7
# define L 1
typedef struct processRR{
	int id;
	int io;
	int Rio;  //remaining of io instruction
	int arrivaltime;
	int ninstruction;
	int Rinstr;  //remaining of instruction
	int finishtime;
	int fresponsetime;
	int responsetime;
	int tat;
	int timeslice;
	struct processRR *ready;
	struct processRR *blocked;
}processRR;
processRR Queue[L][N];
static char TimeSlice = 5;

processRR *Ready = NULL;
processRR *Blocked= NULL;


void enqueueready(processRR *nnew)  //insert at first
{
	if(Ready==NULL) Ready = nnew;
	else{
		nnew->ready = Ready;
		Ready = nnew;
	}
}

void ready1th(processRR *nnew){
	processRR *current = Ready;
	if(current==NULL) Ready = nnew;
	else if(current->ready==NULL){
		current->ready = nnew;
	}else{
		while(current->ready!=NULL) current = current->ready;
		current->ready = nnew;
	}
	nnew->ready = NULL;
}

void blocked1th(processRR *nnew){
	processRR *current = Blocked;
	if(current==NULL) Blocked = nnew;
	else if(current->blocked==NULL) {current->blocked = nnew;}
	else{
		while(current->blocked!=NULL) current = current->blocked;
		current->blocked = nnew;
	}
	nnew->blocked = NULL;
}

void ready2th(processRR *nnew) //insert at the second serive
{
	processRR *current = Ready;
	if(current==NULL) Ready = nnew;
	else if(current->ready==NULL){
		nnew->ready = Ready;
		Ready = nnew;
	}else{
		while(current->ready->ready!=NULL) current = current->ready;
		nnew->ready = current->ready;
		current->ready = nnew;
	}
}

void enqueueblocked(processRR *nnew)  //insert at first
{
	if(Blocked==NULL) Blocked = nnew;
	else{
		nnew->blocked = Blocked;
		Blocked = nnew;
	}
}

processRR *dequeueready()  //delete at last
{
	processRR *current = Ready;
	processRR *re = NULL;
	if(current==NULL) re = NULL;
	else if(current->ready==NULL){re = Ready; Ready = NULL;}
	else{
		while(current->ready->ready!=NULL) current = current->ready;
		re = current->ready;
		current->ready = NULL;
	}
	return re;	
}

processRR *dequeueblocked()  //delete at last
{
	processRR *current = Blocked;
	processRR *re = NULL;
	if(current==NULL) re = NULL;
	else if(current->blocked==NULL){re = Blocked; Blocked = NULL;}
	else{
		while(current->blocked->blocked!=NULL) current = current->blocked;
		re = current->blocked;
		current->blocked = NULL;
	}
	return re;	
}

void arrivalsort(){
    processRR temp;
    int i,j;
    for(int h=0;h<L;h++){
        for(i=0;i<N;i++){
            for(j=i+1;j<N;j++){
                if(Queue[h][i].arrivaltime>Queue[h][j].arrivaltime){
                        temp = Queue[h][i];
                        Queue[h][i] = Queue[h][j];
                        Queue[h][j] = temp;
                }
            }
        }
    }
}

void displayprocess(){
	for(int i=0  ; i<L ; ++i){
        for(int j=0 ; j<N ; ++j){
                if(Queue[i][j].id>0)printf("%d %d %d %d\n",Queue[i][j].id,Queue[i][j].ninstruction,Queue[i][j].io,Queue[i][j].arrivaltime);
        }
        puts("");
    }
}	

void get_input_RR(){

	int i,j;
		FILE *fp;
            fp = fopen("final_not_priority.csv", "r");
            if(fp == NULL){
                perror("ERROR: No file to be read");
                exit(1);
        }
        fscanf(fp,"%d, ",&nprocess);
        fscanf(fp,"%d, ",&iot);
		for(i = 0; i < N; i++){
                for(j = 0; j < L; j++){
                    fscanf(fp, "%d, ", &(Queue[j][i].id));
                    fscanf(fp, "%d, ", &(Queue[j][i].ninstruction));
                    fscanf(fp, "%d, ", &(Queue[j][i].io));
                    fscanf(fp, "%d,\n", &(Queue[j][i].arrivaltime));
                    Queue[j][i].io = (Queue[j][i].io * Queue[j][i].ninstruction)/100;
                    Queue[j][i].Rio =iot;
                    Queue[j][i].Rinstr = Queue[j][i].ninstruction;
                    Queue[j][i].ready = NULL;
                    Queue[j][i].blocked = NULL;
                    Queue[j][i].finishtime =0;
                    Queue[j][i].fresponsetime =0;
                    Queue[j][i].timeslice = 0;
                    Queue[j][i].fresponsetime=-1;	
                }
            }
		fclose(fp);
}





//__________________________________________________________MLFQ____________________________________________________________________________

typedef struct process
{
    int at;     //arrival time
    int bt;     
    int ic;     // ic
    int remic;  //remaning insturction  
    int iop;    // io precentage
    int ioremic;
    int tat;    // 
    int rt=-1; // response time
    int ct; //comeletion time
    int level=-1; //which queue
    bool flag=0; 
    int tc=0; //time consuming
    int rrt=0;
    bool BoostOccured=0;
}process;
void  get_input_MLFQ(map<int,process>&mp)
{
    int i, j;
    FILE *fp;
    fp = fopen("final_not_priority.csv", "r");
    fscanf(fp,"%d, ",&n);
    fscanf(fp,"%d, ",&iot);
    if(fp == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else{
        int id;
        for(i = 0; i < n; i++){
                fscanf(fp, "%d, ",&id) ;
                fscanf(fp, "%d, ", &mp[id].ic);
                fscanf(fp, "%d, ", &mp[id].iop);
                fscanf(fp, "%d,\n", &mp[id].at);
        }
    }
    fclose(fp);
    return ;
}
void write_to_file_randomlyy (int procn){
    int iid,iop,art,ic,io;
    
    FILE *fp;
    fp = fopen("final_not_priority.csv", "w");
    for(int i=1 ;i<=procn ; ++i){
        iid=i;
        iop=(rand() % 15 );
        art=rand() % 5;
        ic=(rand() % 50)+1;
        if(fp == NULL)
        {
            perror("ERROR: ERROR: ERROR:");
            exit(1);
        }
        else{
            fprintf(fp,"%d, ",iid);
            fprintf(fp,"%d, ",ic);
            fprintf(fp,"%d, ",iop);
            fprintf(fp,"%d\n",art);

        }
    }
    fclose(fp);
}

//__________________________________________________________STCF____________________________________________________________________________

void write_to_file_randomly (){

    int no_of_proc, iid,art,iot, io_perc, ins_count;
    FILE *fp;
    fp = fopen("final_not_priority.csv", "w");
    no_of_proc = 4 + (rand() % 5); // from 4 to 8 process
    iot = 2 + (rand() % 10);        // from 2 to 11 unit time
    if(fp == NULL)
    {
        perror("ERROR: ERROR: ERROR:");
        exit(1);
    }

    fprintf(fp,"%d, ",no_of_proc);
    fprintf(fp,"%d\n",iot);
    for(int i=1 ;i<=no_of_proc; ++i){
        iid=i;
        art=rand() % 5;
        io_perc = 10 + (rand() % 11); //from 10% to 20%
        ins_count = 5 + (rand() % 6); // from 5 to 10 instructions

        fprintf(fp,"%d, ",iid);
        fprintf(fp,"%d, ",ins_count);
        fprintf(fp,"%d, ",io_perc);
        fprintf(fp,"%d\n",art);

    }
    fclose(fp);
}
//__________________________________________________________SJF____________________________________________________________________________
void write_randomly (int procn);
void get_info(int num);

struct processSJF
{
    int at,ct, wt, tat, p,inst,bt;
}processSJF[10];

struct processSJF temp;
void write_randomly (int procn){
    int iid,art,NI,io;
    FILE *fp;
    fp = fopen("final_not_priority.csv", "w");
    for(int i=1 ;i<=procn ; ++i){

        iid=i;
        art=(rand() % 5 )+1;
        NI=(rand () %10)+1;
        if(fp == NULL)
        {
            perror("ERROR: ERROR: ERROR:");
            exit(1);
        }
        else{
            fprintf(fp,"%d, ",iid);
            fprintf(fp,"%d,",art);
            fprintf(fp,"%d,",NI);
        }
    }
    fclose(fp);
}
void get_info(int num)
{
    int i, j;
    FILE *fp;
    fp = fopen("final_not_priority.csv", "r");

    if(fp == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else{
        for(i = 0; i <num; i++){
                fscanf(fp, "%d, ", &(processSJF[i].p));
                fscanf(fp, "%d, ", &(processSJF[i].at));
                fscanf(fp, "%d, ", &(processSJF[i].inst));   
        }
    }
    fclose(fp);
}
//______________________FCFS_____________________________________________________________________________________________________________

int no_of_process;
int io_time;

struct Process
{
    int pid ,
    AT ,
    inst,
    io_percentage = -1,
    remain_io_inst,
    st,
    ct;
};
 // Get Data of Processes from File
void get_input_FCFS(struct Process P[]){
 
    int i, j;
    FILE *fp;
    fp = fopen("final_not_priority.csv", "r");
 
    if(fp == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else{
 
        fscanf(fp, "%d, ", &no_of_process);
        fscanf(fp, "%d\n", &io_time);
 
        for(i = 0; i < no_of_process; i++){
            fscanf(fp, "%d, ", &(P[i].pid));
            fscanf(fp, "%d, ", &(P[i].inst));
            fscanf(fp, "%d, ",&(P[i].io_percentage));
            fscanf(fp, "%d\b", &(P[i].AT));
        }
    }
    fclose(fp);
}

// Sort the Processes with respect to their Arrival Time
void sort(struct Process P[]){
    for (int i = 0 ; i<no_of_process-1;++i){
        for (int j = i+1 ; j < no_of_process;++j){
            if (P[i].AT > P[j].AT){
                swap(P[i], P[j]);
                swap(P[i].pid, P[j].pid);
            }
        }
    }
}

//______________________________________________________________________________________________________________________________________

char separator=' ';
template<typename T> void printElement(T t, const int& width)
{
    cout << left << setw(width) << setfill(separator) << t;
}



//________________________________________________________________________________________________________________________________________
int main(){
    FIO
    printf("\nScheduling Algorithms\n");
    printf("     1 -> Multi-Level Feedback Queue \n     2 -> First In First Out\n     3 -> Shortest Job First\n     4 -> Shortest Time-to-Completion First\n     5 -> Round Robin\n");
    printf("     Enter Your Choice: ");
    cin>>schedule;

    puts("");
    srand(time(NULL));
    switch (schedule)
    {
    case 1:
        MLFQ();
        break;
    case 2:
        FCFS();
        break;
    case 3:
        // printf("     Enter the number of process: ");
        // cin>>n;
        SJF();
        break;
    case 4:
        STCF_algorithm();
        break;
    case 5:
        // printf("     Enter the number of process: ");
        // cin>>n;
        // printf("     Enter I/O instruction time in respect to CPU instruction: ");
        // cin>>iot;
        RR();
        break;
    }

    return 0;
}
//________________________________________________________________________________________________________________________________________



void MLFQ(){
        int terminated=0;
        int tQue1=5,tQue2=10,tQue3=20, BoostTime=40,nmwidth=15, numwidth=17;
        int counter=0;
        deque<int>ready1,ready2,ready3,blocked;
        map<int,process>mp;
        map<int,vector<int>>mmp;
        write_to_file_randomly();

        get_input_MLFQ(mp);
        printf("number of process: %d | I/O Tiem :%d\n",n,iot);
    //________________________________________________________________________________________________________________________________________
        printElement("PID",nmwidth);printElement("Ins. count",nmwidth);printElement("I/O Prec.",nmwidth);printElement("Arrival",nmwidth);cout<<nl;
        for(auto el: mp){
            printElement(el.first,numwidth);printElement(el.second.ic,numwidth);printElement(el.second.iop,numwidth);printElement(el.second.at,numwidth);cout<<nl;
            mmp[el.second.at].push_back(el.first);
        }
        cout<<nl;
    //________________________________________________________________________________________________________________________________________
        while(terminated < n){                                       
            int key=0;
            for(auto el : mmp[counter]){            //add all process that come in this moment to ready1
                ready1.push_back(el);      
                mp[el].level=1;
                mp[el].remic=mp[el].ic;
            }
    //________________________________________________________________________________________________________________________________________
            if(!ready1.empty()){                              // see if there is any proc in que1
                key=ready1.front();
                if( mp[key].rt == -1 )mp[key].rt = counter-mp[key].at;          // calculate resopnse time

                if( rand() % (mp[key].ic) >= (mp[key].iop  * mp[key].ic) / 100 ){       // if ins. is not i/o ins.
                    mp[key].remic--;                           // reduce remaning ins.
                    mp[key].tc++;
                    if(mp[key].remic<=0){                                     // if process end 
                        ready1.pop_front();
                        mp[key].ct=counter;
                        mp[key].tat= counter - mp[key].at +1;
                        mp[key].flag=1;
                        terminated++;
                        cout<<"P"<<key<<"->finish"<<nl;
                    }

                    else if(mp[key].tc >= tQue1){                         // if time consuming for proc in greater than time slice for que1
                        mp[key].level=2;
                        ready1.pop_front();
                        ready2.push_back(key);
                        cout<<"P"<<key<<"->To Que2"<<nl;
                    }

                    else {                                               // IF NOT   
                        cout<<"P"<<key<<"  ";
                    }
                    
                }
                else {                                          // if it is I/O ins.
                    ready1.pop_front();
                    blocked.push_back(key);
                    mp[key].ioremic=iot;
                    cout<<"P"<<key<<"->IO"<<nl;
                }   

            }
    //________________________________________________________________________________________________________________________________________
            else if(!ready2.empty()){                            // if there is no process in que1 see if there is any proc in que2
                key=ready2.front();
                if( rand() % (mp[key].ic) >= (mp[key].iop  * mp[key].ic) / 100 ){       // if ins. is not i/o ins.
                    mp[key].remic--;                           // reduce remaning ins.
                    mp[key].tc++;
                    if(mp[key].remic<=0){                                       // if process end 
                        ready2.pop_front();
                        mp[key].ct=counter;
                        mp[key].tat= counter - mp[key].at+1 ;
                        mp[key].flag=1;
                        terminated++;
                        cout<<"P"<<key<<"->finish"<<nl;
                    }
                    else if(mp[key].tc >= tQue1+tQue2){                         // if time consuming for proc in greater than time slice for que1 + que2
                        mp[key].level=3;
                        mp[key].rrt=0;
                        ready2.pop_front();
                        ready3.push_back(key);
                        
                        cout<<"P"<<key<<"->To Que3"<<nl;
                    }
                    else {                                                      // if not    
                        cout<<"P"<<key<<"  ";
                    }
                }
                else {                                          // if it is i/o ins.
                    ready2.pop_front();
                    blocked.push_back(key);
                    mp[key].ioremic=iot;
                    cout<<"P"<<key<<"->IO"<<nl;
                } 
            }
    //________________________________________________________________________________________________________________________________________
            else if(!ready3.empty()){                        // if there is no process in que1 and que2 see if there is any proc in que3
                key=ready3.front();
                if( rand() % (mp[key].ic) >= (mp[key].iop  * mp[key].ic) / 100  ){       // if ins. is not i/o ins.
                    mp[key].remic--;                              // reduce remaning ins.
                    mp[key].tc++;
                    mp[key].rrt++;
                    if(mp[key].remic<=0){                           // if process end 
                        ready3.pop_front();
                        mp[key].ct=counter;
                        mp[key].tat= counter - mp[key].at+1 ;
                        mp[key].flag=1;
                        terminated++;
                        cout<<"P"<<key<<"->finish"<<nl;
                    }
                    else{                                           // if not
                        cout<<"P"<<key<<" ";
                        if((mp[key].rrt ) % tQue3 == 0){                // if the time slice of que3 end go to the next process
                            int temp=ready3.front();
                            ready3.pop_front();
                            ready3.push_back(temp);
                            cout<<nl;
                        }
                    }                
                }
                else {                                          // if it is i/o ins.
                    ready3.pop_front();
                    blocked.push_back(key);
                    mp[key].ioremic=iot;
                    cout<<"P"<<key<<"->IO"<<nl;
                } 
            }
            else{                                               // if ther is no process in any que print NO
                cout<<"NO"<<"  ";
            }
    //________________________________________________________________________________________________________________________________________
            if(!blocked.empty()){
                int key=blocked.front();
                mp[key].ioremic--;
                mp[key].tc++;
                if(mp[key].ioremic<=0){                     // if it finish the I/O instrunction
                    blocked.pop_front();
                    mp[key].remic--;
                    if(mp[key].remic<=0){                   // check if it is the last instruction in the process
                        mp[key].ct=counter;
                        mp[key].tat=counter-mp[key].at+1;
                        mp[key].flag=1;
                        terminated++;
                        cout<<nl<<"P"<<key<<"->finish"<<nl;
                    }
                    else{                                  // if not we need to back it to the correct que
                        if(mp[key].tc > tQue1 + tQue2){             // if the total time consumption is greater than the time slice of que1+que2 then we push it in que3
                            if(mp[key].level==3){                   // if it was in que 3 before I/O insturction 
                                if(!ready3.empty()){                // if que3 is not empty push it in the second place 
                                    auto it=ready3.begin();
                                    it++;
                                    ready3.insert(it,key);
                                }
                                else{                               // else push it in the first space              
                                    ready3.push_back(key);
                                }
                            }
                            else {                                  //else if it wansn't in que3 before I/O instruction push it back to the end of que3
                                cout<<nl<<"P"<<key<<" fin IO To Que3"<<nl;
                                ready3.push_back(key);
                                mp[key].level=3;
                            }
                            mp[key].rrt=0;                          // make the round robin time to 0 so that it will take complete time slice in que3
                        } 
                        else if(mp[key].tc>tQue1) {                 //if the total time consumption is greater than the time slice of que1 then we push it in que2
                            if(mp[key].level==2){
                                if(!ready2.empty()){
                                    auto it=ready2.begin();
                                    it++;
                                    ready2.insert(it,key);
                                }
                                else
                                    ready2.push_back(key);
                                
                            }
                            else {
                                ready2.push_back(key);
                                cout<<nl<<"P"<<key<<" fin IO To Que2"<<nl;
                                mp[key].level=2;
                            }
                        }
                        else {                                      // else we push it in ready 1
                            if(mp[key].level==1){
                                if(!ready1.empty()){
                                    auto it=ready1.begin();
                                    it++;
                                    ready1.insert(it,key);
                                }
                                else ready1.push_back(key);
                            }
                            else{     // if the process was in que2 or que3  but after BoostTime it finish I/O instruction the the time it consume is less than tQue1 then we push it to que1 but in the back
                                ready1.push_back(key);                                  
                                cout<<nl<<"P"<<key<<" fin IO To Que1"<<nl;
                                mp[key].level=1;
                            }
                        }
                    }
                }
            }
    //________________________________________________________________________________________________________________________________________
            if((counter+1) % BoostTime == 0){                               
                cout<<nl<<"BOOST Time"<<nl;
                for(auto el : ready1){
                    mp[el].tc=0;
                }
                while(!ready2.empty()){
                    int temp=ready2.front();
                    ready2.pop_front();
                    ready1.push_back(temp);
                    mp[temp].tc=0;
                    mp[temp].level=1;
                }
                while(!ready3.empty()){
                    int temp=ready3.front();
                    ready3.pop_front();
                    ready1.push_back(temp);
                    mp[temp].tc=0;
                    mp[temp].level=1;
                }
                for(auto el : blocked){
                    mp[el].tc=0;
                }
            }
            counter++;
        }
    //________________________________________________________________________________________________________________________________________
        cout<<nl;
        printElement("PID",nmwidth);printElement("Arrival",nmwidth);printElement("Response",nmwidth);printElement("Completion",nmwidth);printElement("Turnaround",nmwidth);
        cout<<nl<<nl;
        for(auto el :mp){
            printElement(el.first,numwidth);printElement(el.second.at,numwidth);printElement(el.second.rt,numwidth);printElement(el.second.ct,numwidth);printElement(el.second.tat,numwidth);
            cout<<nl<<nl;
        }
}
//________________________________________________________________________________________________________________________________________

void RR(){
    srand(time(NULL));

	write_to_file_randomly();
    get_input_RR();
    arrivalsort();
		displayprocess();
		
		
		processRR *runningprocess=NULL,*blockedprocess = NULL;
		int n = 0,random;
		//enqueue for each ready process
		for(int k=0;k<N;k++){
			if(Queue[0][k].id!=0){
				enqueueready(&Queue[0][k]);
				//printf("%d ",Queue[0][k].id);
			}
		} 
		printf("\n");
		int counter=0;
		while(n!=nprocess){
			
			runningprocess = dequeueready();
			
			if(runningprocess!=NULL){
				if(runningprocess->fresponsetime==-1) runningprocess->fresponsetime= counter;
				//create random instruction
				random = rand() % runningprocess->ninstruction;
				
				if(random < runningprocess->io || runningprocess->io==runningprocess->Rinstr)  //io instruction
				{
					printf("IO%d ",runningprocess->id);
					//dequeue from ready queue
					//enqueue in blocked queue
					runningprocess->timeslice=0;
					enqueueblocked(runningprocess);
					//switch to another ready process
				
				}else   //cpu instruction
				{	
					printf("%d ",runningprocess->id);
					runningprocess->timeslice++;
					runningprocess->Rinstr--;   //dec remaining of instruction
					if(runningprocess->Rinstr==0)  //process finshied
					{
						runningprocess->finishtime = counter+1;
						n++; //inc no of finished process
						//dequeue from ready queue
					}
					else if(runningprocess->timeslice==TimeSlice){
						enqueueready(runningprocess);
						runningprocess->timeslice=0;
					}else{
						ready1th(runningprocess);
					}
					
				}
			}
			
			blockedprocess = dequeueblocked();
			//if there is blocked process in blocked queue
			if(blockedprocess!=NULL){
				blockedprocess->Rio--;
				if(blockedprocess->Rio==0){  //check if reach 0 finshed 
					blockedprocess->Rio = iot;
					blockedprocess->Rinstr--;
					blockedprocess->io--;
					if(blockedprocess->Rinstr==0){ //            check if Rinstr reach 0 === not insert it in ready queue
						runningprocess->finishtime = counter+1;
						n++;
					}else{
						ready2th(blockedprocess);   //                   if not          enqueue in ready queue as 2th
					}
				}else{
					blocked1th(blockedprocess);
				}
			}
			if(Ready==NULL && Blocked==NULL) break;
			counter++;
		}
		printf("\n\n");
		printf("id  -  first response  - arrival time  - finished time - response time  - turnaround time\n");
		for(int i=0  ; i<L ; ++i){
                for(int j=0 ; j<N ; ++j){
                if(Queue[i][j].id ==0 ) continue;
                Queue[i][j].tat = Queue[i][j].finishtime - Queue[i][j].arrivaltime;
                Queue[i][j].responsetime = Queue[i][j].fresponsetime - Queue[i][j].arrivaltime;
                    printf("[%d]      %d                  %d                 %d               %d            %d\n",Queue[i][j].id,Queue[i][j].fresponsetime,Queue[i][j].arrivaltime,Queue[i][j].finishtime,Queue[i][j].responsetime,Queue[i][j].tat);
            }
            puts("");
        }
        
	// }
	return ;	
}

//________________________________________________________________________________________________________________________________________

void STCF_algorithm(){
    struct process{
        int pid = 0;
        int arrival_time = 0;
        int start_time = 0;
        int instruction_count = 0;
        int io_percentage = 0;
        int remin_time_io = 0 ;
        int completion_time = 0;
        int turnaround_time = 0;
        int response_time = 0;
    } p[100];
    int no_of_process;
    int io_time;
    float avg_turnaround_time;
    float avg_response_time;
    int total_turnaround_time = 0;
    int total_response_time = 0;
    int burst_remaining[100];
    int is_completed[100] = {0};
    write_to_file_randomly();
    queue < struct process > io_waiting;

    FILE *fp;
    fp = fopen("final_not_priority.csv", "r");

    if(fp == NULL)
    {
        perror("ERROR: No file to be read");
        exit(1);
    }
    else {
        fscanf(fp, "%d, ", &no_of_process);
        fscanf(fp, "%d\n", &io_time);
        for (int i = 0; i < no_of_process; i++) {
            fscanf(fp, "%d, ", &p[i].pid);
            fscanf(fp, "%d, ", &p[i].instruction_count);
            fscanf(fp, "%d, ", &p[i].io_percentage);
            fscanf(fp, "%d\n ", &p[i].arrival_time);
            burst_remaining[i] = p[i].instruction_count;
        }
    }
    fclose(fp);
    int current_time = 0;
    int completed = 0;

    while(completed != no_of_process) {
        int idx = -1;
        int mn = (1 << 30);
        for(int i = 0; i < no_of_process; i++) {
            if(p[i].remin_time_io != 0) continue;      // check if this process waits in i/o waiting queue
            if(p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if(burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if(burst_remaining[i] == mn) {
                    if(p[i].arrival_time < p[idx].arrival_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }
        if(idx != -1){
            if(burst_remaining[idx] == p[idx].instruction_count) {
                p[idx].start_time = current_time;
            }
            burst_remaining[idx] -= 1;
            int rand_num = 1 + (rand() % 100);   // generate random number from 1 to 100
            if(rand_num <= p[idx].io_percentage){
                p[idx].remin_time_io = io_time;
                io_waiting.push(p[idx]);
                printf("(P%d -> i/o)\n", p[idx].pid);
            }
            else{
                printf("P%d -> ", idx + 1);
            }
            if(burst_remaining[idx] == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].response_time = p[idx].start_time - p[idx].arrival_time;

                total_turnaround_time += p[idx].turnaround_time;
                total_response_time += p[idx].response_time;

                is_completed[idx] = 1;
                completed++;
            }
            current_time++;
        }
        else {
            printf("NO ");
            current_time++;
        }
        //update the i/p waiting queue
        if(!io_waiting.empty()){
            if(io_waiting.front().remin_time_io != 1) io_waiting.front().remin_time_io--;
            else{
                for(int i = 0; i < no_of_process; ++i) //return the process to ready state
                    if(p[i].pid == io_waiting.front().pid) p[i].remin_time_io = 0;
                io_waiting.pop();
            }
        }
    }
	
    for(auto x: chart) cout << x.first <<' ' << x.second <<'\n';
    puts("\n+-----+------------+-----------------+-----------------+---------------+");
    puts("| PID | Start Time | Completion Time | Turnaround Time | Response Time |");
    puts("+-----+------------+-----------------+-----------------+---------------+");

    for(int i = 0; i < no_of_process; i++) {
        printf("| %2d  |     %2d     |      %2d         |        %2d       |        %2d     |\n"
                , p[i].pid, p[i].start_time, p[i].completion_time, p[i].turnaround_time, p[i].response_time);
        puts("+-----+------------+-----------------+-----------------+---------------+");
    }

    avg_turnaround_time = (float) total_turnaround_time / no_of_process;
    avg_response_time = (float) total_response_time / no_of_process;

 
    printf("Average Turnaround Time = %0.2f\n",avg_turnaround_time);
    printf("Average Response Time = %0.2f\n", avg_response_time);

}
//________________________________________________________________________________________________________________________________________
void SJF(){
    int number, i, j, min;
    float atat = 0, awt = 0; 
    printf("Enter the number process : ");
    scanf("%d", &number);
    write_randomly (number);
    get_info(number);
    //sort the process
    for (i = 0; i < number - 1; i++)
    {
            for (j = i; j < number; j++)
            {
                if (processSJF[i].at > processSJF[j].at)
                {
                    temp = processSJF[i];
                    processSJF[i] = processSJF[j];
                    processSJF[j] = temp;
                }
                if (processSJF[i].at == processSJF[j].at) // if same arrival time then sort by burst time
                {
                    if (processSJF[i].inst > processSJF[j].inst)
                    {
                        temp = processSJF[i];
                        processSJF[i] = processSJF[j];
                        processSJF[j] = temp;
                    }
                }
            }
    }
        
        
    
        processSJF[0].ct = processSJF[0].at + processSJF[0].inst;
        processSJF[0].tat = processSJF[0].inst;
        printf("process in order : %d\n", processSJF[0].p);
       // to check how many process will be in the readyqueue till cuurrent process completion's time
        int time = processSJF[0].ct;
        for (i = 1; i < number; i++)
        {
            // when CPU will be Idle
            if (processSJF[i].at > time)
            {
                processSJF[i].ct = processSJF[i].at + processSJF[i].inst;
                //printf("cpu was idle for %d to %d\n", time, process[i].at - 1);
                time = processSJF[i].ct;
                processSJF[i].wt = 0;
                processSJF[i].tat = processSJF[i].inst;
                printf("process in order : %d", processSJF[i].p);
            }
            else
            {
                // swapping current process with the one having least burst time and is in the readyqueue
                min = processSJF[i].inst;
                for (j = i; j < number; j++)
                {
                    if (processSJF[j].at <= time && processSJF[j].inst < min)
                    {
                        min = processSJF[j].inst;
                        temp = processSJF[i];
                        processSJF[i] = processSJF[j];
                        processSJF[j] = temp;
                    }
                }
                printf("process in order : %d\n", processSJF[i].p);
                processSJF[i].ct = time + processSJF[i].inst;
                time = processSJF[i].ct;
                processSJF[i].tat = processSJF[i].ct - processSJF[i].at;
                processSJF[i].wt = processSJF[i].tat - processSJF[i].inst;
            }
            atat += processSJF[i].tat;
            awt += processSJF[i].wt;
        }
        printf("process\t\tAt\tNI\tWT\tTAT\tCT\n");
        for (i = 0; i < number; i++)
            {
                printf("p%d\t\t%d\t%d\t%d\t%d\t%d\n", processSJF[i].p,processSJF[i].at, processSJF[i].inst,processSJF[i].wt, processSJF[i].tat, processSJF[i].ct);
                
            }
        printf("------------------------------\n\tGantChart\n");
        printf("--------------------------------\n") ;   
        for (i=0; i<number; i++){
            printf(" p%d|   ",processSJF[i].p);
        }
        printf("\n");
    
        for (i=0; i<number; i++){ 
            printf("   %d   ",processSJF[i].ct);
        }
        
        printf("\n");
        printf("----------------------------------\n") ;  
    
        printf("aAverage waiting time : %f\n", awt / number);
        printf("average turn around time : %f\n", atat / number);
}
//______________________FCFS_____________________________________________________________________________________________________________

// Scheduling the processes in FCFS Algorithm
void FCFS(){

    printf("                                                         First Come First Served Algorithm (FCFS)                                                 \n\n");

    struct Process P[1000];
    int remainning_inst[1000];

    deque <struct Process> waiting;
    deque <struct Process> running;

    write_to_file_randomly();
    get_input_FCFS(P);
    sort(P);

    for (int i = 0 ;i<no_of_process;++i){
        remainning_inst[i] = P[i].inst;
    }

    int start = P[0].AT,
    Response_Time = 0,
    AVG_Response_Time = 0,
    AVG_Turnaround_Time = 0,
    end_time = start;
    int finish [100];

    int curpidx = 0;
    int cur_time = 0;
    int completed = 0;

    while(completed != no_of_process){
        running.push_back(P[curpidx]);
        while(!running.empty()){
            while(running.front().inst){
                running.front().inst--;
                if (P[curpidx].inst == remainning_inst[curpidx]){
                    P[curpidx].st = cur_time;

                int x = rand()%100 + 1;
                if (x <= running.front().io_percentage){
                    printf("| P%d -> i/o |\n", running.front().pid);
                    running.front().remain_io_inst = io_time;
                    waiting.push_back(running.front());
                    running.pop_front();
                    break;
                }
                else{
                    printf("| P%d  ", running.front().pid);
                    }
                    cur_time++;
                    if(!waiting.empty()){
                        if(waiting.front().remain_io_inst != 1) waiting.front().remain_io_inst--;
                        else{
                            waiting.front().remain_io_inst = 0;
                            running.push_back(waiting.front());
                            waiting.pop_front();
                        }
                    }
                }
            }
            if(!running.empty()){
                if(running.front().inst == 0){
                    for(int i = 0; i < no_of_process; ++i){
                        if(P[i].pid == running.front().pid){
                            P[i].ct = cur_time;
                            break;
                        }
                    }
                    completed++;
                    running.pop_front();
                }
            }
        }
        curpidx++;
    }
    while(!waiting.empty()){
        cur_time += waiting.front().remain_io_inst;
        waiting.front().remain_io_inst = 0;
        int x = rand()%100 + 1;
        while(waiting.front().inst){
            waiting.front().inst--;
            if (x <= waiting.front().io_percentage){
                printf("| P%d -> i/o |\n", waiting.front().pid);
                waiting.front().remain_io_inst = io_time;
                break;
            }
            else {
                printf("| P%d  ", waiting.front().pid);
                cur_time++;
            }
        }
        if(waiting.front().inst == 0){
            for(int i = 0; i < no_of_process; ++i){
                if(P[i].pid == waiting.front().pid){
                    P[i].ct = cur_time;
                    break;
                }
            }
            waiting.pop_front();
        }
    }

    printf("| \n\nPID\tAT\tBT\tST\tCT\tRT\tTAT\n");
    for (int i = 0 ;i < no_of_process;++i){
        AVG_Response_Time   += abs(P[i].st-P[i].AT);
        AVG_Turnaround_Time += abs(P[i].ct-P[i].st);
        finish[i] = P[i].ct;
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
        P[i].pid,P[i].AT,P[i].inst,P[i].st,P[i].ct,abs(P[i].st-P[i].AT),abs(P[i].ct-P[i].AT));
    }

    printf("\n\n");
    printf("AVG_Response_Time = %.3f\n",(float)AVG_Response_Time/no_of_process);
    printf("AVG_Turnaround_Time = %.3f\n",(float) AVG_Turnaround_Time/no_of_process);
    printf("\n");
}
