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
#include <dirent.h>
#include <iomanip>
#include <sstream>


// #include <png.h>

#define SEGPATH "/scratch/jmmoon/Cell/Ganglia/Ganglia_binary_file/"
// #define DIMX 2560
// #define DIMY 2304
// #define DIMZ 3648


// #define DIMX 10240
// #define DIMY 9216
// #define DIMZ 500
// #define DIMZ 14936
// #define DIMZ 1400

#define THREAD 8

using namespace std;


ifstream gIfs;

int Whole_VolumeSize = 80;
int Whole_NodeSize = 16;

unsigned int *VisitTable;
unsigned char *SegResultVolume;


int DIMX = 5120;
int DIMY = 4608;
int DIMZ = 100;

struct coord
{
	long long x;
	long long y;
	long long z;
};

struct cell_info
{
	long long id;
	
	coord minbox;
	coord maxbox;
	coord seed;
	long long count;
	// vector<coord> innerpnt;
};

struct image_element
{
	int z_index;
	unsigned char* image;
};


string ToDigitStr(int value,int digitsCount)
{
    ostringstream os;
    os<<setfill('0')<<setw(digitsCount)<<value;
    return os.str();
}

unsigned char getBitbyCoord(long long x, long long y, long long z, unsigned char *target){
	if(y<0) y=0;
	if(x<0) x=0;
	if(z<0) z=0;

	long long bitPos = (z*DIMY*DIMX) + (y*DIMX) + x;
	unsigned char b = target[bitPos];
	//int count = x%8;
	//return (b<<count)&128;
	return b;
}

void setBitToZero(coord c, unsigned char* target){
	// long long bitPos = (c.z*DIMX*DIMY/8) + (c.y*DIMX/8) + c.x/8;
	// int count = 7 - c.x%8;
	// target[bitPos] &= ~(0x01<<count);

	long long bitPos = (c.z*DIMX*DIMY) + (c.y*DIMX) + c.x;
	target[bitPos] = 0;
}

int checkZIndex(vector<image_element> *src, int z_index){

	for(int i=0;i<src->size();++i){
		if(src->at(i).z_index == z_index){
		 	return i;
		}
	}
	return -1;
}


void checkQueue(queue<coord> *mq, coord c, int cell_index){
	if(c.z >= 0 && c.y >= 0 && c.x >= 0 && c.z < DIMZ && c.y < DIMY && c.x < DIMX){
		

		if(getBitbyCoord(c.x, c.y, c.z, SegResultVolume) != 0){
			long long lvalue = 1;
			long long vpos = (lvalue * DIMX * DIMY * c.z) + (lvalue * DIMX * c.y) + c.x;
			// #pragma omp critical(push)
			// {
				mq->push(c);
				setBitToZero(c, SegResultVolume);
				// setBitToOne(c, VisitTable);
				
				// if(mCell->minbox.x > c.x) mCell->minbox.x = c.x;
				// if(mCell->minbox.y > c.y) mCell->minbox.y = c.y;
				// if(mCell->minbox.z > c.z) mCell->minbox.z = c.z;

				// if(mCell->maxbox.x < c.x) mCell->maxbox.x = c.x;
				// if(mCell->maxbox.y < c.y) mCell->maxbox.y = c.y;
				// if(mCell->maxbox.z < c.z) mCell->maxbox.z = c.z;

				// mCell->seed.x += c.x;
				// mCell->seed.y += c.y;
				// mCell->seed.z += c.z;

				// mCell->count++;
				VisitTable[vpos] = cell_index;
			// }
		}
	}
}


