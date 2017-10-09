//  Difference
//  对输入的inData进行差分，并输出为outData
//  lyk-20171006

#include "declaration.h"

void Difference(float* inData, float* outData, int data_size) {
	outData[0] = (inData[1] - inData[0] + 2 * inData[2] - 2 * inData[0]) / 10;
	outData[1] = (inData[2] - inData[0] + 2 * inData[3] - 2 * inData[1]) / 10;
	outData[data_size - 1] = (inData[data_size - 1] - inData[data_size - 2] + 2 * inData[data_size - 1] - 2 * inData[data_size - 3]) / 10;
	outData[data_size - 2] = (inData[data_size - 1] - inData[data_size - 3] + 2 * inData[data_size - 2] - 2 * inData[data_size - 4]) / 10;
	for (int i = 2; i < data_size - 2; i++) {
		outData[i] = (inData[i + 1] - inData[i - 1] + 2 * inData[i + 2] - 2 * inData[i - 2]) / 10;
	}
}