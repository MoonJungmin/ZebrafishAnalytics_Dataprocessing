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

#include<sstream>


#define PATH "/scratch/jmmoon/connectedcomponent_label/"
#define OPATH "/scratch/jmmoon/connectedcomponent_clean/"
using namespace std;

 
int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 8000;

bool fexists(const char *filename);

int main(int argc, char **argv){
	vector<unsigned int> LabelsList;
	long long lvalue = 1;
	long long SliceSize = lvalue * DIMX * DIMY;
	unsigned int *mSlice = new unsigned int[SliceSize];


	for(int i=0; i<16;++i){
		string lpath = "/home/jmmoon/Projects/ConnectedComponent/shared/labels_" + to_string(i) + ".dat";
		ifstream lifs(lpath);
		unsigned int number;
		while (lifs >> number)
		{
			if(number > 0){
				cout << number << endl;
				LabelsList.push_back(number);
			}
		}
	}

	sort(LabelsList.begin(), LabelsList.end());
	LabelsList.erase(unique(LabelsList.begin(), LabelsList.end()), LabelsList.end());

	unsigned int Max = LabelsList.back();
	unsigned int *LabelsArr = new unsigned int[Max];
	memset(LabelsArr, 0, sizeof(unsigned int)* Max);

	vector<unsigned int>::iterator iter=LabelsList.begin();

	unsigned int index = 1;
	for (iter = LabelsList.begin(); iter != LabelsList.end(); ++iter){
		LabelsArr[*iter] = index++;
	}

	cout << index << endl;

	string slicepath(PATH);

	for(int z=0;z<DIMZ;++z){
		ostringstream number_fill;	
		number_fill << setw( 5 ) << setfill( '0' ) << z;
		string rpath = PATH + number_fill.str() + ".raw";
		string opath = OPATH + number_fill.str() + ".raw";

		if(fexists(rpath.c_str())){
			ifstream img_ifs(rpath, ifstream::binary);
			img_ifs.read((char *)&mSlice[0], SliceSize * sizeof(unsigned int));
			img_ifs.close();

			for(int y=0;y<DIMY;++y){
				for(int x=0;x<DIMX;++x){
					unsigned int value = mSlice[y*DIMX +x];
					if(value != 0){
						if(LabelsArr[value] == 0){
							cout << "Error" << endl;
							return 0;
						}
						mSlice[y*DIMX +x] = LabelsArr[value];
					}
				}
			}

			FILE *wfp = fopen(opath.c_str(), "wb");
			fwrite(mSlice, SliceSize*sizeof(unsigned int), 1, wfp);
			fclose(wfp);
		}

		cout << z << endl;
	}	

}

bool fexists(const char *filename) {
	ifstream ifile(filename);
	return (bool)ifile;
}
