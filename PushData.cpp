//  PushData
//  ��ĳ�β�ֵ����ݲ��뵽final_data�еĶ�Ӧλ��
//  lyk-20171006

#include "declaration.h"

void PushData(float* in_data, float* final_data, int frames, int datas_per_frame, int current_diff_time, int diff_time) {
	// Ϊ�˱�֤ÿ֡���ݵ�����
	for (int i = 0; i < frames; i++) {
		for (int j = 0; j < datas_per_frame; j++) {
			final_data[j + i * (diff_time + 1) * datas_per_frame + current_diff_time * datas_per_frame] = in_data[j + i*datas_per_frame];
		}
	}
}