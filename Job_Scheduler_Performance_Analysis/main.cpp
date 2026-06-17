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
using namespace std;

//randomJobGenrator generates a job.txt file with random job info as required by project specifications.
void randomJobGenerator();
//getJobFile will load the job.txt file to be parsed by jobTimes function.
ifstream getJobFile();
//parseJobFile will parse input file jobList to return a vector with job times.
vector<int> parseJobFile(ifstream& jobList);
//The four algorthms to be analyzed: RR (Times Slices 2 and 5 minutes), FCFS, SJF.
void RoundRobin2(vector<int>& jobTime);
void RoundRobin5(vector<int>& jobTimes);
void FirstComeFirstServe(vector<int>& jobTimes);
void ShortestJobFirst(vector<int>& jobTimes);



int main(int argc, const char * argv[]) {
    randomJobGenerator();
    ifstream jobList = getJobFile();
    
}

void randomJobGenerator(){
    
}

vector<int> parseJobFile(ifstream& jobList){
    vector<int> jobTimes;
    
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

