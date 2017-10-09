//  WriteHead
//  写入plp文件的文件头，共12字节
//  lyk-20171006

#include "declaration.h"

void WriteHead(string write_route, int frames, int useless, short bytes_per_frame, short type) {
	ofstream ofs;
	ofs.open(write_route, ios::binary | ios::out);
	ofs.write((char*)&frames, 4);
	ofs.write((char*)&useless, 4);
	ofs.write((char*)&bytes_per_frame, 2);
	ofs.write((char*)&type, 2);
	ofs.close();
}