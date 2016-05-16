#pragma comment(lib,"libiconv.lib") 
#include "fileutil.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

//FileUtils FileUtils::instance=new FileUtils();
using namespace std;

FileUtils* FileUtils::getInstance(){
	return new FileUtils(); //instance;
}

std::string FileUtils::getStringFromFile(char* filename){
	string result;

	ifstream file(filename);
	char buffer[256];

	FILE *fp;
	fp=fopen(filename,"rb");

	if (fp!=NULL){
		int cnt;//=fread(buffer,1,255,fp);
		while(!feof(fp)){
			memset(buffer,0,256);
			cnt= fread(buffer,1,255,fp);
			result.append(buffer,cnt);
		}
		fclose(fp);
	}
	 
	return result;
}
