#pragma once

#include <iostream>
#include "math.h"
#include "time.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <bitset>
#include <fstream>
#include <string>
#include <utility>
#include "getArgs_GATSP.h"
#define GENERATION_SIZE 4
#define RUNS 10000
#define vsize 101
#define OPTFREQ 70
#define MUTE 5


using namespace std;
string root = "C:/Temp/NSC400/GATSP/source/";
long rr;
//Counter variables
long i, j, k, l , m , n, z=0, u, eliteTour, c1, c2, swapper,swap2,eliteRun;

long xval,yval, dist, average;

long **weights = new long *[vsize];
long lastC;
bool valid,immigrate=false;
vector<long> track;
vector<long> temp;
vector<long> cities;
long temprank[GENERATION_SIZE];

double * BubbleSort(double * sortinglist) {
	for (i=0;i< GENERATION_SIZE-1;i++)
	 {
		 for (j=0; j < GENERATION_SIZE-1-i; j++)
		 {
			 if (sortinglist[j+1] < sortinglist[j])
			 {
				 swapper = sortinglist[j];
				 swap2 = temprank[j];
				 sortinglist[j] = sortinglist[j+1];
				 temprank[j] = temprank[j+1];
				 sortinglist[j+1] = swapper;
				 temprank[j+1]=swap2;
				 
			 }
		 }
		 
	 }
	 
	 return sortinglist;
}

void writeTour(vector<long> tour) {
	ofstream tourFile;
	tourFile.open(root+"Tour.txt");
	for (i = 0; i< tour.size();i++) {
		tourFile << i << " "<< weights[tour[i]][0] << " " << weights[tour[i]][1] << endl;
	}
	tourFile.close();
}

void prlongArray( long * a, long num) {
	
	for (u = 0; u < num; u++)
	{
		cout << a[u] << " ";
	}
	cout <<endl;
}

long getBound() {
	long bound =0;
	for (int i=0;i<vsize-1;i++)
		bound += sqrt((pow(weights[i+1][0]-weights[i][0],2))+(pow(weights[i+1][1]-weights[i][1],2)));
	bound += sqrt((pow(weights[vsize-1][0]-weights[0][0],2))+(pow(weights[vsize-1][1]-weights[0][1],2)));
	//cout << "Max Dist: " <<bound<< endl;
	bound= bound/vsize;
	//cout<< "Bound: " <<bound << endl;
	return bound;
}

void getMap() {
	ifstream tsp;
	char* read;
	
	tsp.open(file);
	for (i = 0; i < vsize; i++) {
		track.push_back(i);
		tsp.getline(read,100,' ');
		if (atoi(read) == i+1) {
			tsp.getline(read,100,' ');
			weights[i][0] = atoi(read);
			tsp.getline(read,100,'\n');
			weights[i][1] = atoi(read);
			}
		//cout << i+1 << " " << weights[i][0] << " " << weights [i][1] << endl;
		}
}

long nearestNeighbor(vector<long> c, long prevCity, long bound) {
	bool found = true;
	long n = 0;
	long city;
	while (found && n < c.size()) {
		city = rand()%c.size();
		
		if (sqrt((pow(weights[c[city]][0]-weights[prevCity][0],2))+(pow(weights[c[city]][1]-weights[prevCity][1],2))) <= bound)
			found = false;
		
		n++;
		c.erase(c.begin()+city);
	}
	
	return city;
}

vector<long> randTour(long bound) {
	//initialize variables
	temp = track;
	vector<long> ind;
	long city;
	//add the first city, remove it from options
	ind.push_back(temp[0]);
	temp.erase(temp.begin());
	//find the next city
	for (int i=1;i<vsize;i++) {
		city = nearestNeighbor(temp,ind.back(),bound);
		ind.push_back(temp[city]);
		temp.erase(temp.begin() + city);
	}
	//add last city
	ind.push_back(0);
	
	return ind;
}




