
#include "main.h"
// #include "feature.h"
// #include "datastructure.h"

#include <omp.h>



// bool operator<(const cell_info& a, const cell_info& b)
// {
// 	int w_a = a.maxbox.x - a.minbox.x;
// 	int h_a = a.maxbox.y - a.minbox.y;
// 	int d_a = a.maxbox.z - a.minbox.z;

// 	int w_b = b.maxbox.x - b.minbox.x;
// 	int h_b = b.maxbox.y - b.minbox.y;
// 	int d_b = b.maxbox.z - b.minbox.z;
//     return w_a*h_a*d_a < w_b*h_b*d_b;
// }

int main(int argc, char **argv){
	int node_number = atoi(argv[1]);


	string box_path(BOX_PATH);
	ifstream ifs(box_path);
	vector<cell_info> mBox;
	vector<cell_info> mBox2;
	long long int lvalue = 1;
	while(!ifs.eof()){
		cell_info tmp;
		ifs >> tmp.index >> tmp.minbox.x >> tmp.minbox.y >> tmp.minbox.z >> tmp.maxbox.x >> tmp.maxbox.y >> tmp.maxbox.z;// >> tmp.seed.x >> tmp.seed.y >> tmp.seed.z;

		int w = tmp.maxbox.x - tmp.minbox.x;
		int h = tmp.maxbox.y - tmp.minbox.y;
		int d = tmp.maxbox.z - tmp.minbox.z;
		long long size = lvalue * w * h * d;
		if(size > 10*1000*1000){
			mBox2.push_back(tmp);
		}
		else{
			mBox.push_back(tmp);		
		}
	}
    
    cout << mBox.size() << endl;
    cout << mBox2.size() << endl;

    

	int step_size = mBox.size() / 16;

	int _start = node_number * step_size;
	//int _start = 111120;//node_number * step_size;
	int _end = (node_number+1) * step_size;
	if(node_number == 15)
		_end = mBox.size();
	// cout << mBox.size() << endl;
	
	clock_t before;
	double  result;
	before = clock();


	string outpath("/home/jmmoon/Projects/ConnectedComponent/shared/Morphological_" + to_string(node_number) + ".dat");
	ofstream ofs(outpath);
 	
 	omp_set_num_threads(4);
	
	#pragma omp parallel
	{
		int tnum = omp_get_thread_num();
		int tsize = omp_get_num_threads();

		for(int i=_start+tnum;i<_end;i+=tsize) {
			
			
			feature_voxel mFeature;
			// feature_set feature_result;
			// feature_result.volume = 0; 
			// feature_result.surfacearea = 0;
			// feature_result.sphericity = 0;
			// feature_result.eccentricity = 0;
			// feature_result.intensity = 0;
			feature_set feature_result = mFeature.getFeature(mBox.at(i));
			#pragma omp critical(file_write)
			{
				cout << mBox.at(i).index << " " << feature_result.volume << " " << feature_result.surfacearea << " " << feature_result.sphericity << " " << feature_result.eccentricity << " " << feature_result.intensity << endl;
				ofs << mBox.at(i).index << " " << feature_result.volume << " " << feature_result.surfacearea << " " << feature_result.sphericity << " " << feature_result.eccentricity << " " << feature_result.intensity << endl;
			}
						
		}

	}
	

	for(int i=node_number;i<mBox2.size();i+=15) {
				
			feature_voxel mFeature;
			feature_set feature_result = mFeature.getFeature(mBox2.at(i));
			cout << "Big size" << endl;
			cout << mBox2.at(i).index << " " << feature_result.volume << " " << feature_result.surfacearea << " " << feature_result.sphericity << " " << feature_result.eccentricity << " " << feature_result.intensity << endl;
			ofs << mBox2.at(i).index << " " << feature_result.volume << " " << feature_result.surfacearea << " " << feature_result.sphericity << " " << feature_result.eccentricity << " " << feature_result.intensity << endl;					
	}



	// for(int i=_start;i<_end;i+=1){

	// 	cout << i << " / " << _end << endl;

	// 	//feature_voxel mFeature;
	// 	//feature_set feature_result = mFeature.getFeature(mBox.at(i));
	// 	//ofs << mBox.at(i).index << " " << feature_result.volume << " " << feature_result.surfacearea << " " << feature_result.sphericity << " " << feature_result.eccentricity << " " << feature_result.intensity << endl;				
	// }
	
	ofs.close();
	result = (double)(clock() - before) / CLOCKS_PER_SEC;
	printf("걸린시간은 %5.2f 입니다.\n", result);

	// outFile.close();
	// mysql->insertCellMorphological();



	return 0;
}
















