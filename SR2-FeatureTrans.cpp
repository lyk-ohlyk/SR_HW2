#include "declaration.h"
#include <io.h>

int main()
{
	string inDir, outDir, txt_filename;
	int diff_time, cmvn_mode;
	cout << "�����¸�ʽ���������" << endl << "�����ļ���  ����ļ���  txt�ļ���  ��ִ���  CMVNģʽ" << endl;
	cout << "���磺inDir  OutDir  fealist.txt  3  2" << endl;
	cin >> inDir >> outDir >> txt_filename >> diff_time >> cmvn_mode;
	FeatureTrans(inDir, outDir, txt_filename, diff_time, cmvn_mode);
    return 0;
}

