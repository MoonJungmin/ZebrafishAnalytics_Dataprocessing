#include "feature_voxel.h"


// using namespace pcl;
using namespace std;
using Eigen::MatrixXd;





string feature_voxel::getEMPath(int em_number){
    string path(EM_PATH);
    ostringstream em_number_fill;
    em_number_fill << setw( 5 ) << setfill( '0' ) << em_number;
    string EM_Image_path;
    EM_Image_path = path + em_number_fill.str() + ".png";
    cout << EM_Image_path << endl;
    return EM_Image_path;
}


feature_voxel::feature_voxel(){

}

unsigned int feature_voxel::getBitbyCoord(long long x, long long y, long long z, unsigned int *target, cell_info a){
	// int DIMX = 10240;
	// int DIMY = 9216;
	int w = a.maxbox.x - a.minbox.x;
	int h = a.maxbox.y - a.minbox.y;
	int d = a.maxbox.z - a.minbox.z;

	if(y>=0 && x>=0 && z>=0 && x<w && y<h && z<d){
		long long bitPos = (z*h*w) + (y*w) + x;
		unsigned int b = target[bitPos];
		if(a.index == b){
			return 1;	
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
	

}



feature_set feature_voxel::getFeature(cell_info cell){
	int DIMX = 5120;
	int DIMY = 4608;
	int DIMZ = 8000;


	feature_set result;

	result.volume = 0;
	result.surfacearea = 0;
	result.sphericity = 0;
	result.eccentricity = 0;
	result.intensity = 0;

	vector<coord> cell_points;
	
	long long lvalue = 1;
	string CELL_PATHSTR(CELL_PATH);
	string EM_PATHSTR(EM_PATH);

//	cout << "Make Cell Block" << endl;

	int cell_width = (cell.maxbox.x - cell.minbox.x);
	int cell_height = (cell.maxbox.y - cell.minbox.y);
	int cell_depth = (cell.maxbox.z - cell.minbox.z);

	long long slice_size = lvalue * cell_width * cell_height;
	long long block_size = lvalue * cell_width * cell_height * cell_depth;
	unsigned int *CellBlock = new unsigned int[block_size];	
	unsigned char *EMBlock = new unsigned char[block_size];	

	cout <<"alloc done : " << cell_width << "x" << cell_height << "x" << cell_depth << endl;

	for(int z=cell.minbox.z;z<cell.maxbox.z;++z){
		ostringstream number_fill;	
		number_fill << setw( 5 ) << setfill( '0' ) << z;
		string cell_path = CELL_PATHSTR + number_fill.str() + ".raw";
		string em_path = EM_PATHSTR + number_fill.str() + ".raw";


		FILE *rfp = fopen(cell_path.c_str(), "rb");
		long long slice_size = lvalue * cell_width * cell_height;
		for(int y=cell.minbox.y;y<cell.maxbox.y;++y){
			fseek(rfp, (lvalue*DIMX*y + cell.minbox.x)*sizeof(unsigned int), SEEK_SET);
			fread(&CellBlock[(z-cell.minbox.z)*slice_size + (y-cell.minbox.y)*cell_width], cell_width*sizeof(unsigned int), 1, rfp);	
		}
		fclose(rfp);

		FILE *rfp2 = fopen(em_path.c_str(), "rb");
		for(int y=cell.minbox.y;y<cell.maxbox.y;++y){
			fseek(rfp2, (lvalue*DIMX*y + cell.minbox.x)*sizeof(unsigned char), SEEK_SET);
			fread(&EMBlock[(z-cell.minbox.z)*slice_size + (y-cell.minbox.y)*cell_width], cell_width*sizeof(unsigned char), 1, rfp2);	
		}
		fclose(rfp2);


		// Mat em_image;
  //   	em_image = imread(em_path, 0);
		// for(int y=cell.minbox.y;y<cell.maxbox.y;++y){
		// 	for(int x=cell.minbox.x;x<cell.maxbox.x;++x){
		// 		EMBlock[(z-cell.minbox.z)*slice_size + (y-cell.minbox.y)*cell_width + (x-cell.minbox.x)] = em_image.at<uchar>(y, x);
		// 	}
		// }
	}

//	cout <<"read done " << endl;

	long long int Intensity_Sum = 0;
	for(int z=cell.minbox.z;z<cell.maxbox.z;++z){
		for(int y=cell.minbox.y;y<cell.maxbox.y;++y){
			for(int x=cell.minbox.x;x<cell.maxbox.x;++x){				
				int local_x = x - cell.minbox.x;
				int local_y = y - cell.minbox.y;
				int local_z = z - cell.minbox.z;

				long long pos = local_z*slice_size + local_y*cell_width + local_x;
				unsigned int cell_value = 0;
				int em_value = 6;

				if(pos >= 0 && pos < block_size){
					cell_value = CellBlock[pos];
					em_value = EMBlock[pos];
				}
				 
				
				if(cell_value == cell.index){
					//cout << "test - " ;
					Intensity_Sum += em_value;

					coord temp;
					temp.x = local_x;
					temp.y = local_y;
					temp.z = local_z;
					cell_points.push_back(temp);
					result.volume ++;

					int nv[8];
					nv[0] = (int)getBitbyCoord(local_x-1, local_y-1, local_z, CellBlock, cell);
					nv[1] = (int)getBitbyCoord(local_x-1, local_y, local_z, CellBlock, cell);
					nv[2] = (int)getBitbyCoord(local_x-1, local_y+1, local_z, CellBlock, cell);
					nv[3] = (int)getBitbyCoord(local_x, local_y-1, local_z, CellBlock, cell);
					nv[4] = (int)getBitbyCoord(local_x, local_y+1, local_z, CellBlock, cell);
					nv[5] = (int)getBitbyCoord(local_x+1, local_y-1, local_z, CellBlock, cell);
					nv[6] = (int)getBitbyCoord(local_x+1, local_y, local_z, CellBlock, cell);
					nv[7] = (int)getBitbyCoord(local_x+1, local_y+1, local_z, CellBlock, cell);
					
					int sum = 0;
					for(int j=0;j<8;++j){
						sum += nv[j];
					}
					if(sum != 8)
						result.surfacearea ++;

					//cout << "done" << endl;

				}
			}
		} 
	}
	//cout <<"process done " << endl;

	delete[] CellBlock;
	delete[] EMBlock;


	result.sphericity = getSphericity(result.volume, result.surfacearea);
	result.eccentricity = getEccentricity(cell_points);
	result.intensity = (long double)Intensity_Sum / (long double)result.volume;

	//cout << "volume : " << result.volume << endl; 
	//cout << "surfacearea : " << result.surfacearea << endl;
	//cout << "sphericity : " << result.sphericity << endl;
	//cout << "eccentricity : " << result.eccentricity << endl;
	//cout << "intensity : " << result.intensity << endl;

				// long long fpos = lvalue * (z*DIMX*DIMY + y*DIMX + x);


	// 			unsigned int value = CellSlice[local_y*cell_width + local_x];

	// 	 		// cout << value << endl;
	// 			if(value == cell.index){
	// 		    	cout << value << " " << cell.index << endl;
	// 		    	cout << em_image.rows << " " << em_image.cols << " " << em_image.channels() << endl;
	// 				int em_voxel = (int)em_image.at<uchar>(y, x);


	// 				// em_sum += (int)em_voxel;
	// 				cout << em_voxel << endl;
	// 				// coord temp;
	// 				// temp.x = x;
	// 				// temp.y = y;
	// 				// temp.z = z;
	// 				// cell_points.push_back(temp);
	// 				// result.volume ++;

	// 				// int nv[8];
	// 				// nv[0] = (int)getBitbyCoord(x-1, y-1, z, cell_bit_volume, mCell);
	// 				// nv[1] = (int)getBitbyCoord(x-1, y, z, cell_bit_volume, mCell);
	// 				// nv[2] = (int)getBitbyCoord(x-1, y+1, z, cell_bit_volume, mCell);
	// 				// nv[3] = (int)getBitbyCoord(x, y-1, z, cell_bit_volume, mCell);
	// 				// nv[4] = (int)getBitbyCoord(x, y+1, z, cell_bit_volume, mCell);
	// 				// nv[5] = (int)getBitbyCoord(x+1, y-1, z, cell_bit_volume, mCell);
	// 				// nv[6] = (int)getBitbyCoord(x+1, y, z, cell_bit_volume, mCell);
	// 				// nv[7] = (int)getBitbyCoord(x+1, y+1, z, cell_bit_volume, mCell);
					

	// 				// int sum = 0;
	// 				// for(int j=0;j<8;++j){
	// 				// 	sum += nv[j];
	// 				// // cout << nv[j] << " ";
	// 				// }
	// 		  //   	// cout << endl;
	//     // 			if(sum != 8)
	// 	   //  			result.surfacearea ++;
	// 	    	}
	// 			// cell_ifs.seekg(i*2560*2304*4+y*2560*4+x*4, ios::beg);
	// 		}
	// 	}

	// }

	return result;


	// for(int z=0; z<d ; z++){
	// 	int v = z/64;
	// 	int s = z%64;

	// 	string file_path = "";
	// 	if(v < 116){
	// 		string p(CELL_PATH1);
	// 		file_path += p;
	// 	}
	// 	else{
	// 		string p(CELL_PATH2);
	// 		file_path += p;
	// 	}
	// 	file_path += "v" + to_string(v) + "_s" + to_string(s) + ".raw";
	// 	cout << file_path << endl;
	// 	ifstream cell_ifs;
	// 	cell_ifs.open(cell_path, ios::binary);
	// 	unsigned int *mSlice1 = new unsigned int[lvsize];
	// 	long long seek_size = lvalue * DIMX * mCell.minbox.y
	// 	seekg(DIMX*)
	// 	cell_ifs.read(reinterpret_cast<char*>(&mSlice1[0]), SliceSize*4);


	// 	cout << z << endl;
	// 	for(int y=0;y<h;++y){
	// 		for(int x=0;x<w;++x){				
	// 			// long long fpos = lvalue * (z*DIMX*DIMY + y*DIMX + x);
	// 		 	int value = (int)getBitbyCoord(x, y, z, cell_bit_volume, mCell);

	// 	 		// cout << value << endl;
	// 			if(value == 1){
	// 		    	// cout << value << " " << mCell.index << endl;
	// 		    long long empos = lvalue * ((z+mCell.minbox.z)*DIMY*DIMX + (y+mCell.minbox.y)*DIMX + (x+mCell.minbox.x));
	// 				unsigned char em_voxel = readChar(&em_ifs, empos);
	// 				em_sum += (int)em_voxel;
	// 				// cout << (int)em_voxel << endl;
	// 				coord temp;
	// 				temp.x = x;
	// 				temp.y = y;
	// 				temp.z = z;
	// 				cell_points.push_back(temp);
	// 				result.volume ++;

	// 				int nv[8];
	// 				nv[0] = (int)getBitbyCoord(x-1, y-1, z, cell_bit_volume, mCell);
	// 				nv[1] = (int)getBitbyCoord(x-1, y, z, cell_bit_volume, mCell);
	// 				nv[2] = (int)getBitbyCoord(x-1, y+1, z, cell_bit_volume, mCell);
	// 				nv[3] = (int)getBitbyCoord(x, y-1, z, cell_bit_volume, mCell);
	// 				nv[4] = (int)getBitbyCoord(x, y+1, z, cell_bit_volume, mCell);
	// 				nv[5] = (int)getBitbyCoord(x+1, y-1, z, cell_bit_volume, mCell);
	// 				nv[6] = (int)getBitbyCoord(x+1, y, z, cell_bit_volume, mCell);
	// 				nv[7] = (int)getBitbyCoord(x+1, y+1, z, cell_bit_volume, mCell);
					

	// 				int sum = 0;
	// 				for(int j=0;j<8;++j){
	// 					sum += nv[j];
	// 				// cout << nv[j] << " ";
	// 				}
	// 		    	// cout << endl;
	//     		if(sum != 8)
	// 	    		result.surfacearea ++;
	// 	    }
	// 			// cell_ifs.seekg(i*2560*2304*4+y*2560*4+x*4, ios::beg);
	// 		}

	// 	}
		
	// }
	// // cout << "voxel count done" << endl;
	// delete cell_bit_volume;
	// cell_ifs.close();
	// em_ifs.close();
	// result.sphericity = getSphericity(result.volume, result.surfacearea);
	// result.eccentricity = getEccentricity(cell_points);
	// result.intensity = (long double)em_sum / (long double)result.volume;

	// // cout << "volume : " << result.volume << endl; 
	// // cout << "surfacearea : " << result.surfacearea << endl;
	// // cout << "sphericity : " << result.sphericity << endl;
	// // cout << "eccentricity : " << result.eccentricity << endl;
	// // cout << "intensity : " << result.intensity << endl;

	// return result;
}


float feature_voxel::getEccentricity(vector<coord> points) 
{
	if(points.size()<4)
	{
		return 0;
	}
	float covarianceMatrix[3][3];
	double means[3] = {0, 0, 0};
	for (int i = 0; i < points.size(); i++)
	{
		means[0] += points[i].x,
		means[1] += points[i].y,
		means[2] += points[i].z;
	}
	means[0] /= points.size(), means[1] /= points.size(), means[2] /= points.size();
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) 
		{
			covarianceMatrix[i][j] = 0.0;
			for (int k = 0; k < points.size(); k++)
			{
				if(i==0 &&j==0)
					covarianceMatrix[i][j] += (means[i] - points[k].x) * (means[j] - points[k].x);
				else if(i==0 &&j==1)
					covarianceMatrix[i][j] += (means[i] - points[k].x) * (means[j] - points[k].y);
				else if(i==0 &&j==2)
					covarianceMatrix[i][j] += (means[i] - points[k].x) * (means[j] - points[k].z);
				else if(i==1 &&j==0)
					covarianceMatrix[i][j] += (means[i] - points[k].y) * (means[j] - points[k].x);
				else if(i==1 &&j==1)
					covarianceMatrix[i][j] += (means[i] - points[k].y) * (means[j] - points[k].y);
				else if(i==1 &&j==2)
					covarianceMatrix[i][j] += (means[i] - points[k].y) * (means[j] - points[k].z);
				else if(i==2 &&j==0)
					covarianceMatrix[i][j] += (means[i] - points[k].z) * (means[j] - points[k].x);
				else if(i==2 &&j==1)
					covarianceMatrix[i][j] += (means[i] - points[k].z) * (means[j] - points[k].y);
				else
					covarianceMatrix[i][j] += (means[i] - points[k].z) * (means[j] - points[k].z);
			}
			covarianceMatrix[i][j] /= points.size() - 1;
		}
	}	
	MatrixXd Mat(3,3);
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			Mat(i,j)=covarianceMatrix[i][j];
		}
	}
	Eigen::EigenSolver<MatrixXd> es(Mat);
	vector<float> eigenvals;
	float sum=0;
	for(int i=0; i<3; i++)
	{
		eigenvals.push_back(es.eigenvalues()[i].real());
		sum+=eigenvals.at(i);

	}
	sort(eigenvals.begin(),eigenvals.end(), greater<int>());


	//cout << eigenvals[0] << " //// " <<  eigenvals[1] << " //// " <<  eigenvals[2] <<endl;
//	return (3*((eigenvals[1]/sum)+(eigenvals[2]/sum)))/2;
	return sqrt(1-((eigenvals[2]*eigenvals[2]) / (eigenvals[0]*eigenvals[0])));
}



float feature_voxel::getSphericity(int volume, int surface)
{
	if(surface==0)
		return 0;
	float As, radius;
	radius= pow((3/12.566368)*volume, 1.0/3.0);
	As=4*pow(radius,2);
	return As/surface;
}

