//Author: Kevin Mody
#include <iostream>
#include <iomanip>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

//clreating classes
class task {
        public:
        char taskID;
        int orginal_time;
        int arrival_time;
        int final_time;
        int num_tasks;
        int task_counter;
        int begin_queue;
        int end_queue;

};

class task2{
        public:
        int x;
        int y;
        int placement;
};

bool task2_sort(const task2 &a, const task2 &b){
if (a.x < b.x) return true;
if (a.x > b.x) return false;
if (a.y< b.y) return true;
if (a.y > b.y) return false;
return false;
}
void FIFO ();
void SJF ();
void RR();

int main(int argc, char *argv[]){
    vector<task> myVector1;
    string function = argv[1];
    int x1,x2;
    char taskID = 'A';
//Available time holds the open slot for putting things in
    int availble_time =0;
    if(function == "-fifo"){
        FIFO();
    }

    else if (function == "-sjf"){
        SJF();
    }

    else if (function == "-rr") {
        RR();
    }

    else {
        cout<<"Incorrect specifier Teriminating"<<endl;
        return 0;
    }

    return 0;
}

/*
This function will read in the ints from the file and creak tasks with them
It will then mathemtically determine the start and end time as well as the
Queue time if ever has to be inside of the queue. It will then print the
neccesary graphs.
*/
void FIFO (){
    int x1,x2;
    char taskID = 'A';
    int availble_time =0;
    vector<task> myVector1;
    while (cin>> x1){
        cin>>x2;
        task temp; //class and the object
        temp.taskID = taskID;
        temp.orginal_time= x1;
        temp.arrival_time = x1;
        temp.final_time= x1+x2-1;
        temp.num_tasks = x2;
        temp.task_counter=x2;
        temp.begin_queue = -1;
        temp.end_queue = -1;
        for(auto i:myVector1){
            if (temp.arrival_time <= i.final_time){
                temp.begin_queue =temp.arrival_time;
                temp.end_queue = availble_time-1;
                temp.arrival_time = availble_time;
                temp.final_time = availble_time + temp.num_tasks-1;
            }
        }
        availble_time = temp.final_time+1;
        myVector1.emplace_back(temp);
        taskID++;
    }

    availble_time = myVector1[myVector1.size() -1].final_time+1;
//printing first table
    cout << "FIFO scheduling results\n\n";
    cout << "time\tcpu\tready\tqueue (taskID/rst)\n";
    cout<<"--------------------------------------------"<<endl;
    for(int i = 0; i< availble_time; i++ ){
        cout<< i<<"\t";
        for (auto &x: myVector1){
            if(i>=x.arrival_time && i<=x.final_time){
                cout<<x.taskID <<x.task_counter<<"\t";
                x.task_counter-=1;
                //cout<<"\t"<< " Task counter is now: "<< x.task_counter;
            }

            if(i>=x.begin_queue && i<=x.end_queue){
                cout<<x.taskID<<x.task_counter<<"\t";
            }
        }
        cout<<endl;
    }
//printing second table
    cout << "\n\tarrival\tservice\tcompletion\tresponse\twait\n";
    cout << "taskID\ttime\ttime\ttime\t\ttime\t\ttime\n";
    for(int i = 0; i < 60; i++){
        cout << "-";
    }
    cout << "\n";
    for(auto item:myVector1){
        int response_time = item.num_tasks+(item.end_queue-item.begin_queue)+1;
        int wait = item.end_queue-item.begin_queue+1;
        if(item.end_queue == -1){
            wait=0;
            response_time=item.num_tasks;
        }
        cout<<item.taskID<<"\t"<<item.orginal_time<<"\t"<<item.num_tasks<<"\t";
        cout<<item.final_time+1<<"\t\t"<<response_time<<"\t\t"<<wait<<endl;
    }
//printing third table
    cout<<"\n";
    cout<<"service " << "wait"<<endl;
    cout<<"time\t"<<"time"<<endl;
    cout<<"-----------------------------------------------------------"<<endl;
    for (vector<task>::reverse_iterator i = myVector1.rbegin();
         i != myVector1.rend(); ++i ) {
        int wait = i->end_queue-i->begin_queue+1;
        if(i->end_queue == -1){
            wait=0;
        }
        cout<<i->num_tasks<<"\t"<<wait<<endl;
    }


} // END OF FIFO

