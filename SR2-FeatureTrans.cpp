#include "declaration.h"
#include <io.h>

int main()
{
	string inDir, outDir, txt_filename;
	int diff_time, cmvn_mode;
	cout << "按以下格式输入参数：" << endl << "输入文件夹  输出文件夹  txt文件名  差分次数  CMVN模式" << endl;
	cout << "例如：inDir  OutDir  fealist.txt  3  2" << endl;
	cin >> inDir >> outDir >> txt_filename >> diff_time >> cmvn_mode;
	FeatureTrans(inDir, outDir, txt_filename, diff_time, cmvn_mode);
    return 0;
}

