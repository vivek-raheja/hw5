

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
size_t findMin(vector<size_t> values);
size_t findMax(vector<size_t> values);
int findIndex(vector<size_t> arr, size_t value);
int findIndexUnsigned(vector<Worker_T> arr, Worker_T value);
bool performBackTracking(const AvailabilityMatrix&, const size_t, const size_t, DailySchedule&, size_t);

vector<size_t> totAvailablePerDay;
vector<size_t> totWorkerCapacity;
vector<size_t> totWorkerUsed;
size_t minVal = 0;
vector<size_t> totAvailablePerDaySorted;
vector<size_t> totWorkerCapacitySorted;
vector<size_t> totWorkerCapacityRecursive;
size_t dayIndex;

// Add your implementation of schedule() and other helper functions here
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    size_t count;
	size_t count2;

	//Step 1. Find sum of workers available on all days
	for(count=0;count<avail.size();count++) {
		vector<bool> currMatrix = avail[count];
		size_t totWorkers = 0;
		for(count2=0;count2<currMatrix.size();count2++) {
			if(currMatrix[count2]) {
				totWorkers++;
			}
		}
		if(count == 0) {
			minVal = totWorkers;
		} else if(totWorkers < minVal) {
			minVal = totWorkers;
		}
		totAvailablePerDay.push_back(totWorkers);
		
		//cout << "Day " << (count+1) << ". " << totWorkers << " " << dailyNeed << endl;
		
		//Initialize the schedule
		vector<Worker_T> vec;
		sched.push_back(vec);
	}

	if(minVal < dailyNeed) {
		//Do not have daily workers available
		return false;
	}

	//Step 2. Find Total capacity of workers
	for(count=0;count<avail[0].size();count++) {
		//Initialize the worker capacity
		totWorkerCapacity.push_back(0);
		totWorkerUsed.push_back(0);
	}
	for(count=0;count<avail.size();count++) {
		vector<bool> currMatrix = avail[count];
		for(count2=0;count2<currMatrix.size();count2++) {
			if(currMatrix[count2]) {
				totWorkerCapacity[count2]++;
			}
		}
	}
	
	//Step 3. Perform back tracking (recursive)
	//Sort the availability array to check minimum workers available on each day
	totAvailablePerDaySorted = totAvailablePerDay;
	sort(totAvailablePerDaySorted.begin(), totAvailablePerDaySorted. end());
	
	totWorkerCapacitySorted = totWorkerCapacity;
	totWorkerCapacityRecursive = totWorkerCapacity;
	sort(totWorkerCapacitySorted.begin(), totWorkerCapacitySorted. end(), greater<size_t>());
	
	return performBackTracking(avail, dailyNeed, maxShifts, sched, 0);
}

bool performBackTracking(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, size_t backTrack) {
	if(backTrack >= totAvailablePerDaySorted.size())
	{
		return (sched[dayIndex].size() == dailyNeed);
	}
	
	size_t count2;
	size_t availVal = totAvailablePerDaySorted[backTrack];
	dayIndex = findIndex(totAvailablePerDay, availVal);
	
	totWorkerCapacityRecursive = totWorkerCapacity;
	
	//Step 4. Use these workers to find maximum indices in the Workers array sets
	for(count2=0;count2<totWorkerCapacitySorted.size();count2++) {
		size_t currentCapacity = totWorkerCapacitySorted[count2];
		
		int workerIndex = findIndex(totWorkerCapacityRecursive, currentCapacity);
		size_t workerUsed = totWorkerUsed[workerIndex];
		
		if(workerUsed < maxShifts && avail[dayIndex][workerIndex]) {
			//This worker is available, check if its needed in schedule
			if(sched[dayIndex].size() < dailyNeed && findIndexUnsigned(sched[dayIndex], workerIndex) == -1) {
				sched[dayIndex].push_back(workerIndex);
				
				//Reduce the worder capacity
				totWorkerCapacity[workerIndex]--;
				totWorkerCapacitySorted[count2]--;
				totWorkerCapacityRecursive[workerIndex] = 0;
				totWorkerUsed[workerIndex]++;
				
				if(sched[dayIndex].size() == dailyNeed)
					break;
			}
		}
		totWorkerCapacityRecursive[workerIndex] = 0;
	}
	
	if(sched[dayIndex].size() == dailyNeed) {
		//Mark this day as scheduled
		totAvailablePerDay[dayIndex] = 0;
		totAvailablePerDaySorted[backTrack] = 0;
		
		sort(totWorkerCapacitySorted.begin(), totWorkerCapacitySorted. end(), greater<size_t>());	
	} else {
		return false;
	}
	
	
	return performBackTracking(avail, dailyNeed, maxShifts, sched, backTrack+1);
}

size_t findMin(vector<size_t> values) {
	return *min_element(values.begin(), values.end());
}

size_t findMax(vector<size_t> values) {
	return *max_element(values.begin(), values.end());
}

int findIndex(vector<size_t> arr, size_t value) {
	std::vector<size_t>::iterator it;
	it = find (arr.begin(), arr.end(), value);
	if (it != arr.end())
    {
        return it - arr.begin();
    }
    else
        return -1;
}

int findIndexUnsigned(vector<Worker_T> arr, Worker_T value) {
	unsigned int count;
	for(count=0;count<arr.size();count++) {
		if(arr[count] == value)
			return count;
	}
	
	return -1;
} 

template<typename T>
void printArray(string str, vector<T> vec) {
	cout << str << endl;
	for (auto x : vec)
        cout << x << " ";
	cout << endl;
}
