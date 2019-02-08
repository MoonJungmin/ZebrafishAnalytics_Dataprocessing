#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>



#define PATH "/scratch/jmmoon/connectedcomponent/"


using namespace std;


int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 100;
int VOLCOUNT = 5;
int NODESIZE = 16;

struct coord
{
	long long x;
	long long y;
	long long z;
};


int main(int argc, char **argv){

	int node_number = atoi(argv[1]);
	unsigned int *MAXArray = new unsigned int[VOLCOUNT*NODESIZE];
	unsigned int *SumArr = new unsigned int[VOLCOUNT*NODESIZE];
	
	for(int i=0;i<NODESIZE;++i){
		string rpath = "/home/jmmoon/Projects/ConnectedComponent/shared/max_" + to_string(i) + ".dat";	
		ifstream ifs(rpath, ifstream::in);
		
		for(int j=0;j<VOLCOUNT;++j){
			string data;
			ifs >> data;
			MAXArray[i*VOLCOUNT+j] = atoi(data.c_str());
		}			
	}
	cout << "Data Read" << endl;
	unsigned int sum = 0;
	for(int i =0 ;i<VOLCOUNT*NODESIZE;++i){
		SumArr[i] = sum;
		sum += MAXArray[i];
	}
	cout << "Generate Array" << endl;

	long long lvalue = 1;
	long long SliceSize = lvalue * DIMX * DIMY;
	unsigned int *mSlice = new unsigned int[SliceSize];

	
	for(int v=node_number*VOLCOUNT ; v<node_number*VOLCOUNT + 5;++v){
		for(int s=0; s<100; ++s){
			string slicepath(PATH);
			slicepath += "v" + to_string(v) + "_s" + to_string(s) +".raw" ;
			FILE *fp = fopen(slicepath.c_str(), "rb");
			fread(mSlice, SliceSize*sizeof(unsigned int), 1, fp);
			fclose(fp);


			for(int y = 0;y<DIMY;++y){
				for(int x = 0;x<DIMX;++x){
					if(mSlice[y*DIMX + x] > 0){
						mSlice[y*DIMX + x] += SumArr[v];
					}
				}
			}

			FILE *wfp = fopen(slicepath.c_str(), "wb");
			fwrite(mSlice, SliceSize*sizeof(unsigned int), 1, wfp);
			fclose(wfp);
		}
	}

	return 0;
}
