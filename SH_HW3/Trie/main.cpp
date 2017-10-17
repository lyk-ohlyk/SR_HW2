#include "Trie_Gram_Vocab.h"

int main(int argc, char* argv[]) {

	NgramCounts test;
	test.readVocab("863.lmlist");	
	test.readTrainFile_("text_train.seg");
	test.Estimate();
	test.computeProb();
	test.computeBow();
	//test.writeArpa("text_train.arpa");
	test.computePpl("text_test.seg", "text_test.ppl");
	system("pause");
	return 11;
}