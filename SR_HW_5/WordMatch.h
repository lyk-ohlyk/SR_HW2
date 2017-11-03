#pragma once
#ifndef WORDMATCH_H_
#define WORDMATCH_H_

#include "BK_Tree.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class WordMatch {
public:
	WordMatch() {};
	WordMatch(vector<string> a) : dictionary(a) {};
	void ReadDict(const string&);
	void ReadWords(const string&);
	void Match(const string&) const;

	void BK_Match(const string&) const;
	void BK_ReadDict(const string&);

private:
	Node bk_root;
	vector<string> dictionary, words;
	unordered_map<string, int> dict_map; //�������ٲ�����ȫƥ��Ĵ�

	bool FindInMap(const string&) const;
	string ClearWord(const string &) const;
	string AddSpace(const string &) const;
	int Compare(const string &dict, const string &word) const;
	int charCompare(const char &a, const char &b) const;
	vector<string> FindAlignment(const string &dict, const string &word) const;
};

inline int WordMatch::charCompare(const char &a, const char &b) const {
	if (a == b)
		return 1;  //�ַ����ʱ�ĵ÷�
	else
		return -3;  //�ַ����ȵĵ÷�
}

#endif // !WORDMATCH_H_
