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



#define PATH "/scratch/jmmoon/connectedcomponent/"


using namespace std;


int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 100;
int VOLCOUNT = 5;


struct coord
{
	long long x;
	long long y;
	long long z;
};


int main(int argc, char **argv){

	int node_number = atoi(argv[1]);
	unsigned int *MAXArray = new unsigned int[VOLCOUNT];
	memset(MAXArray, 0, sizeof(int)*VOLCOUNT);

	cout << "Start MAX Process " << endl;
	
	long long lvalue = 1;
	long long SliceSize = lvalue * DIMX * DIMY;
	unsigned int *mSlice = new unsigned int[SliceSize];

	cout << "Alloc step" << endl;
	for(int v=0;v<VOLCOUNT;++v){
		for(int s=0;s<100;++s){
			string slicepath(PATH);
			slicepath += "v" + to_string(VOLCOUNT*node_number + v) + "_s" + to_string(s) + ".raw" ;
			cout << slicepath << endl;
			FILE *fp = fopen(slicepath.c_str(), "rb");
			fread(mSlice, SliceSize*sizeof(unsigned int), 1, fp);
			fclose(fp);
			cout << "Data Read" << endl;
			//Get Max 
			for(int y = 0;y<DIMY;++y){
				for(int x = 0;x<DIMX;++x){
					//cout << mSlice[y*DIMX + x] << endl;
					if(mSlice[y*DIMX + x] > MAXArray[v]){
						MAXArray[v] = mSlice[y*DIMX + x];
					}
				}
			}	
		}
		cout << "Process done " << MAXArray[v] << endl;
	}

	cout << "Export" << endl;
	string wpath = "/home/jmmoon/Projects/ConnectedComponent/shared/max_" + to_string(node_number) + ".dat";
	FILE *wfp = fopen(wpath.c_str(), "wb");
	string data;
	for(int i=0;i<VOLCOUNT;++i){
		data += to_string(MAXArray[i]) + " ";
	}
	fwrite(data.c_str(), data.length(), 1, wfp);
	fclose(wfp);

	return 0;
}
