//  CMVN
//  输入data，计算均值、方差之后规整数据，输出为cmvn_data
//  lyk-20171006

#include "declaration.h"

void CMVN(float *data, float *cmvn_data, int frames, int datas_per_frame) {
	float mean, sigma, temp_varience, minus_mean;
	for (int dim = 0; dim < datas_per_frame; dim++) {
		mean = temp_varience = 0;
		for (int i = 0; i < frames; i++) {
			mean += data[dim + i*datas_per_frame];
		}
		mean /= frames;

		for (int i = 0; i < frames; i++) {
			minus_mean = data[dim + i*datas_per_frame] - mean;
			temp_varience += (minus_mean * minus_mean);
		}
		temp_varience /= frames;
		sigma = sqrt(temp_varience);

		for (int i = 0; i < frames; i++) {
			cmvn_data[dim + i*datas_per_frame] = (data[dim + i*datas_per_frame] - mean) / sigma;
		}
	}
}