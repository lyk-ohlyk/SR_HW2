#pragma once
#ifndef NGRAM_H
#define NGRAM_H

#include<map>
#include<string>
#include<vector>
using namespace std;

class Vocab {
public:
	map<string, size_t> map_vocab;
	vector<string> vector_vocab;
	size_t nextIndex, maxIndex;
};

class Trie {
public:
	map<size_t, Trie*> sub_trie;
	unsigned long count;
	double prob, bow;
	Trie* insertTrie(size_t word_id);  
	Trie* findSubTrie(size_t word_id);

	Trie() { count = 0; prob = 0; bow = 0; }
};

class NgramCounts {
public:
	Vocab vocab;
	size_t order;
	size_t k;
	Trie root;

	double *param;
	NgramCounts() { order = 3; k = 6;}

	void readTrainFile_(string filename);
	//void readTrainFile(string filename);
	//void Insert(vector<string> word_vec);
	void Estimate();
	double Discount(unsigned long count, size_t n);
	void computeProb();
	void computeBow();
	void computePpl(string fin, string fout);
	double getProb(vector<size_t> ngram);
	size_t getId(string word);
	void writeFreq(string filename);
	void writeArpa(string filename);
	void readVocab(string filename);
};

#endif // !NGRAM_H
