#include "global.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>

#include <iomanip>
#include<sstream>
#include <fstream>


using namespace std;
using namespace cv;

class feature_voxel{

	public:
		vector<string> EM_Image_path;

		feature_voxel();
		// int getVolume(vector<vector<vertex>> contour_set);
		// // vector<vertex> getAllPoints(vector<vector<vertex>> contour_set);
		// int getSurfaceArea(vector<vertex> Points_set,int label);
		// float getSphericity(int volume, int surface);
		// float getEccentricity(vector<vertex> points);
		string getEMPath(int em_number);

		float getSphericity(int volume, int surface);
		feature_set getFeature(cell_info cell);
		
		float getEccentricity(vector<coord> points);
		unsigned int getBitbyCoord(long long x, long long y, long long z, unsigned int *target, cell_info a);

	// private:
	// 	void volume_add(vertex& prev_point, vertex& now_point, vertex& next_point, float& peak_sum, int &change, float & sum);
	// 	int getContourArea(vector<vertex> contour);
	// 	vector<vertex> getPoints(vector<vertex> contour);
};
