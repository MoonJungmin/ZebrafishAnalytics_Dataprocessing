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

int NODESIZE = 16;


int *IndexTable;
vector<vector<int>> gVector;
// list<int> sList;

void checktree(int a1, int a2){

	int isA1 = IndexTable[a1];
	int isA2 = IndexTable[a2];

	// cout << "checktree" << endl;
	if(isA1 != -2 && isA2 != -2){
		// cout << "case 1" << endl;
		if(isA1 != -1 && isA2 != -1){
			
			int root1 = isA1;
			int root2 = isA2;

			while(IndexTable[root1] != -1)
				root1 = IndexTable[root1];

			while(IndexTable[root2] != -1)
				root2 = IndexTable[root2];

			if(root1 > root2){
				IndexTable[root1] = root2;
			}

			if(root1 < root2){
				IndexTable[root2] = root1;
			}

		}
		else if(isA1 == -1 && isA2 == -1){
			int root2 = isA2;
			while(IndexTable[root2] != -1)
				root2 = IndexTable[root2];

			if(a1 > root2){
				IndexTable[a1] = root2;
			}

			if(a1 < root2){
				IndexTable[root2] = a1;
			}

		}
		else if(isA1 != -1 && isA2 == -1){
			int root1 = isA1;
			while(IndexTable[root1] != -1)
				root1 = IndexTable[root1];

			if(root1 > a2){
				IndexTable[root1] = a2;
			}

			if(root1 < a2){
				IndexTable[a2] = root1;
			}
		}
		else if(isA1 == -1 && isA2 == -1){
			if(a1 > a2){
				IndexTable[a1] = a2;
			}

			if(a1 < a2){
				IndexTable[a2] = a1;
			}
		}
	}
	else if(isA1 == -2 && isA2 == -2){
		// cout << "case 2" << endl;
		if(a1 > a2){
			IndexTable[a1] = a2;
			IndexTable[a2] = -1;
		}

		if(a1 < a2){
			IndexTable[a2] = a1;
			IndexTable[a1] = -1;
		}
	}
	else if(isA1 == -2 && isA2 != -2){
		// cout << "case 3" << endl;
		int root2 = a2;
		while(IndexTable[root2] != -1)
			root2 = IndexTable[root2];

		IndexTable[a1] = root2;
	}
	else if(isA1 != -2 && isA2 == -2){
		// cout << "case 4" << endl;
		int root1 = a1;

		while(IndexTable[root1] != -1)
			root1 = IndexTable[root1];

		IndexTable[a2] = root1;
	}

}

int main(int argc, char **argv){

	string INPATH(argv[1]);


	int start_volume = 0;
	int end_volume = 44;

	long long lvalue = 1;
	long long SliceSize = lvalue * DIMX * DIMY;
	unsigned int *mSlice1 = new unsigned int[SliceSize];
	unsigned int *mSlice2 = new unsigned int[SliceSize];


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
	int max_index = 0;
	for(int i =0 ;i<=VOLCOUNT*NODESIZE;++i){
		SumArr[i] = sum;
		sum += MAXArray[i];
		if (MAXArray[i] != 0){
			end_volume = i;
		}
	}
	
	//cout << max_index << endl;


	unsigned int maxValue = sum;
	// string path0 = INPATH + "v" + to_string(end_volume) + "_s99.raw";
	// cout << path0 << endl;
	// FILE *rfp = fopen(path0.c_str(), "rb");
	// fread(mSlice1, SliceSize*sizeof(unsigned int), 1, rfp);
	// fclose(rfp);

	// cout << "last file read done" << endl;

	// for(int y=0; y<DIMY;++y){
	// 	for(int x=0;x<DIMX;++x){
	// 		if(mSlice1[y*DIMX + x] > maxValue){
	// 			maxValue = mSlice1[y*DIMX + x];
	// 		}
	// 	}
	// }

	cout << "max value is " << maxValue << endl;
	IndexTable = new int[maxValue];
	memset(IndexTable, -2, sizeof(int)*maxValue);
	
	for(long long i=0;i<maxValue;++i){
		IndexTable[i] = -2;
	}

	for(int i=start_volume;i<end_volume;++i){
		string path1 = INPATH + "v" + to_string(i) + "_s99.raw";
		string path2 = INPATH + "v" + to_string(i+1) + "_s0.raw";
	
		cout << path1 << " " << path2 << endl;

		FILE *rfp1 = fopen(path1.c_str(), "rb");
		fread(mSlice1, SliceSize*sizeof(unsigned int), 1, rfp1);
		fclose(rfp1);
		
		FILE *rfp2 = fopen(path2.c_str(), "rb");
		fread(mSlice2, SliceSize*sizeof(unsigned int), 1, rfp2);
		fclose(rfp2);


		for(int y=0; y<DIMY;++y){
			for(int x=0;x<DIMX;++x){
				if(mSlice1[y*DIMX + x] != 0 && mSlice2[y*DIMX + x] != 0){

					//cout << "check tree" << endl;
					checktree(mSlice1[y*DIMX + x], mSlice2[y*DIMX + x]);
				}
			}
		}
	}


	ofstream ofs("/home/jmmoon/Projects/ConnectedComponent/shared/boundary/result_index_table.txt");
	
	for(long long i=0; i<maxValue; ++i){
		if(IndexTable[i] == -2)
			IndexTable[i] = -1;
		// for(int j=0; j<gVector.at(i).size();++j){
		// cout << i << endl;	
		ofs << IndexTable[i] << endl;
		// }
		// ofs << "\n";
	}

	ofs.flush();
	ofs.close();
	cout << "generate done" << endl;


	return 0;
}