void checkJob(long long x, long long y, long long z, int cell_index){
	
	
	queue<coord> mQueue;

	coord seed;
	seed.x = x;
	seed.y = y;
	seed.z = z;
	mQueue.push(seed);
	
	queue<coord> *LocalQueue = new queue<coord>[THREAD];

	while(!mQueue.empty()){
	
		
		int thread_num = 0;
		// for(int qi=0; qi<mQueueSize; ++qi){
		int QueueSize = mQueue.size();

		// cout << "Distribute Queue" << endl;
		while(!mQueue.empty()){
			LocalQueue[thread_num].push(mQueue.front());
			mQueue.pop();
			if(LocalQueue[thread_num].size() > QueueSize/THREAD){
				thread_num++;
			}	
		}
		// cout << "Distribute Queue Done" << endl;

		bool isEmpty = false;

		#pragma omp parallel shared(mQueue, SegResultVolume, isEmpty)
		{
			
			while(LocalQueue[omp_get_thread_num()].size() > 0 && isEmpty == false){
				coord it;
				// bool jobflag = false;
				// #pragma omp critical(push)
				// {
				// if(mQueue.size() > 0){
					// Q.front()
				it = LocalQueue[omp_get_thread_num()].front();
				LocalQueue[omp_get_thread_num()].pop();
					// cout << mQueue.size() << " ";
					// mQueue.erase(mQueue.begin());
					// jobflag = true;
				// }
					// else{
					// 	;//cout << "empty" << endl;
					// }
				// }
				
				if(omp_get_thread_num() == 0 && LocalQueue[omp_get_thread_num()].size()%10 == 0){
					//cout << LocalQueue[omp_get_thread_num()].size() << " " << it.z << endl;
					int mz = (int)it.z;
					printf("CellID : %d  //  Queue size : %d   //   Current Z : %d \n",cell_index, LocalQueue[omp_get_thread_num()].size(), mz);
				}

				// if(it.z > DIMZ){
				// 	printf("Error!\n");
				// }
				// if(omp_get_thread_num() == 0){
				// 	for(int i=0;i<THREAD;++i){
				// 		cout << LocalQueue[omp_get_thread_num()].size() << " ";
				// 	}
				// 	cout << endl;
				// }
				


				// if(jobflag){
				coord t1;
				t1.x = it.x+1;
				t1.y = it.y;
				t1.z = it.z;
				checkQueue(&LocalQueue[omp_get_thread_num()], t1, cell_index);


				coord t2;
				t2.x = it.x-1;
				t2.y = it.y;
				t2.z = it.z;
				checkQueue(&LocalQueue[omp_get_thread_num()], t2, cell_index);

				coord t3;
				t3.x = it.x;
				t3.y = it.y+1;
				t3.z = it.z;
				checkQueue(&LocalQueue[omp_get_thread_num()], t3, cell_index);

				coord t4;
				t4.x = it.x;
				t4.y = it.y-1;
				t4.z = it.z;
				checkQueue(&LocalQueue[omp_get_thread_num()], t4, cell_index);

				coord t5;
				t5.x = it.x;
				t5.y = it.y;
				t5.z = it.z+1;
				checkQueue(&LocalQueue[omp_get_thread_num()], t5, cell_index);

				coord t6;
				t6.x = it.x;
				t6.y = it.y;
				t6.z = it.z-1;
				checkQueue(&LocalQueue[omp_get_thread_num()], t6, cell_index);
					// }
						// cout << "pop done" << endl;
			}

			if(LocalQueue[omp_get_thread_num()].size() == 0){
				#pragma omp critical(break_calc)
				{	
					isEmpty = true;
				}
			}
		}

		// cout << "Merge Queue" << endl;
		for(int i=0;i<THREAD;++i){
			// if(LocalQueue[i].size() > 0){
			while(!LocalQueue[i].empty()){
				mQueue.push(LocalQueue[i].front());
				LocalQueue[i].pop();
			}
			// }
		}
		// cout << "Merge Queue Done" << endl;


	}

	// cout << "Cell done " << endl;

}

int connectedcomponent(int start_slice, int slice_size){
	long long cell_index = 0;
	for(int i=start_slice; i<start_slice+slice_size; ++i){
		for(int j=0;j<DIMX*DIMY;++j){
			coord c;
			c.x = j%DIMX;
			c.y = j/DIMX;
			c.z = i;
			unsigned char bitChar = getBitbyCoord(c.x, c.y, c.z, SegResultVolume);
			if((int)bitChar != 0){
				checkJob(c.x, c.y, c.z, cell_index++);
			}
		}

	}

	return 0;
}

void writeProcess(int volume_number, string path){
	// string path(OUTPATH);

	long long lvalue = 1;
	long long slice_size = lvalue*DIMX*DIMY*sizeof(int);

	for(int i=0;i<DIMZ;++i){
		string out_path = path + "v" + to_string(volume_number) + "_s"+to_string(i)+".raw";
		ofstream ofs(out_path, ios::binary);
		long long pos = lvalue*i*DIMY*DIMX;

		ofs.write(reinterpret_cast<char*>(&VisitTable[pos]), slice_size);
		ofs.close();
	}
}


int main(int argc, char **argv){

	string path(SEGPATH);

	omp_set_num_threads(THREAD);


	int volume_number = atoi(argv[1]);
	int node_number = atoi(argv[2]);

	long long lvalue = 1;
	
	// 
	long long OutputSize = lvalue * DIMX * DIMY * DIMZ;
	long long TableSize = lvalue*DIMX*DIMY*DIMZ;

	VisitTable = new unsigned int[OutputSize];

	
	cout << "Data Reading.." << endl;
	SegResultVolume = new unsigned char[TableSize];
//	vector<string> filelist = getFileList(path);    
  //  sort(filelist.begin(), filelist.end());
   	//cout << "Vector sizes" << filelist.size() << endl;

	int each_node_volume_size = Whole_VolumeSize / Whole_NodeSize;


	for(int i=0;i<DIMZ;++i){
		long long fileindex = lvalue * each_node_volume_size * DIMZ * node_number + lvalue * DIMZ * volume_number + i;
		long long target_index = lvalue * i * DIMY * DIMX;
		string file_path = path + ToDigitStr(fileindex, 5) + ".raw";

		FILE *fp = fopen(file_path.c_str(), "rb");

		fread(&SegResultVolume[target_index], DIMX*DIMY, 1, fp);
		fclose(fp);
	}
	cout << "Data Read Done" << endl;

	cout << "Output Volume Init..." << endl;
	long long memsetSize = OutputSize*4;
	// cout << memsetSize << endl;
	memset(VisitTable, 0, memsetSize);
	cout << "Output Volume Done" << endl;


	connectedcomponent(0, DIMZ);
	cout << "Connected Component done" <<endl;	
	// long long outSize = TableSize * 8 * 4;
	string _p(argv[3]);


	writeProcess(each_node_volume_size * node_number + volume_number, _p);

	delete VisitTable;
	delete SegResultVolume;


	return 0;
}
