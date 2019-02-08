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
#include <string.h>
#include <iomanip>
using namespace std;


string SavePath = "/scratch/jmmoon/Cell/Ganglia/Block/";

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


unsigned int *temp_block_slice;
unsigned int *sub_volume;

int main(int argc, char **argv){

	int node_number = atoi(argv[1]);

	int x_count_lv0;
	int y_count_lv0;
	int z_count_lv0;

	CalcXYZCount(&x_count_lv0, &y_count_lv0, &z_count_lv0, 0, whole_volume_x, whole_volume_y, whole_volume_z, sub_volume_size);

	long long int lvalue = 1;
	long long int block_size = lvalue * sub_volume_size * sub_volume_size *sub_volume_size;
	long long int tile_size = lvalue * sub_volume_size * sub_volume_size;
	

	// generate yz 
	for(int k = 0; k < sub_volume_size; ++k){
		int width = whole_volume_z;
		int height = whole_volume_y;
		unsigned int *image = new unsigned int[whole_volume_z * whole_volume_y];
		
		for (int y = 0; y < y_count_lv0; ++y){
			for(int z = 0; z < z_count_lv0; ++z){
				string path = SavePath + "YZ/" + to_string(z) + "_" + to_string(y) + "_" + to_string(node_number) + "_0.block";
				FILE *blck_rfp = fopen(path.c_str(), "rb");
				unsigned int *tile = new unsigned int[tile_size];
				long long int offset = lvalue * tile_size * sizeof(unsigned int) * k;
				fseek(blck_rfp, offset, SEEK_SET);
				fread(tile, tile_size * sizeof(unsigned int), 1, blck_rfp);
				fclose(blck_rfp);

				for (int i = 0; i < sub_volume_size; ++i) {
					for (int j = 0; j < sub_volume_size; ++j) {
						long long int dst_offset = lvalue * ( sub_volume_size * y + i) * width + lvalue * ( sub_volume_size * z + j);
						image[dst_offset] = tile[sub_volume_size*i + j];
					} 
				}

				delete[] tile;
			}	
		}

		for (int lv = 1; lv <= sub_volume_level; ++lv) {
			int size = width / pow(2, lv) * height / pow(2, lv);
			unsigned int *resize_image = new unsigned int[size];
			rescale(image, resize_image, pow(2, lv), width, height);


			int x_count;
			int y_count;
			int z_count;
			CalcXYZCount(&x_count, &y_count, &z_count, lv, whole_volume_x, whole_volume_y, whole_volume_z, sub_volume_size);


			for (int y = 0; y < y_count; ++y) {
				for (int z = 0; z < z_count; ++z) {
					unsigned int *croppedYZ = new unsigned int[sub_volume_size * sub_volume_size];

					crop(resize_image, croppedYZ, z*sub_volume_size, y*sub_volume_size, sub_volume_size, sub_volume_size, width / pow(2, lv), height / pow(2, lv));

					//XY write step
					string tile_pathYZ = SavePath + "YZ/" + to_string(z) + "_" + to_string(y) + "_" + to_string(node_number) + "_" + to_string(lv) + ".block";
					FILE *yz_tile_fp = fopen(tile_pathYZ.c_str(), "ab");
					fwrite(croppedYZ, sizeof(unsigned int)*sub_volume_size*sub_volume_size, 1, yz_tile_fp);
					fclose(yz_tile_fp);

					delete[] croppedYZ;
				}
			}
			delete[] resize_image;
		}

		delete[] image;
	}


	for(int k = 0; k < sub_volume_size; ++k){
		int width = whole_volume_x;
		int height = whole_volume_z;
		unsigned int *image = new unsigned int[whole_volume_x * whole_volume_z];
		memset(image, 0, sizeof(unsigned int) * width * height);
		for (int z = 0; z < z_count_lv0; ++z){
			for(int x = 0; x < x_count_lv0; ++x){
				string path = SavePath + "ZX/" + to_string(z) + "_" + to_string(node_number) + "_" + to_string(x) + "_0.block";
				cout << path << endl;
				if(fexists(path.c_str())){
					FILE *blck_rfp = fopen(path.c_str(), "rb");
					unsigned int *tile = new unsigned int[tile_size];
					long long int offset = lvalue * tile_size * sizeof(unsigned int) * k;
					fseek(blck_rfp, offset, SEEK_SET);
					fread(tile, tile_size * sizeof(unsigned int), 1, blck_rfp);
					fclose(blck_rfp);

					for (int i = 0; i < sub_volume_size; ++i) {
						for (int j = 0; j < sub_volume_size; ++j) {
							long long int dst_offset = lvalue * ( sub_volume_size * z + i) * whole_volume_x + lvalue * ( sub_volume_size * x + j);
							image[dst_offset] = tile[i*sub_volume_size + j];
						}
					}
					delete[] tile;
				}
			}											
		}

		for (int lv = 1; lv <= sub_volume_level; ++lv) {
			int size = width / pow(2, lv) * height / pow(2, lv);
			unsigned int *resize_image = new unsigned int[size];
			rescale(image, resize_image, pow(2, lv), width, height);


			int x_count;
			int y_count;
			int z_count;
			CalcXYZCount(&x_count, &y_count, &z_count, lv, whole_volume_x, whole_volume_y, whole_volume_z, sub_volume_size);


			for (int z = 0; z < z_count; ++z) {
				for (int x = 0; x < x_count; ++x) {
					unsigned int *croppedZX = new unsigned int[sub_volume_size * sub_volume_size];

					crop(resize_image, croppedZX, x*sub_volume_size, z*sub_volume_size, sub_volume_size, sub_volume_size, width / pow(2, lv), height / pow(2, lv));

					//XY write step
					string tile_pathZX = SavePath + "ZX/" + to_string(z) + "_" + to_string(node_number) + "_" + to_string(x) + "_" + to_string(lv) + ".block";
					FILE *zx_tile_fp = fopen(tile_pathZX.c_str(), "ab");
					fwrite(croppedZX, sizeof(unsigned int)*sub_volume_size*sub_volume_size, 1, zx_tile_fp);
					fclose(zx_tile_fp);

					delete[] croppedZX;
				}
			}
			delete[] resize_image;
		}

		delete[] image;
	}


	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////


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
