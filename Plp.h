#pragma once
#include<string>

class Plp
{
public:
	int frames;
	int useless_length;
	short bytes_per_frame;
	short type;
	float* data;

	int ReadFile(std::string filename);
	void PrintInfo();

	~Plp();
};
