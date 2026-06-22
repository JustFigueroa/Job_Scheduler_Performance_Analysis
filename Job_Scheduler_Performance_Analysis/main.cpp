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
using namespace std;

//randomJobGenrator generates a job.txt file with random job info as required by project specifications.
void randomJobGenerator(int numOfJobs);
//getJobFile will load the job.txt file to be parsed by jobTimes function.
ifstream getJobFile(const char * argv[]);
//parseJobFile will parse input file jobList to return a vector with job times.
vector<int> parseJobFile(ifstream& jobList);
//The four algorthms to be analyzed: RR (Times Slices 2 and 5 minutes), FCFS, SJF.
void RoundRobin2(vector<int>& jobTime);
void RoundRobin5(vector<int>& jobTimes);
void FirstComeFirstServe(vector<int>& jobTimes);
void ShortestJobFirst(vector<int>& jobTimes);



int main(int argc, const char * argv[]) {
    for (int i = 5; i <= 30; i++){
        if (i % 5 == 0){
            randomJobGenerator(i);
            ifstream jobList = getJobFile(argv);
            vector<int> jobTimes = parseJobFile(jobList);
        }
        else{
        }
    }
}

//randomJobGenrator generates a job.txt file with random job info as required by project specifications.
void randomJobGenerator(int numOfJobs){
    stringstream ss;
    int seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    srand(seed);
    
    for (int i = 1; i <= numOfJobs; i ++){
        int burst = rand() % 100 + 1;
        ss << "Job " << i << "\n";
        ss << burst << "\n";
    }
    ofstream OutFile("job.txt");
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

//parseJobFile will parse input file jobList to return a vector with job times.
vector<int> parseJobFile(ifstream& jobList){
    vector<int> jobTimes;
    string line;
    int x;
    if (!jobList.is_open()){
        cout << "File not found";
    }
    while (getline(jobList, line)){
        istringstream ss(line);
        
        if (ss >> x){
            jobTimes.push_back(x);
        }
    }
    return jobTimes;
}

void RoundRobin2(vector<int>& jobTimes){
    
}

void RoundRobin5(vector<int>& jobTimes){
    
}

void FirstComeFirstServe(vector<int>& jobTimes){
    
}

void ShortestJobFirst(vector<int>& jobTimes){
    
}

