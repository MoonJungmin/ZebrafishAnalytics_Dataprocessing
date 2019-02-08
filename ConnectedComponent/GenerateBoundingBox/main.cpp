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

#include<sstream>


int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 8000;

#define PATH "/scratch/jmmoon/connectedcomponent_clean/"


using namespace std;


int *IndexTable;


struct coord
{
	long long x;
	long long y;
	long long z;
};

struct cell_info
{
	bool status = false;
	int index;
	coord minbox;
	coord maxbox;
	coord seed;
};

bool fexists(const char *filename) {
	ifstream ifile(filename);
	return (bool)ifile;
}


int main(int argc, char **argv){
	int node_number = atoi(argv[1]);
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
	cout << "max  : " << index << endl;

	cell_info *BBox = new cell_info[index];


	for(int i=0; i<index; ++i){
		BBox[i].minbox.x = 99999;
		BBox[i].minbox.y = 99999;
		BBox[i].minbox.z = 99999;

		BBox[i].maxbox.x = -99999;
		BBox[i].maxbox.y = -99999;
		BBox[i].maxbox.z = -99999;
	}

	cout << "generate done" << endl;

	for(int z=0;z<DIMZ;++z){
		ostringstream number_fill;	
		number_fill << setw( 5 ) << setfill( '0' ) << z;
		string rpath = PATH + number_fill.str() + ".raw";

		if(fexists(rpath.c_str())){
			cout << rpath << endl;
			ifstream ifs1;
			ifs1.open(rpath, ios::binary);
			ifs1.read(reinterpret_cast<char*>(&mSlice[0]), SliceSize*sizeof(unsigned int));
			ifs1.close();

			for(int y=0; y<DIMY;++y){
				for(int x=0;x<DIMX;++x){
					if(mSlice[y*DIMX + x] != 0){
						int id = mSlice[y*DIMX + x];
						coord c;
						c.x = x;
						c.y = y;
						c.z = z;

						BBox[id].index = id;
						BBox[id].status = true;


						if(BBox[id].minbox.x >= c.x) BBox[id].minbox.x = c.x;
						if(BBox[id].minbox.y >= c.y) BBox[id].minbox.y = c.y;
						if(BBox[id].minbox.z >= c.z) BBox[id].minbox.z = c.z;
	 
						if(BBox[id].maxbox.x <= c.x) BBox[id].maxbox.x = c.x;
						if(BBox[id].maxbox.y <= c.y) BBox[id].maxbox.y = c.y;
						if(BBox[id].maxbox.z <= c.z) BBox[id].maxbox.z = c.z;

						BBox[id].seed.x = c.x;
						BBox[id].seed.y = c.y;
						BBox[id].seed.z = c.z;

					}
				}
			}

			
		}
	
	}

	string wpath = "/home/jmmoon/Projects/ConnectedComponent/shared/BoundingBox_" + to_string(node_number) + ".dat";
	ofstream ofs(wpath);
	
	for(int i=0; i<index; ++i){
		int id = i;
		if(BBox[id].status){
			string tmp = to_string(BBox[id].index) + " " + to_string(BBox[id].minbox.x) + " " + to_string(BBox[id].minbox.y) + " " + to_string(BBox[id].minbox.z);
			tmp += " " + to_string(BBox[id].maxbox.x) + " " + to_string(BBox[id].maxbox.y) + " " + to_string(BBox[id].maxbox.z);
			tmp += " " + to_string(BBox[id].seed.x) + " " + to_string(BBox[id].seed.y) + " " + to_string(BBox[id].seed.z);
			ofs << tmp << endl;			
		}
	}


	return 0;
}
