//  Difference
//  对输入的inData进行差分，并输出为outData
//  lyk-20171006

#include "declaration.h"

void Difference(float* inData, float* outData, int data_size) {
	if (inData != outData) {
		outData[0] = (inData[1] - inData[0] + 2 * inData[2] - 2 * inData[0]) / 10;
		outData[1] = (inData[2] - inData[0] + 2 * inData[3] - 2 * inData[1]) / 10;
		outData[data_size - 1] = (inData[data_size - 1] - inData[data_size - 2] + 2 * inData[data_size - 1] - 2 * inData[data_size - 3]) / 10;
		outData[data_size - 2] = (inData[data_size - 1] - inData[data_size - 3] + 2 * inData[data_size - 2] - 2 * inData[data_size - 4]) / 10;
		for (int i = 2; i < data_size - 2; i++) {
			outData[i] = (inData[i + 1] - inData[i - 1] + 2 * inData[i + 2] - 2 * inData[i - 2]) / 10;
		}
	}
	else {
		float *tempData = new float[data_size];
		tempData[0] = (inData[1] - inData[0] + 2 * inData[2] - 2 * inData[0]) / 10;
		tempData[1] = (inData[2] - inData[0] + 2 * inData[3] - 2 * inData[1]) / 10;
		tempData[data_size - 1] = (inData[data_size - 1] - inData[data_size - 2] + 2 * inData[data_size - 1] - 2 * inData[data_size - 3]) / 10;
		tempData[data_size - 2] = (inData[data_size - 1] - inData[data_size - 3] + 2 * inData[data_size - 2] - 2 * inData[data_size - 4]) / 10;
		for (int i = 2; i < data_size - 2; i++) {
			tempData[i] = (inData[i + 1] - inData[i - 1] + 2 * inData[i + 2] - 2 * inData[i - 2]) / 10;
		}
		for (int i = 0; i < data_size; i++) {
			outData[i] = tempData[i];
		}
		delete[] tempData;
	}
}