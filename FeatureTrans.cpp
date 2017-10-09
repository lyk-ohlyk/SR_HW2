//  FeatureTrans
//  main程序中使用的函数。
//  参数依次为输入文件夹、输出文件夹、文件列表（txt）、差分阶数、CMVN选项
//  lyk-20171006

#include "declaration.h"

int FeatureTrans(std::string inDir, std::string outDir, std::string textFile, int diff_time, int cmvn)
{
	inDir = inDir + "\\";
	outDir = outDir + "\\";
	string plp_file_name, plp_read_route, plp_write_route;
	ifstream text_fs(textFile);

	if (diff_time > 10 || diff_time < 0) {//设定最大差分次数为10
		cerr << "Too much difference or negative diff_time" << endl;
		return -1;
	}
	if (cmvn != 1 && cmvn != 2 && cmvn != 0) {
		cerr << "Wrong type of CMVN" << endl;
		return -2;
	}
	if (!text_fs) {
		cerr << textFile << endl;
		cerr << "Wrong text file route" << endl;
		return -3;
	}

	while (getline(text_fs, plp_file_name) && (cmvn == 1 || cmvn == 0)) {
		Plp plp_file;
		plp_read_route = inDir + plp_file_name;
		plp_write_route = outDir + plp_file_name;

		plp_file.ReadFile(plp_read_route);
		int datas_per_frame = plp_file.bytes_per_frame / 4;
		int data_size = plp_file.frames * datas_per_frame;
		float* out_data = new float[data_size];
		float* final_data = new float[data_size*(1 + diff_time)];

		WriteHead(plp_write_route, plp_file.frames, plp_file.useless_length, plp_file.bytes_per_frame * (diff_time + 1), plp_file.type);
		if (cmvn == 0) {
			for (int i = 0; i < data_size; i++) {
				out_data[i] = plp_file.data[i];
			}

			for (int i = 0; i <= diff_time; i++) {
				PushData(out_data, final_data, plp_file.frames, datas_per_frame, i, diff_time);
				if (i != diff_time)
					Difference(out_data, out_data, data_size);
			}//for
			WriteData(plp_write_route, final_data, (diff_time + 1)*data_size);
		}//if
		else if (cmvn == 1) {
			float* dif_data = new float[data_size];
			for (int i = 0; i < data_size; i++) {
				dif_data[i] = plp_file.data[i];
			}

			for (int i = 0; i <= diff_time; i++) {
				CMVN(dif_data, out_data, plp_file.frames, datas_per_frame);
				PushData(out_data, final_data, plp_file.frames, datas_per_frame, i, diff_time);
				if (i != diff_time)
					Difference(dif_data, dif_data, data_size);
			}//for			
			WriteData(plp_write_route, final_data, (diff_time + 1)*data_size);
			delete[]dif_data;
		}//else if
		delete[]out_data;
		delete[]final_data;
	} // while(getline)

	  //  global cmvn, first traversal for calculating global mean ,
	  //  second traversal for calculating global sigma,
	  //  third traversal for updating data
	if (cmvn == 2) {
		float *global_mean = new float[(1 + diff_time) * 15];
		float *global_sigma = new float[(1 + diff_time) * 15];
		float total_frames = 0;
		for (int i = 0; i < (1 + diff_time) * 15; i++) {
			global_mean[i] = 0;
			global_sigma[i] = 0;
		}

		text_fs.clear();
		text_fs.seekg(0); //返回txt文件开头

		//  first traversal for calculating global mean
		while (getline(text_fs, plp_file_name)) {
			Plp plp_file;
			plp_read_route = inDir + plp_file_name;
			plp_file.ReadFile(plp_read_route);
			total_frames += plp_file.frames;
			int datas_per_frame = plp_file.bytes_per_frame / 4;
			int data_size = plp_file.frames * datas_per_frame;

			float* out_data = new float[data_size];
			for (int i = 0; i < data_size; i++) {
				out_data[i] = plp_file.data[i];
			}

			//  save the sum of every dim in global_mean
			for (int i = 0; i <= diff_time; i++) {
				for (int m = 0; m < plp_file.frames; m++) {
					for (int n = 0; n < datas_per_frame; n++) {
						global_mean[n + i * datas_per_frame] += out_data[n + m * datas_per_frame];
					}
				}
				if (i != diff_time)
					Difference(out_data, out_data, data_size);
			}

			for (int i = 0; i < datas_per_frame * (1 + diff_time); i++) {
				global_mean[i] /= total_frames;
			}
			delete[]out_data;
		}//  first traversal

		text_fs.clear();
		text_fs.seekg(0); //返回txt文件开头

		//  second traversal for calculating global sigma
		while (getline(text_fs, plp_file_name)) {
			Plp plp_file;
			plp_read_route = inDir + plp_file_name;
			plp_file.ReadFile(plp_read_route);
			int datas_per_frame = plp_file.bytes_per_frame / 4;
			int data_size = plp_file.frames * datas_per_frame;

			float* out_data = new float[data_size];

			for (int i = 0; i < data_size; i++) {
				out_data[i] = plp_file.data[i];
			}
			for (int i = 0; i <= diff_time; i++) {
				//save the sum of every dim in global_mean
				for (int m = 0; m < plp_file.frames; m++) {
					for (int n = 0; n < datas_per_frame; n++) {
						global_sigma[n + i * datas_per_frame] += (out_data[n + m * datas_per_frame] - global_mean[n + i * datas_per_frame])
							* (out_data[n + m * datas_per_frame] - global_mean[n + i * datas_per_frame]);
					}
				}
				if (i != diff_time)
					Difference(out_data, out_data, data_size);
			}

			for (int i = 0; i < datas_per_frame * (1 + diff_time); i++) {
				global_sigma[i] /= total_frames;
				global_sigma[i] = sqrt(global_sigma[i]);
			}
			delete[]out_data;
		}//  second traversal

		text_fs.clear();
		text_fs.seekg(0); //返回txt文件开头
						
		//  third traversal for updating data
		while (getline(text_fs, plp_file_name)) {
			Plp plp_file;
			plp_read_route = inDir + plp_file_name;
			plp_write_route = outDir + plp_file_name;
			plp_file.ReadFile(plp_read_route);
			int datas_per_frame = plp_file.bytes_per_frame / 4;
			int data_size = plp_file.frames * datas_per_frame;
			float *dif_data = new float[data_size];
			float *out_data = new float[data_size];
			float *final_data = new float[data_size * (1 + diff_time)];

			for (int i = 0; i < data_size; i++) {
				dif_data[i] = plp_file.data[i];
			}

			WriteHead(plp_write_route, plp_file.frames, plp_file.useless_length, plp_file.bytes_per_frame * (diff_time + 1), plp_file.type);
			for (int i = 0; i <= diff_time; i++) {
				for (int m = 0; m < plp_file.frames; m++) {
					for (int n = 0; n < datas_per_frame; n++) {
						out_data[n + m * datas_per_frame] = (dif_data[n + m * datas_per_frame] - global_mean[n + i * datas_per_frame])
							/ global_sigma[n + i * datas_per_frame];
					}
				}
				PushData(out_data, final_data, plp_file.frames, datas_per_frame, i, diff_time);
				if (i != diff_time)
					Difference(dif_data, dif_data, data_size);
			}
			WriteData(plp_write_route, final_data, data_size * (1 + diff_time));

			delete[]dif_data;
			delete[]out_data;
			delete[]final_data;
		}//  third traversal

		delete[]global_mean;
		delete[]global_sigma;
	}

	text_fs.close();
	return 0;

}
