//  定义了Plp类，用来实现对plp文件的读取操作
//  ReadFile用于读取文件头及数据，PrintInfo用于打印信息
//  lyk-20171006

#include "declaration.h"

Plp::~Plp() {
	delete[]data;
}

int Plp::ReadFile(std::string filename)
{
	ifstream ifs;
	ifs.open(filename, ios::binary | ios::in);
	if (!ifs) {
		cerr << "error route" << endl;
		return -1;
	}
	
	//ifs.seekg(0x00);
	ifs.read((char*)&frames, 4);
	//ifs.seekg(0x04);
	ifs.read((char*)&useless_length, 4);
	//ifs.seekg(0x08);
	ifs.read((char*)&bytes_per_frame, 2);
	//ifs.seekg(0x0a);
	ifs.read((char*)&type, 2);

	int data_size = frames * bytes_per_frame / 4;
	data = new float[data_size];
	ifs.read((char*)data, 4 * data_size);

	ifs.close();
}


void Plp::PrintInfo()
{
	cout << "frames:   " << frames << endl;
	cout << "length per frames:   " << useless_length << endl;
	cout << "bytes per frame:   " << bytes_per_frame << endl;
	cout << "type:   " << type << endl;
}