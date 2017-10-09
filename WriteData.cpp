//  WriteData
//  将final_data写入文件中。正常情况下final_data中应包含了
//  计算后的所需特征。
//  lyk-20171006

#include "declaration.h"

int WriteData(string write_route, float* final_data, int final_data_size) {
	ofstream ofs;
	int data_position;

	ofs.open(write_route, ios::binary | ios::app);
	if (!ofs.is_open()) {
		cerr << "can't open write file" << endl;
		return -1;
	}

	ofs.write((char*)final_data, 4 * final_data_size);

	ofs.close();
	return 0;
}