#include <stdio.h>
#define MAXJOB 7

typedef struct {
    int pageID;
    int pages;
    int arrivalTime;        
    int printTime;         // assume it takes 2s to print each page
    int startTime;
    int finishTime;
    int waitingTime;
    int turnaroundTime;
    int completed;
} PrintJob;

void sortBySJF(PrintJob jobs[] , int n) {
    for(int i = 0; i < MAXJOB-1; i++) {
        for(int j = i+1; j < MAXJOB; j++) {
            if(jobs[i].printTime > jobs[j].printTime) {
                PrintJob temp = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = temp;
            }
        }
    }
}

void setprintTime(PrintJob jobs[] , int n) {
    for(int i = 0; i < n; i++) {
        jobs[i].printTime = jobs[i].pages * 2;
    }
}

int main() {
    // Initialize jobs with page ID, number of pages, and arrival time
    PrintJob jobs[] = {
        // {document id, document number of pages, arrival time}
        {1, 15, 0},
        {2, 30, 2},
        {3, 22, 2},
        {4, 15, 6},
        {5, 20, 8},
        {6, 13, 10},
        {7, 5, 12}
    };
    
    int n = sizeof(jobs) / sizeof(jobs[0]); // Calculate the number of jobs
    
    // Initialize completion status
    for(int i = 0; i < n; i++) {
        jobs[i].completed = 0;
    }
    
    setprintTime(jobs, n);
    
    // Display the unsorted jobs
    printf("--- Initial Job Queue ---\n");
    printf("ID\tPages\tArrival\tPrint Time\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\n", jobs[i].pageID, jobs[i].pages, jobs[i].arrivalTime, jobs[i].printTime);
    }
    
    // Sort by Shortest Job First (for demonstration)
    sortBySJF(jobs , n);
    
    // Display the sorted jobs
    printf("\n--- Sorted by SJF ---\n");
    printf("ID\tPages\tArrival\tPrint Time\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\n", 
            jobs[i].pageID, jobs[i].pages, jobs[i].arrivalTime, jobs[i].printTime);
    }
    
    // Reset completion status for the actual simulation
    for(int i = 0; i < n; i++) {
        jobs[i].completed = 0;
    }
    
    int currentTime = 0, completed = 0;  //initializing current time and number of completion
    
    printf("\n--- Printing Log ---\n");
    while(completed < n) {
        int idx = -1;
        int minTime = 99999;  // reseting minTime 
        
        for(int i = 0; i < n; i++) {     
            if(jobs[i].arrivalTime <= currentTime && !jobs[i].completed) {   // check if the document arrived
                if(jobs[i].printTime < minTime) { // check if the document is the shortest
                    minTime = jobs[i].printTime;
                    idx = i;
                }
            }
        }
        // calculate and store pcbs values , waiting time and turnaround time 
        if(idx != -1) {
            jobs[idx].startTime = currentTime;
            jobs[idx].finishTime = currentTime + jobs[idx].printTime;
            jobs[idx].waitingTime = jobs[idx].startTime - jobs[idx].arrivalTime;
            jobs[idx].turnaroundTime = jobs[idx].finishTime - jobs[idx].arrivalTime;
            currentTime = jobs[idx].finishTime;
            jobs[idx].completed = 1;
            completed++;
            printf("Time %d: Printing document %d (Pages: %d, Print time: %d)... Done at %d\n",
                jobs[idx].startTime, jobs[idx].pageID, jobs[idx].pages,jobs[idx].printTime, 
                jobs[idx].finishTime);
        } else {
            printf("Time %d: Printer is idle...\n", currentTime);
            currentTime++;
        }
    }
    
    printf("\n--- Summary Table ---\n");
    printf("ID\tArrival\tPages\tPrint\tStart\tFinish\tWaiting\tTurnaround\n");
    for(int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            jobs[i].pageID,jobs[i].arrivalTime,jobs[i].pages,jobs[i].printTime,jobs[i].startTime,jobs[i].finishTime,
            jobs[i].waitingTime,jobs[i].turnaroundTime);
    }
    
    // Calculate and display average waiting and turnaround times
    float avgWaiting = 0, avgTurnaround = 0;
    for(int i = 0; i < n; i++) {
        avgWaiting += jobs[i].waitingTime;
        avgTurnaround += jobs[i].turnaroundTime;
    }
    avgWaiting /= n;
    avgTurnaround /= n;
    
    printf("\nAverage Waiting Time: %.2f\n", avgWaiting);
    printf("Average Turnaround Time: %.2f\n", avgTurnaround);
    
    return 0;
}