//  PushData
//  将某次差分的数据插入到final_data中的对应位置
//  lyk-20171006

#include "declaration.h"

void PushData(float* in_data, float* final_data, int frames, int datas_per_frame, int current_diff_time, int diff_time) {
	// 为了保证每帧数据的连续
	for (int i = 0; i < frames; i++) {
		for (int j = 0; j < datas_per_frame; j++) {
			final_data[j + i * (diff_time + 1) * datas_per_frame + current_diff_time * datas_per_frame] = in_data[j + i*datas_per_frame];
		}
	}
}