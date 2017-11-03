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
	unordered_map<string, int> dict_map; //用来快速查找完全匹配的词

	bool FindInMap(const string&) const;
	string ClearWord(const string &) const;
	string AddSpace(const string &) const;
	int Compare(const string &dict, const string &word) const;
	int charCompare(const char &a, const char &b) const;
	vector<string> FindAlignment(const string &dict, const string &word) const;
};

inline int WordMatch::charCompare(const char &a, const char &b) const {
	if (a == b)
		return 1;  //字符相等时的得分
	else
		return -3;  //字符不等的得分
}

#endif // !WORDMATCH_H_
