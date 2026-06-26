//
//  main.cpp
//  Job_Scheduler_Performance_Analysis
//
//  This program is designed to analyze the efficiency of different job scheduling algorithms.
//
//  Created by Justin Figueroa on 6/17/26.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <iomanip>
using namespace std;

//Struct used to store job data
struct job{
    int id = 0;
    int burst = 0;
    int startTime = 0;
    int remainingTime = 0;
    int completeTime = 0;
    bool isComplete = false;
};
//End Struct


//PROTOTYPES//
//randomJobGenrator generates a job.txt file with random job info as required by project specifications.
void randomJobGenerator(int numOfJobs, const char * argv[]);
//getJobFile will load the job.txt file to be parsed by jobTimes function.
ifstream getJobFile(const char * argv[]);
//parseJobFile will parse input file jobList to return a vector with job times.
vector<job> parseJobFile(ifstream& jobList);
//The four algorthms to be analyzed: RR (Times Slices 2 and 5 minutes), FCFS, SJF.
double RoundRobin2(vector<job> jobs);
double RoundRobin5(vector<job> jobs);
double FirstComeFirstServe(vector<job> jobs);
double ShortestJobFirst(vector<job> jobs);
//quicksort for SJF
int partition(vector<job>& jobs, int low, int high);
void quickSort(vector<job>& jobs, int low, int high);
//outputScheduleTable will display relevant shedulet table
void outputScheduleTable(vector<job>& jobs);
//outputAvgTurnTime will display the ATT per algorithm per set of jobs
void outputAvgTurnTime(vector<double>& AvgTurnTimes);
//outPutAvgerageATT will display the Average of the Average Turn Around Times
void outputAverageATT(vector<double>& AvgRR2TurnTimes, vector<double>& AvgRR5TurnTimes, vector<double>& AvgFCFSTurnTimes, vector<double>& AvgTurnTimes);
//END PROTOYPES//


int main(int argc, const char * argv[]) {
    //DECLARTACTIONS
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    double RR2AvgTurnTime = 0;
    double RR5AvgTurnTime = 0;
    double FCFSAvgTurnTime = 0;
    double SJFAvgTurnTime = 0;

    //Two loops, the first will calculate averages and averages of averages for 5, 10, ..., 30 jobs
    //second Loop does each 20 runs of random values for each 5, 10, ... , 30 jobs
        for (int i = 5; i <= 30; i+=5){
            vector<double> avgRR2TurnTimes;
            vector<double> avgRR5TurnTimes;
            vector<double> avgFCFSTurnTimes;
            vector<double> avgSJFTurnTimes;
            for (int j = 0; j < 20; j++){
            vector<double> avgTurnaroundTimes;
            randomJobGenerator(i, argv);
            ifstream jobList = getJobFile(argv);
            vector<job> jobs = parseJobFile(jobList);
            
            cout << "Round Robin Time Quantum 2: " << endl
            << "----------------------------" << endl;
 
            RR2AvgTurnTime = RoundRobin2(jobs);
            avgTurnaroundTimes.push_back(RR2AvgTurnTime);
            avgRR2TurnTimes.push_back(RR2AvgTurnTime);
           

            cout << endl << "Round Robin Time Quantum 5: " << endl
            << "----------------------------" << endl;
 
            RR5AvgTurnTime = RoundRobin5(jobs);
            avgTurnaroundTimes.push_back(RR5AvgTurnTime);
            avgRR5TurnTimes.push_back(RR5AvgTurnTime);
            
            cout << endl << "First Come First Serve: " << endl
            << "----------------------------" << endl;

            FCFSAvgTurnTime = FirstComeFirstServe(jobs);
            avgTurnaroundTimes.push_back(FCFSAvgTurnTime);
            avgFCFSTurnTimes.push_back(FCFSAvgTurnTime);
            
            cout << endl << "Shortest Job First: " << endl
            << "----------------------------" << endl;
            SJFAvgTurnTime = ShortestJobFirst(jobs);
            avgTurnaroundTimes.push_back(SJFAvgTurnTime);
            avgSJFTurnTimes.push_back(SJFAvgTurnTime);
            outputAvgTurnTime(avgTurnaroundTimes);
        }
        cout << "--------------------" << endl;
        cout << "Average of Averages for " << i << " jobs: " << endl;
        cout << "--------------------" << endl;
        outputAverageATT(avgRR2TurnTimes, avgRR5TurnTimes, avgFCFSTurnTimes, avgSJFTurnTimes);
    }
}


