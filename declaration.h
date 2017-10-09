// declaration.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include <io.h>

#include "Plp.h"

using namespace std;

int FeatureTrans(std::string inDir, std::string outDir, std::string textFile,int difference, int cmvn);
void Difference(float* inData, float* outData, int data_size);
void WriteHead(string write_route, int frames, int useless, short bytes_per_frame, short type);
int WriteData(string write_route, float* out_data, int final_data_size);
void CMVN(float* data, float*cmvn_data, int frames, int datas_per_frame);
void PushData(float* in_data, float* final_data, int frames, int bytes_per_frame, int current_diff_time, int diff_time);

void WriteTxt(string txt_file);
