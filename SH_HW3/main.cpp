#include "Ngram.h"
#include <vector>
#include <map>
using namespace std;

int main(int agrc, char* argv[]) {
	Ngram mok;
	mok.readVocab("863.lmlist");
	//mok.readTrainFile("C:\\Users\\atu\\Desktop\\���ƴ�\\����ʶ��\\W3\\text_train.seg");
	mok.readTrainFile("lyk-train.txt");
	mok.writeFreq("ohya.txt");
	//mok.writeFreq("test_1_1406.txt");
	for (int i = 0; i < 10; i++) {
		cout << "" << endl;
	}

	system("pause");
	return 0;
}