//randomJobGenrator generates a job.txt file with random job info as required by project specifications.
void randomJobGenerator(int numOfJobs, const char * argv[]){
    stringstream ss;
    int seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    srand(seed);
    
    for (int i = 1; i <= numOfJobs; i ++){
        int burst = rand() % 100 + 1;
        ss << "Job " << i << "\n";
        ss << burst << "\n";
    }
    filesystem::path exePath = filesystem::absolute(argv[0]);
    filesystem::path exeDir = exePath.parent_path();
    filesystem::path jobPath = exeDir / "job.txt";
    ofstream OutFile(jobPath);
    OutFile << ss.str();
}


//getJobFile will load the job.txt file to be parsed by jobTimes function.
ifstream getJobFile(const char * argv[]){
    filesystem::path exePath = std::filesystem::absolute(argv[0]);
    filesystem::path exeDir = exePath.parent_path();
    filesystem::path jobPath = exeDir / "job.txt";
    ifstream inFile(jobPath);
    if (!inFile) { cerr <<"File not found";}
    
    return inFile;
}


//parseJobFile will parse input file jobList to return a vector with job times
//Portions that have been commented our were to allow manual entry of job bursts for testing
vector<job> parseJobFile(ifstream& jobList){
  /*  bool isManual = false;
    int manualEntry;
    cout << "Manual Entry?" << endl;
    cin >> manualEntry;
    
    if (manualEntry == 0)
        isManual = false;
    else if (manualEntry == 1)
        isManual = true;
    */
    
    vector<int> jobTimes;
    vector<job> jobs;
    
    
    string line;
    int x;
    if (!jobList.is_open()){
        cerr << "File not found";
    }
    while (getline(jobList, line)){
        istringstream ss(line);
        
        if (ss >> x){
            jobTimes.push_back(x);
        }
    }
       // if (isManual == false){
            for (int i = 0; i < jobTimes.size(); i++){
                job current;
                current.id = i;
                current.burst = jobTimes[i];
                current.startTime = 0;
                current.completeTime = 0;
                current.remainingTime = current.burst;
                jobs.push_back(current);
            }
            return jobs;
        //}
        
        /* else if (isManual == true){
            for (int i = 0; i < jobTimes.size(); i++){
                int burst;
                job current;
                current.id = i;
                cout << "Job " << i << ": ";
                cin >> burst;
                current.burst = burst;
                current.startTime = 0;
                current.completeTime = 0;
                current.remainingTime = current.burst;
                jobs.push_back(current);
            }
            return jobs;
        */
}
    

//Round Robin Algorithm with Time Quantum 2
double RoundRobin2(vector<job> jobs){
    int currentTime = 0;
    int numOfJobs = jobs.size();
    double averageTurnaroundTime = 0;
    int jobsComplete = 0;
    while (jobsComplete < numOfJobs){
        for (int i = 0; i < jobs.size();){
            if (jobs[i].isComplete == true){
                i++;
            }
            
           else if (jobs[i].remainingTime <= 2) {
     
                    currentTime += jobs[i].remainingTime;
                    averageTurnaroundTime += currentTime;
                    jobs[i].completeTime = currentTime;
                    jobs[i].remainingTime = 0;
                    jobs[i].isComplete = true;
                    jobsComplete++;
                    i++;
            }
            else{
                jobs[i].remainingTime -= 2;
                currentTime += 2;
                i++;
            }
        }
    }
    outputScheduleTable(jobs);
    averageTurnaroundTime = averageTurnaroundTime / numOfJobs;
    return averageTurnaroundTime;
}


//Round Robin Algorithm With Time Quantum 5
double RoundRobin5(vector<job> jobs){
    int currentTime = 0;
    int numOfJobs = jobs.size();
    double averageTurnaroundTime = 0;
    int jobsComplete = 0;
    while (jobsComplete < numOfJobs){
        for (int i = 0; i < jobs.size();){
            
            if (jobs[i].isComplete == true){
                i++;
            }
            
           else if (jobs[i].remainingTime <= 5) {
     
                    currentTime += jobs[i].remainingTime;
                    averageTurnaroundTime += currentTime;
                    jobs[i].completeTime = currentTime;
                    jobs[i].remainingTime = 0;
                    jobs[i].isComplete = true;
                    jobsComplete++;
                    i++;
            }
            else{
                jobs[i].remainingTime -= 5;
                currentTime += 5;
                i++;
            }
        }
    }
    outputScheduleTable(jobs);
    averageTurnaroundTime = averageTurnaroundTime / numOfJobs;
    return averageTurnaroundTime;
}


