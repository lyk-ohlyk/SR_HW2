//  WriteTxt
//  在主程序中未用到，是写plp文件列表（txt文件）的函数，
//  输入为txt文件的文件名。
//  lyk-20171006

#include "declaration.h"

//Write txt file filled with plp-file names
void WriteTxt(string txt_file) {
	ofstream fs;
	fs.open(txt_file, ios::out);

	_finddata_t fileDir;
	char* dir = "FD15\\*.plp";
	long lfDir;
	if ((lfDir = _findfirst(dir, &fileDir)) == -1l)
		printf("No file is found\n");
	else {
		printf("file list:\n");
		do {
			fs << fileDir.name << endl;
		} while (_findnext(lfDir, &fileDir) == 0);
	}
	fs.close();
	_findclose(lfDir);
}