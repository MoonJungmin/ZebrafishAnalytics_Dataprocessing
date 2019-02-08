#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <list>
#include <map>

using namespace std;


int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 100;
int VOLCOUNT = 5;



int *IndexTable;
vector<vector<int>> gVector;
list<int> sList;


int main(int argc, char **argv){

	string PATH(argv[1]);

	int start_volume = atoi(argv[2]);
	int end_volume = atoi(argv[3]);

	long long lvalue = 1;
	long long SliceSize = lvalue * DIMX * DIMY;
	unsigned int *mSlice1 = new unsigned int[SliceSize];

	ifstream ifs("/home/jmmoon/Projects/ConnectedComponent/shared/boundary/result_index_table.txt");

	int index = 0;
	while(!ifs.eof()){
		string s;
		ifs >> s;
		sList.push_back(atoi(s.c_str()));
	}
	long long maxValue = sList.size();
	cout << "max value is " << maxValue << endl;

	IndexTable = new int[maxValue];


	list<int>::iterator iter;
	for(iter=sList.begin();iter!=sList.end();++iter){
		IndexTable[index++] = *iter;
		//cout << IndexTable[index-1] <<endl;
	}


	cout << "generate done" << endl;

//return 0;	
	for(int i=start_volume;i<end_volume;++i){
		
		for(int j=0;j<DIMZ;++j){
			string path1 = PATH;
			path1 += "v" + to_string(i) + "_s" + to_string(j) +".raw";
			FILE *rfp1 = fopen(path1.c_str(), "rb");
			fread(mSlice1, SliceSize*sizeof(unsigned int), 1, rfp1);
			fclose(rfp1);

			cout << j << endl;

			for(int y=0; y<DIMY;++y){
				for(int x=0;x<DIMX;++x){
					if(mSlice1[y*DIMX + x] != 0){
						int root = mSlice1[y*DIMX + x];
						while(IndexTable[root] != -1){
							root = IndexTable[root];
						}
						//cout << root << endl;
						mSlice1[y*DIMX + x] = root;
					}
				}
			}

			string path2 = "/scratch/jmmoon/connectedcomponent_label/";
			path2 += "v" + to_string(i) + "_s" + to_string(j) + ".raw";
			cout << path2 << endl;
			FILE *wfp2 = fopen(path2.c_str(), "wb");
			fwrite(mSlice1, SliceSize*sizeof(unsigned int), 1, wfp2);
			fclose(wfp2);
		}
	}



	return 0;
}
