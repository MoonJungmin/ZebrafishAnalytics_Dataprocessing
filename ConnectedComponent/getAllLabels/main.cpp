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

using namespace std;


int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 8000;

bool fexists(const char *filename);

int main(int argc, char **argv){
	int node_number = atoi(argv[1]);
	vector<unsigned int> LabelsList;
	long long lvalue = 1;
	long long SliceSize = lvalue * DIMX * DIMY;
	unsigned int *mSlice = new unsigned int[SliceSize];

	string slicepath(PATH);

	for(int z=0;z<DIMZ;++z){
		ostringstream number_fill;	
		number_fill << setw( 5 ) << setfill( '0' ) << z;
		string path = PATH + number_fill.str() + ".raw";

		if(fexists(path.c_str())){
			ifstream img_ifs(path, ifstream::binary);
			img_ifs.read((char *)&mSlice[0], SliceSize * sizeof(unsigned int));
			img_ifs.close();


			for(int y=0;y<DIMY;++y){
				for(int x=0;x<DIMX;++x){
					unsigned int value = mSlice[y*DIMX +x];
					if(value != 0){
						LabelsList.push_back(value);
					}
				}
			}

			sort(LabelsList.begin(), LabelsList.end());
			LabelsList.erase(unique(LabelsList.begin(), LabelsList.end()), LabelsList.end());

		}
		cout << z << endl;
	}	
	string wpath = "/home/jmmoon/Projects/ConnectedComponent/shared/labels_" + to_string(node_number) + ".dat";
	ofstream ofs(wpath);
	vector<unsigned int>::iterator iter=LabelsList.begin();

	for (iter = LabelsList.begin(); iter != LabelsList.end(); ++iter){
        ofs << *iter << endl;
    }
	return 0;
}

bool fexists(const char *filename) {
	ifstream ifile(filename);
	return (bool)ifile;
}
