#include<sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <omp.h>

#include <iomanip>
using namespace std;

void setVolumeSize(int ax, int ay, int az);
void setPathInfo(string save_path, string data_type, string input_path);

string InputPath = "/scratch/jmmoon/Cell/Ganglia/raw_file/";
string SavePath = "/scratch/jmmoon/Cell/Ganglia/Block/";


int whole_volume_x = 5120;
int whole_volume_y = 4608;
int whole_volume_z = 8192;
int sub_volume_size = 512;
int sub_volume_level = 4;

int node_number;


void rescale(unsigned int *src, unsigned int *dst, int factor, int width, int height);
void crop(unsigned int *src, unsigned int *dst, int x, int y, int rect_width, int rect_height, int src_width, int src_height);
void CalcXYZCount(int *x, int *y, int *z, int level, int x_size, int y_size, int z_size, int blocksize);
bool fexists(const char *filename);

unsigned int *temp_block_slice;
unsigned int *sub_volume;

int main(int argc, char **argv){

	omp_set_num_threads(1);

	#pragma omp parallel
	{
		cout << "start"<< endl;

		int threadnum = omp_get_thread_num();
		int threadsize = omp_get_num_threads();
		for (int z = threadnum; z < whole_volume_z; z += threadsize) {
			ostringstream number_fill;
			number_fill << setw( 5 ) << setfill( '0' ) << z;
			string path = InputPath + number_fill.str() + ".raw";
			if(fexists(path.c_str())){
				int width = whole_volume_x;
				int height = whole_volume_y;
				unsigned int *image = new unsigned int[width * height];
				ifstream img_ifs(path, ifstream::binary);
				img_ifs.read((char *)&image[0], width*height * sizeof(unsigned int));


				for (int lv = 0; lv <= sub_volume_level; ++lv) {
					int size = width / pow(2, lv) * height / pow(2, lv);
					unsigned int *resize_image = new unsigned int[size];
					rescale(image, resize_image, pow(2, lv), width, height);


					int x_count;
					int y_count;
					int z_count;
					CalcXYZCount(&x_count, &y_count, &z_count, lv, whole_volume_x, whole_volume_y, whole_volume_z, sub_volume_size);


					for (int y = 0; y < y_count; ++y) {
						for (int x = 0; x < x_count; ++x) {
							unsigned int *croppedXY = new unsigned int[sub_volume_size * sub_volume_size];

							crop(resize_image, croppedXY, x*sub_volume_size, y*sub_volume_size, sub_volume_size, sub_volume_size, width / pow(2, lv), height / pow(2, lv));

							//XY write step
							string tile_pathXY = SavePath + "XY/" + to_string((int)(z/512)) + "_" + to_string(y) + "_" + to_string(x) + "_" + to_string(lv) + ".block";
							FILE *xy_tile_fp = fopen(tile_pathXY.c_str(), "ab");
							fwrite(croppedXY, sizeof(unsigned int)*sub_volume_size*sub_volume_size, 1, xy_tile_fp);
							fclose(xy_tile_fp);

							delete[] croppedXY;
						}
					}
					delete[] resize_image;
				}
				delete[] image;

			}

		}	
	}


	return 0;
}

bool fexists(const char *filename) {
  ifstream ifile(filename);
  return (bool)ifile;
}

void CalcXYZCount(int *x, int *y, int *z, int level, int x_size, int y_size, int z_size, int blocksize) {
	//level -= 1;
	int size_x = x_size / pow(2,level);
	int size_y = y_size / pow(2,level);
	int size_z = z_size / pow(2,level);

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