// SJF works just like FIFO
// Here I created a Pair class that reads in the integers and stores them
// It is then sorted. From there It works just like FIFO but instead
// It reads from the Pair vector instead.
void SJF (){
    vector<task> myVector1;
    vector<task2> myVector2;

    char taskID ='@';
    int x1,x2;
    int x3=1;
    while(cin>>x1){
        cin>>x2;
        task2 temp; //class and the object
        temp.x=x1;
        temp.y=x2;
        temp.placement = x3;
        x3+=1;
        myVector2.emplace_back(temp);
    }
    sort(myVector2.begin(),myVector2.end(),task2_sort);
    for(auto &x:myVector1){
        x.taskID = taskID;
        taskID++;
    }
    //Now its just FIFO
    int availble_time =0;

    for(auto p:myVector2){
        task temp;
        temp.taskID = taskID+p.placement;
        temp.orginal_time = p.x;
        temp.arrival_time = p.x;
        temp.final_time= p.x+p.y-1;
        temp.num_tasks = p.y;
        temp.task_counter=p.y;
        temp.begin_queue = -1;
        temp.end_queue = -1;
        for(auto i:myVector1){
            if (temp.arrival_time <= i.final_time){
                temp.begin_queue =temp.arrival_time;
                temp.end_queue = availble_time-1;
                temp.arrival_time = availble_time;
                temp.final_time = availble_time + temp.num_tasks-1;
            }
        }
        availble_time = temp.final_time+1;
        myVector1.emplace_back(temp);
    }
    availble_time = myVector1[myVector1.size() -1].final_time+1;

    //printing first table
    cout << "SJF(preemptive) scheduling results\n\n";
    cout << "time\tcpu\tready\tqueue (taskID/rst)\n";

    for(int i = 0; i< availble_time; i++ ){
        cout<< i<<"\t";
        for (auto &x: myVector1){
            if(i>=x.arrival_time && i<=x.final_time){
                cout<<x.taskID;
                cout<<x.task_counter << "    ";
                x.task_counter-=1;
            }

            else if(i>=x.begin_queue && i<=x.end_queue&&x.begin_queue!=0){
                cout<<"\t" << x.taskID<<x.task_counter;
            }
        }
        cout<<"\n";
    }
    //printing second table
    cout << "\n\tarrival\tservice\tcompletion\tresponse\twait\n";
    cout << "taskID\ttime\ttime\ttime\t\ttime\t\ttime\n";
    for(int i = 0; i < 60; i++){
        cout << "-";
    }
    cout << "\n";
    for(auto item:myVector1){
        int response_time = item.num_tasks+(item.end_queue-item.begin_queue)+1;
        int wait = item.end_queue-item.begin_queue+1;
        if(item.end_queue == -1){
            wait=0;
            response_time=item.num_tasks;
        }
        cout<<item.taskID<<"\t"<<item.orginal_time<<"\t"<<item.num_tasks<<"\t";
        cout<<item.final_time+1<<"\t\t"<<response_time<<"\t\t"<<wait<<endl;
    }
    //printing third table
    cout<<"\n";
    cout<<"service " << "wait"<<endl;
    cout<<"time\t"<<"time"<<endl;
    cout<<"-----------------------------------------------------------"<<endl;
    for (vector<task>::iterator i = myVector1.begin();
         i != myVector1.end(); ++i ) {
        int wait = i->end_queue-i->begin_queue+1;
        if(i->end_queue == -1){
            wait=0;
        }
        cout<<i->num_tasks<<"\t"<<wait<<endl;
    }


} // END sjf

void RR(){
    cout<<"Could not figure out... :)"<<endl;
    /* My idea for this implementation was to use 3 vectors to store the different
    States and then move through them. But I could not get it to work :(
    */
}