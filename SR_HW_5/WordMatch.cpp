#include "WordMatch.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

#ifndef INT_MIN
#define INT_MIN 0x80000000
#endif // !INT_MIN

void WordMatch::ReadDict(const string &filename){
	ifstream ifs;
	ifs.open(filename);
	if (!ifs) {
		cerr << "Can't open dictionary file." << endl;
		exit(-2);
	}
	string dict_word;
	dictionary.clear();
	while (ifs >> dict_word) {
		dictionary.push_back(dict_word);
		dict_map.insert({ dict_word, 0 });
	}
	ifs.close();
}
//
//void WordMatch::BK_ReadDict(const string &filename) {
//	ifstream ifs;
//	ifs.open(filename);
//	if (!ifs) {
//		cerr << "Can't open dictionary file." << endl;
//		exit(-2);
//	}
//	string dict_word;
//	if (ifs >> dict_word)
//		bk_root = Node(dict_word);
//
//	while (ifs >> dict_word) {
//		dict_map.insert({ dict_word, 0 });
//		int value;
//		value = Compare(bk_root.dict, dict_word);
//		//if()
//
//	}
//	ifs.close();
//}

void WordMatch::ReadWords(const string &filename) {
	ifstream ifs;
	ifs.open(filename);
	if (!ifs) {
		cerr << "Can't open words file." << endl;
		exit(-2);
	}
	string word;
	while (ifs >> word)
		words.push_back(word);
	ifs.close();
}

bool WordMatch::FindInMap(const string &word) const {
	string word_clear = ClearWord(word);
	if (dict_map.find(word_clear) != dict_map.end())
		return true;
	return false;
}

void WordMatch::Match(const string &writefile) const{
	ofstream ofs;
	ofs.open(writefile, ios::out | ios::trunc);
	if (!ofs) {
		cerr << "Can't open writefile file." << endl;
		exit(-2);
	}
	for (int i = 0; i < words.size(); ++i) {
		string word = words[i];
		string matched_dict;
		if (FindInMap(words[i])) {
			matched_dict = ClearWord(words[i]);
		}
		else
		{
			int max_value = INT_MIN, temp_value;
			size_t max_pos;
			for (int j = 0; j < dictionary.size(); ++j) {
				temp_value = Compare(dictionary[j], word);
				if (temp_value > max_value) {
					max_value = temp_value;
					max_pos = j;
				}
			}
			matched_dict = dictionary[max_pos];  //找到得分最高的词
		}
		vector<string> ans =  FindAlignment(matched_dict, word);

		ofs << "Input word is: " << word << endl;
		ofs << "The most similar word is: " << matched_dict << endl;
		ofs << "Distance is: " << ans[3] << endl;
		ofs << "dict:\t " << AddSpace(ans[0]) << endl;
		ofs << "word:\t " << AddSpace(ans[1]) << endl;
		ofs << "result:\t " << AddSpace(ans[2]) << endl << endl;;
	}
	ofs.close();
}

//  动态规划，求得最优的匹配方式的得分
int WordMatch::Compare(const string &dict, const string &word) const{
	size_t dict_size = dict.size();
	size_t word_size = word.size();
	vector<int> score(dict_size + 1, 0);
	vector<int> newscore(dict_size + 1, 0);
	for (int i = 0; i < dict_size + 1; ++i)
		score[i] = -3 * i;
	for (int j = 1; j < word_size + 1; ++j) {
		newscore[0] = score[0] - 3;
		for (int i = 1; i < dict_size + 1; ++i) {
			newscore[i] = max(score[i - 1] + charCompare(dict[i - 1], word[j - 1]),
				max(score[i] - 3, newscore[i - 1] - 3));
		}
		for (int i = 0; i < dict_size + 1; ++i)
			score[i] = newscore[i];
	}
	return score[dict_size];  //最优匹配的得分
}

vector<string> WordMatch::FindAlignment(const string &dict, const string &word) const{
	string m_dict, m_word, m_condition;  //m 表示 match
	vector<vector<int>> value_matrix;  //用来保存评估矩阵，便于回溯
	size_t dict_size = dict.size();
	size_t word_size = word.size();
	vector<int> score(dict_size + 1, 0);
	vector<int> newscore(dict_size + 1, 0);
	for (size_t i = 0; i < dict_size + 1; ++i) {
		score[i] = -3 * i;
	}
	value_matrix.push_back(score);
	for (size_t j = 1; j < word_size + 1; ++j) {
		newscore[0] = score[0] - 3;
		for (size_t i = 1; i < dict_size + 1; ++i) {
			newscore[i] = max(score[i - 1] + charCompare(dict[i - 1], word[j - 1]),
				max(score[i] - 3, newscore[i - 1] - 3));
		}
		for (size_t i = 0; i < dict_size + 1; ++i)
			score[i] = newscore[i];
		value_matrix.push_back(score);
	}

	//按回溯法得到最优匹配序列
	size_t match_pos = dict_size;
	size_t i = word_size;
	int distance = 0;
	while (i > 0) {
		if (match_pos == 0) {
			m_dict.push_back('*');
			m_word.push_back(word[i - 1]);
			m_condition.push_back('I');
			++distance;
			--i;
			continue;
		}
		int value = charCompare(dict[match_pos - 1], word[i - 1]);
		if (value_matrix[i][match_pos] == value + value_matrix[i - 1][match_pos - 1]) {
			m_dict.push_back(dict[match_pos - 1]);
			m_word.push_back(word[i - 1]);
			if (value > 0)
				m_condition.push_back('C');
			else {
				m_condition.push_back('S');
				++distance;
			}
			--match_pos;
			--i;
		}
		else if (value_matrix[i][match_pos] == value_matrix[i - 1][match_pos] - 3) {
			m_dict.push_back('*');
			m_word.push_back(word[i - 1]);
			m_condition.push_back('I');
			++distance;
			--i;
		}
		else {
			m_dict.push_back(dict[match_pos - 1]);
			m_word.push_back('*');
			m_condition.push_back('D');
			++distance;
			--match_pos;
		}
	}
	reverse(m_dict.begin(), m_dict.end());
	reverse(m_word.begin(), m_word.end());
	reverse(m_condition.begin(), m_condition.end());
	stringstream ss;
	string s_distance;
	ss << distance;
	ss >> s_distance;
	return{ m_dict, m_word, m_condition, s_distance };
}


string WordMatch::AddSpace(const string &s) const {
	string s_add;
	for (auto i : s) {
		s_add.push_back(i);
		s_add.push_back(' ');
	}
	return s_add;
}

string WordMatch::ClearWord(const string &s) const {
	//  删去词中的冗余字符
	string s_clear;
	for (char a : s) {
		if (a == '!' || a == '@' || a == '#' || a == '$' || a == '%' || a == '*' || a == '(' || a == ')')
			continue;
		s_clear.push_back(a);
	}
	return s_clear;
}