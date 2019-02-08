#include<sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>

#include <iomanip>
using namespace std;


string CellPath = "";
string SubregionPath = "";

bool fexists(const char *filename);
vector<string> Split(const char *phrase, string delimiter);
vector<string> getfilelist(string root, string type);
map<unsigned int, bool> includeCell(string subregion_block, string cell_block);

unsigned int *temp_block_slice;
unsigned int *sub_volume;


int main(int argc, char **argv){

	CellPath += argv[1];
	SubregionPath += argv[2];
	string Outpath1 = "";
	Outpath1 += argv[3];
	string Outpath2 = "";
	Outpath2 += argv[4];

	vector<string> cellblock_list = getfilelist(CellPath, "block");
	vector<string> subregionblock_list = getfilelist(SubregionPath, "block");

	long long int lvalue = 1;
	long long int block_size = lvalue * 512 * 512 * 512;

	map<unsigned int, bool> result_include;
	map<unsigned int, bool> result_intersect;

	for(int i=0;i<subregionblock_list.size();++i){
		for(int j=0;j<cellblock_list.size();++j){
			if (subregionblock_list[i].compare(cellblock_list[j]) == 0) {
				if (subregionblock_list[i].find("0.block") != string::npos) {
					string block_name = subregionblock_list[i];
					
					map<unsigned int, bool> include_map;
					map<unsigned int, bool> intersect_map;

					string subregion_block_path = SubregionPath + block_name;
					string cell_block_path = CellPath + block_name;
					cout << "Subregion Path : " << subregion_block_path << endl;
					cout << "Cell Path : " << cell_block_path << endl;


					long long int subregion_block_size = block_size * sizeof(unsigned char);
					long long int cell_block_size = block_size * sizeof(unsigned int);

					FILE *cell_blck_rfp = fopen(cell_block_path.c_str(), "rb");
					FILE *subregion_blck_rfp = fopen(subregion_block_path.c_str(), "rb");
					
					unsigned char *subregion_block = new unsigned char[block_size];
					unsigned int *cell_block = new unsigned int[block_size];

					fread(cell_block, cell_block_size, 1, cell_blck_rfp);
					fread(subregion_block, subregion_block_size, 1, subregion_blck_rfp);


					for(long long int i = 0; i < block_size; ++i){
						if(subregion_block[i] > 0){
							if(cell_block[i] != 0){
								include_map[cell_block[i]] = true;
								cell_block[i] = 0;
							}
						}
					}

					for(long long int i = 0; i < block_size; ++i){
						if(cell_block[i] != 0){
							if ( include_map.find(cell_block[i]) == include_map.end() ) {
								continue;
							} else {
								intersect_map[cell_block[i]] = true;
							}
						}
					}

					map<unsigned int, bool>::iterator iter;
					for (iter = include_map.begin(); iter != include_map.end(); ++iter) {
						result_include[iter->first] = iter->second;
					}
					for (iter = intersect_map.begin(); iter != intersect_map.end(); ++iter) {
						result_intersect[iter->first] = iter->second;
					}

					fclose(cell_blck_rfp);
					fclose(subregion_blck_rfp);

					delete[] subregion_block;
					delete[] cell_block;

				}
			}
		}
	}
	
	ofstream mOfs(Outpath1);
	map<unsigned int, bool>::iterator iter;
	for (iter = result_include.begin(); iter != result_include.end(); ++iter) {
		mOfs << iter->first << " ";
	}	
	ofstream mOfs2(Outpath2);
	for (iter = result_intersect.begin(); iter != result_intersect.end(); ++iter) {
		mOfs2 << iter->first << " ";
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
