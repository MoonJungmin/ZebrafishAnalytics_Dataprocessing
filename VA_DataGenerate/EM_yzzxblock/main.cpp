#include<sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>

#include <iomanip>
using namespace std;


string InputPath = "";
string SavePath1 = "";
string SavePath2 = "";

int whole_volume_x = 5120;
int whole_volume_y = 4608;
int whole_volume_z = 8192;
int sub_volume_size = 512;
int sub_volume_level = 4;

void rescale(unsigned int *src, unsigned int *dst, int factor, int width, int height);
void crop(unsigned int *src, unsigned int *dst, int x, int y, int rect_width, int rect_height, int src_width, int src_height);
void CalcXYZCount(int *x, int *y, int *z, int level, int x_size, int y_size, int z_size, int blocksize);
bool fexists(const char *filename);
vector<string> Split(const char *phrase, string delimiter);
vector<string> getfilelist(string root, string type);
void Rotate(string filename);

unsigned int *temp_block_slice;
unsigned int *sub_volume;

int main(int argc, char **argv){
	InputPath += argv[1];
	SavePath1 += argv[2];
	SavePath2 += argv[3];

	cout << InputPath << endl;
	cout << SavePath1 << endl;
	cout << SavePath2 << endl;

	//return 0;
	vector<string> filelist = getfilelist(InputPath, "block");
	for(int i=0;i<filelist.size();++i){
		cout << filelist[i] << endl;
		Rotate(filelist[i]);
	}
	
	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////
void Rotate(string filename){

	int x_count_lv0;
	int y_count_lv0;
	int z_count_lv0;

	CalcXYZCount(&x_count_lv0, &y_count_lv0, &z_count_lv0, 0, whole_volume_x, whole_volume_y, whole_volume_z, sub_volume_size);

	long long int lvalue = 1;
	long long int block_size = lvalue * sub_volume_size * sub_volume_size *sub_volume_size;
	long long int tile_size = lvalue * sub_volume_size * sub_volume_size;
	

	string inputpath = InputPath + filename;
	string savepath_yz = SavePath1 + filename;
	string savepath_zx = SavePath2 + filename;
	FILE *blck_rfp = fopen(inputpath.c_str(), "rb");
	

	FILE *blck_wfp_yz = fopen(savepath_yz.c_str(), "wb");
	FILE *blck_wfp_zx = fopen(savepath_zx.c_str(), "wb");

	unsigned char *blockxy = new unsigned char[block_size];
	unsigned char *blockyz = new unsigned char[block_size];
	unsigned char *blockzx = new unsigned char[block_size];

	fread(blockxy, block_size, 1, blck_rfp);



	for(int z = 0; z < sub_volume_size; ++z){ //blocky
		for (int y = 0; y < sub_volume_size; ++y){ //blockz
			for(int x = 0; x < sub_volume_size; ++x){ //blockx
				//copy and reposition the block_xy to (block_xz, block_yz)
				blockyz[x*sub_volume_size*sub_volume_size + y*sub_volume_size + z] = blockxy[z*sub_volume_size*sub_volume_size + y*sub_volume_size + x];								
				blockzx[y*sub_volume_size*sub_volume_size + z*sub_volume_size + x] = blockxy[z*sub_volume_size*sub_volume_size + y*sub_volume_size + x];
				
			}											
		}
	}

	fwrite(blockzx, block_size, 1, blck_wfp_zx);
	fwrite(blockyz, block_size, 1, blck_wfp_yz);

	fclose(blck_rfp);
	fclose(blck_wfp_yz);
	fclose(blck_wfp_zx);
}


vector<string> Split(const char *phrase, string delimiter) {
	vector<string> list;
	string s = string(phrase);
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		list.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	list.push_back(s);
	return list;
}



vector<string> getfilelist(string root, string type){
	DIR *dir;
	vector<string> filelist;
	struct dirent *ent;
	dir = opendir (root.c_str());
	if (dir != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			string aType = Split(ent->d_name, ".").back();
			if(!aType.compare(type)){
				filelist.push_back(ent->d_name);
			}
		}
		closedir (dir);
	} else {
	 	/* could not open directory */
		perror ("");
	}

	return filelist;
}


bool fexists(const char *filename) {
  ifstream ifile(filename);
  return (bool)ifile;
}

void CalcXYZCount(int *x, int *y, int *z, int level, int x_size, int y_size, int z_size, int blocksize) {
	//level -= 1;
	int size_x = x_size / pow(2,level);
	int size_y = y_size / pow(2, level);
	int size_z = z_size / pow(2, level);

	if (size_x % blocksize == 0)
		*x = size_x / blocksize;
	else
		*x = size_x / blocksize + 1;

	if (size_y % blocksize == 0)
		*y = size_y / blocksize;
	else
		*y = size_y / blocksize + 1;

	if (size_z % blocksize == 0)
		*z = size_z / blocksize;
	else
		*z = size_z / blocksize + 1;


	if (size_z < blocksize)
		*z = 1;
	if (size_y < blocksize)
		*y = 1;
	if (size_x < blocksize)
		*x = 1;

}

void rescale(unsigned int *src, unsigned int *dst, int factor, int width, int height) {
	int dst_height = height / factor;
	int dst_width = width / factor;
		
	for (int y = 0; y < dst_height; ++y) {
		for (int x = 0; x < dst_width; ++x) {
			long long dst_index = y * dst_width + x;
			long long src_index = (y * factor) * width + (x * factor);
			if (src_index < width*height && dst_index < dst_height*dst_width) {
				dst[dst_index] = src[src_index];
			}
			//dst[dst_index] = src[src_index];
		}
	}
}

void crop(unsigned int *src, unsigned int *dst, int x, int y, int rect_width, int rect_height, int src_width, int src_height) {
	
	for (int i = y; i < y+rect_height; ++i) {
		for (int j = x; j < x+rect_width; ++j) {
			long long dst_index = (i-y)*rect_width + (j-x);
			long long src_index = i * src_width + j;
			if (i >= src_height || j >= src_width) {
				dst[dst_index] = 0;
			}
			else {
				dst[dst_index] = src[src_index];
			}
			
		}
	}
}