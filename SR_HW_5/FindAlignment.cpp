#include "head.h"

//vector<vector<size_t>> FindAlignment(const string &dict, const string &word){
//	if (dict.size() == 0 || word.size() == 0)
//		return{ {} };
//	static vector<vector<size_t>> align_pos;
//	vector<int> pre_vec(word.size()), suf_vec(word.size());  //用来保存prefix和suffix的比较数组
//	pre_vec = SubCompare(dict, word.substr(0, word.size() / 2), 0);
//	suf_vec = SubCompare(dict, word.substr(word.size() / 2 + 1), true);
//	size_t maxpos = 0;
//	int maxvalue = 0, temp;
//	for (int i = 0; i < dict.size(); ++i) {
//		temp = pre_vec[i] + suf_vec[i];
//		if (temp > maxvalue) {
//			maxvalue = temp;
//			maxpos = i;
//		}
//	}
//
//	align_pos.push_back({ maxpos, word.size() / 2 });
//	FindAlignment(dict.substr(0, maxpos + 1), word.substr(0, word.size() / 2));
//	FindAlignment(dict.substr(maxpos + 1), word.substr(word.size() / 2));
//	return align_pos;
//}