//First Come First Serve Algorithm
double FirstComeFirstServe(vector<job> jobs){
    int currentTime = 0;
    double averageTurnaroundTime = 0;
    int numOfJobs = jobs.size();
    for (int i = 0; i < jobs.size(); i++){
        currentTime += jobs[i].burst;
        jobs[i].remainingTime = 0;
        jobs[i].completeTime = currentTime;
        averageTurnaroundTime += currentTime;
    }
    outputScheduleTable(jobs);
    averageTurnaroundTime = averageTurnaroundTime / numOfJobs;
    return averageTurnaroundTime;
}


//The Following Functions are used to sort the jobs for the Shortest Job First Algorithm
int partition(vector<job>& jobs, int low, int high){
    int pivot = jobs[low].burst;
    int i = low + 1;
    int j = high;
    while (true){
        while (i <= high && jobs[i].burst <= pivot){
            i++;
        }
        while (j >= low + 1 && jobs[j].burst > pivot){
            j--;
        }
        if (i >= j){
            break;
        }
        swap(jobs[i], jobs[j]);
    }
        swap(jobs[low], jobs[j]);
        return j;
}
void quickSort(vector<job>& jobs, int low, int high){
    if (low < high){
        int pivotIndex = partition(jobs, low, high);
        quickSort(jobs, low, pivotIndex-1);
        quickSort(jobs, pivotIndex + 1, high);
    }
}


//Shortest Job First Algorithm
double ShortestJobFirst(vector<job> jobs){
    int currentTime = 0;
    int numOfJobs = jobs.size();
    double avgTurnaroundTime = 0;
    if (!jobs.empty()){
        quickSort(jobs, 0, jobs.size() - 1);
    }
    for (int i = 0; i < jobs.size(); i++){
        currentTime += jobs[i].burst;
        jobs[i].completeTime = currentTime;
        jobs[i].remainingTime = 0;
        jobs[i].isComplete = true;
        avgTurnaroundTime += jobs[i].completeTime;
    }
    
    outputScheduleTable(jobs);
    avgTurnaroundTime = avgTurnaroundTime / numOfJobs;
    return avgTurnaroundTime;
}


//Outputs Schedule Table and Relevant Info
void outputScheduleTable(vector<job>& jobs){
    cout << left;
    cout << setw(8)  << "Job"
         << setw(10) << "Burst"
         << setw(12) << "Arrival"
         << setw(12) << "Complete"
         << endl;
    cout << string(39, '-') << endl;
    for (int i = 0; i < jobs.size(); i++){
        cout << setw(8) << jobs[i].id << setw(10) << jobs[i].burst << setw(12) << jobs[i].startTime << setw(12) << jobs[i].completeTime << setw(12)  << endl << endl;
    }
}


//Outputs Average Turn around Time Per Set of Jobs
void outputAvgTurnTime(vector<double>& AvgTurnTimes){
    cout << "Average Turnaround Time: " << endl;
    cout << "------------------------" << endl;
    cout << "RR2: " << AvgTurnTimes[0] << endl;
    cout << "RR5: " << AvgTurnTimes[1] << endl;
    cout << "FCFS: " << AvgTurnTimes[2] << endl;
    cout << "SJF: " << AvgTurnTimes[3] << endl << endl;
}


void outputAverageATT(vector<double>& AvgRR2TurnTimes, vector<double>& AvgRR5TurnTimes, vector<double>& AvgFCFSTurnTimes, vector<double>& AvgSJFTurnTimes){
    
    int numOfAverages = AvgSJFTurnTimes.size();
    double averageRoundRobin2 = 0;
    double averageRoundRobin5 = 0;
    double averageFirstComeFirstServe = 0;
    double averageShortestJobFirst = 0;
    
    for (int i = 0; i < numOfAverages; i++){
        averageRoundRobin2 += AvgRR2TurnTimes[i];
        averageRoundRobin5 += AvgRR5TurnTimes[i];
        averageFirstComeFirstServe += AvgFCFSTurnTimes[i];
        averageShortestJobFirst += AvgSJFTurnTimes[i];
    }
    averageRoundRobin2 = averageRoundRobin2 / numOfAverages;
    averageRoundRobin5 = averageRoundRobin5 / numOfAverages;
    averageFirstComeFirstServe = averageFirstComeFirstServe / numOfAverages;
    averageShortestJobFirst = averageShortestJobFirst / numOfAverages;
    

    cout << "RR2: " << averageRoundRobin2 << endl;
    cout << "RR5: " << averageRoundRobin5 << endl;
    cout << "FCFS: " << averageFirstComeFirstServe << endl;
    cout << "SJF: " << averageShortestJobFirst << endl;
    
};

