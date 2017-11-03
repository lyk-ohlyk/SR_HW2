#include "head.h"
#include <iostream>
#include<time.h>

int ArgPos(char *str, int argc, char **argv) {
	int a;
	for (a = 1; a < argc; ++a)
		if (!strcmp(str, argv[a])) {
			if (a == argc - 1) {
				printf("Argument missing for %s\n", str);
				exit(1);
			}
			return a;
		}
	return -1;
}

int main(int argc, char **argv) {
	//if (argc == 1) {
	//	cout << endl << "------------------ INPUT HELP ---------------------" << endl;
	//	cout << "-dict\t enter dictionary file route, like \"sorted.word\"." << endl;
	//	cout << "-test\t enter your test file route, like \"test.word\"." << endl;
	//	cout << "-output\t enter the output file route." << endl << endl;
	//	cout << "You must enter all the parameters above to make the program work." << endl;
	//	cout << "This Program shall only take you a few seconds." << endl;
	//	cout << "------------------- END HELP ----------------------" << endl;
	//	return 0;
	//}
	//
	//int i;
	//string dict_file, test_file, output_file;
	//if ((i = ArgPos((char *)"-dict", argc, argv)) > 0) dict_file = argv[i + 1];
	//if ((i = ArgPos((char *)"-test", argc, argv)) > 0) test_file = argv[i + 1];
	//if ((i = ArgPos((char *)"-output", argc, argv)) > 0) output_file = argv[i + 1];

	//WordMatch WM;
	//
	//WM.ReadDict(dict_file);
	//WM.ReadWords(test_file);
	//WM.Match(output_file);
	
	string dict_file, test_file, output_file;
	dict_file = "sorted.word"; test_file = "test.word"; output_file = "testtime";
	WordMatch WM;	auto start_time = clock();
	WM.ReadDict(dict_file);
	auto end1 = clock();
	WM.ReadWords(test_file);
	auto end2 = clock();
	WM.Match(output_file);
	auto finish = clock();

	cout << "read dict time:\t" << end1 - start_time << "ms" << endl;
	cout << "read test time:\t" << end2 - end1 << "ms" << endl;
	cout << "run test time:\t" << finish - end2 << "ms" << endl;
	system("pause");
	return 1;
/*
	int a = lykCompare("redemes", "redeemed");
	int b = lykCompare("redemes", "recedes");
	cout << a << "\t" << b << endl;*/
	//system("pause");
}