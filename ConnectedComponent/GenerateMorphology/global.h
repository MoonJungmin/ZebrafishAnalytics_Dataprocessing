#pragma once
#include <unistd.h>
#include <dirent.h>



#define CELL_PATH "/scratch/jmmoon/diamond/Cell/Brain/labeled_rawfile/"
#define EM_PATH "/scratch/jmmoon/diamond/EM/180602_median2_160515_SWiFT_120nmpx_singles_raw_link/"
#define BOX_PATH "/scratch/jmmoon/diamond/Cell/Brain/Cell_BoundingBox.dat"


struct coord
{
	long long x;
	long long y;
	long long z;
};

struct cell_info
{
	bool status = false;
	long long index;
	coord minbox;
	coord maxbox;
	coord seed;
};


struct feature_set
{
	long long volume;
	long double surfacearea;
	long double sphericity;
	long double eccentricity;	
	long double intensity;
};
