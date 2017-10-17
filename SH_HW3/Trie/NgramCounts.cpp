#include "Trie_Gram_Vocab.h"
#include <fstream>
#include <iostream>
#include <math.h>
using namespace std;

size_t NgramCounts::getId(string word) {
	auto itr = vocab.map_vocab.find(word);
	if (itr != vocab.map_vocab.end()) {
		return vocab.map_vocab[word];
	}
	else
		return 0;  //<unk>
}

void NgramCounts::writeFreq(string filename) {
	ofstream fs;
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open the write file." << endl;
		exit(-3);
	}
	cout << "-------------------------------------------------" << endl;
	cout << "||||||||||||||strat writing file||||||||||||||" << endl;

	fs << "\\data\\" << endl;
	fs << "Ngram 1 = " << root.sub_trie.size() << endl << endl;

	fs << "\\1-gram" << endl;	
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		size_t id1 = itr1->first;
		fs << vocab.vector_vocab[id1] << "\t\t" << itr1->second->count << endl;
	}

	fs << endl << "\\2-gram" << endl;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		size_t id1 = itr1->first;
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			size_t id2 = itr2->first;
			fs << vocab.vector_vocab[id1] << " " << vocab.vector_vocab[id2]
				<< "\t\t" << itr2->second->count << endl;
		}
	}
	fs << endl << "\\3-gram" << endl;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		size_t id1 = itr1->first;
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			size_t id2 = itr2->first;
			for (auto itr3 = itr2->second->sub_trie.begin(); itr3 != itr2->second->sub_trie.end(); itr3++) {
				size_t id3 = itr3->first;
				fs << vocab.vector_vocab[id1] << " " << vocab.vector_vocab[id2] << " "
					<< vocab.vector_vocab[id3] << "\t\t" << itr3->second->count << endl;
			}
		}
	}	

	cout << "----------------------DONE------------------------" << endl;

}

void NgramCounts::writeArpa(string filename) {
	ofstream fs;
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open the write file." << endl;
		exit(-3);
	}
	cout << "-------------------------------------------------" << endl;
	cout << "||||||||||||||strat writing file||||||||||||||" << endl;

	unsigned long gram1 = 0, gram2 = 0, gram3 = 0;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		gram1++;
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			gram2++;
			for (auto itr3 = itr2->second->sub_trie.begin(); itr3 != itr2->second->sub_trie.end(); itr3++) {
				gram3++;
			}
		}
	}

	fs << "\\data\\" << endl;
	fs << "ngram 1 = " << gram1 << endl;
	fs << "ngram 2 = " << gram2 << endl;
	fs << "ngram 3 = " << gram3 << endl << endl;


	fs << "\\1-grams:" << endl;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		size_t id1 = itr1->first;
		fs << log10(itr1->second->prob) << "\t" << vocab.vector_vocab[id1];
		if(itr1->second->bow != 1) fs << "\t" << log10(itr1->second->bow);
		fs<< endl;
	}

	fs << endl << "\\2-grams:" << endl;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		size_t id1 = itr1->first;
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			size_t id2 = itr2->first;
			fs << log10(itr2->second->prob) << "\t" << vocab.vector_vocab[id1] << " " << vocab.vector_vocab[id2];
			if (itr2->second->bow != 1) fs << "\t" << log10(itr1->second->bow);
			fs << endl;
		}
	}
	fs << endl << "\\3-grams:" << endl;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		size_t id1 = itr1->first;
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			size_t id2 = itr2->first;
			for (auto itr3 = itr2->second->sub_trie.begin(); itr3 != itr2->second->sub_trie.end(); itr3++) {
				size_t id3 = itr3->first;
				fs << log10(itr3->second->prob) << "\t" << vocab.vector_vocab[id1] << " " << vocab.vector_vocab[id2] << " "
					<< vocab.vector_vocab[id3] << endl;
			}
		}
	}

	fs << endl << "\\end\\" << endl;
	cout << "----------------------DONE------------------------" << endl;

}

void NgramCounts::readVocab(string filename) {
	ifstream fs;
	string word;
	size_t id = 0; //ID=0 <UNK>
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open vocab file." << endl;
		exit(-2);
	}
	vocab.map_vocab.insert(map<string, size_t>::value_type("<unk>", 0));
	vocab.vector_vocab.push_back("<unk>");
	root.insertTrie(0);
	while (fs >> word) {
		id++;
		vocab.map_vocab.insert(map<string, size_t>::value_type(word, id));
		vocab.vector_vocab.push_back(word);
		root.insertTrie(id);
	}
}

void NgramCounts::readTrainFile_(string filename) {
	vector<size_t> word_ids;
	string word;
	ifstream fs;
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open training set." << endl;
		exit(-2);
	}
	size_t id;
	Trie* node;
	int sentence_count = 0;
	while (fs >> word) {
		id = getId(word);
		word_ids.push_back(id);
		if (id == 1) {//"</s>"
			sentence_count++;
			if (sentence_count % 1000 == 0) {
				printf("%cSentence Processed: %d", 13, sentence_count);
				fflush(stdout);
			}

			for (size_t i = 0; i < word_ids.size(); i++) {
				node = &root;
				node->count++; //total word nums in train file
				for (size_t j = 0; j < order; j++) {
					if (i + j < word_ids.size()) {
						node = node->insertTrie(word_ids[i + j]);
						node->count++;
					}
				}
			}
			word_ids.clear();
		}
	}
}

void NgramCounts::Estimate() {
	param = new double[(k+1)*order];
	for (int i = 0; i < (k + 1) * order; i++) param[i] = 0;

	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		if (itr1->second->count <= 6) {
			param[(size_t)itr1->second->count]++;
		}
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			if (itr2->second->count <= 6) {
				param[(size_t)(itr2->second->count) + k + 1]++;
			}
			for (auto itr3 = itr2->second->sub_trie.begin(); itr3 != itr2->second->sub_trie.end(); itr3++) {
				if (itr3->second->count <= 6) {
					param[(size_t)(itr3->second->count) + (k + 1) * 2]++;
				}
			}
		}
	}
}

double NgramCounts::Discount(unsigned long count, size_t n) {
	if (count >= 6) return (double)count;
	else if(count) return ((count + 1) * param[n * (k + 1) + count + 1] / param[n * (k + 1) + count] - count * k * param[n * (k + 1) + 6] / param[n * (k + 1) + 1])
		/ (1 - k * param[n * (k + 1) + 6] / param[n * (k + 1) + 1]);
	else return param[n * (k + 1) + 1] / param[n * (k + 1)];
}

void NgramCounts::computeProb() {
	printf("\nComputing Probability...\n");
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		itr1->second->prob = Discount(itr1->second->count, 0) / (double)(root.count);
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			itr2->second->prob = Discount(itr2->second->count, 1) / (double)itr1->second->count;
			itr1->second->bow += itr2->second->prob;
			for (auto itr3 = itr2->second->sub_trie.begin(); itr3 != itr2->second->sub_trie.end(); itr3++) {
				itr3->second->prob = Discount(itr3->second->count, 2) / (double)itr2->second->count;
				itr2->second->bow += itr3->second->prob;
			}
		}
	}
	printf("Done with Computing Probability!\n");
}

void NgramCounts::computeBow() {
	printf("Computing Back-off...\n");
	double denominator1, denominator2;
	map<size_t, Trie*>::iterator itr_prev;
	for (auto itr1 = root.sub_trie.begin(); itr1 != root.sub_trie.end(); itr1++) {
		itr1->second->bow = 1 - itr1->second->bow;
		denominator1 = 0;
		for (auto itr2 = itr1->second->sub_trie.begin(); itr2 != itr1->second->sub_trie.end(); itr2++) {
			itr2->second->bow = 1 - itr2->second->bow;
			denominator2 = 0;
			itr_prev = root.sub_trie.find(itr2->first);
			denominator1 += itr_prev->second->prob;
			for (auto itr3 = itr2->second->sub_trie.begin(); itr3 != itr2->second->sub_trie.end(); itr3++) {
				itr3->second->bow = 1 - itr3->second->bow;
				itr_prev = root.sub_trie.find(itr2->first)->second->sub_trie.find(itr3->first);
				denominator2 += itr_prev->second->prob;
			}
			denominator2 = 1 - denominator2;
			if (!denominator2) denominator2 += 1e-8;
			itr2->second->bow /= denominator2;
			if (!itr2->second->bow) itr2->second->bow += 1e-8;
		}
		denominator1 = 1 - denominator1;
		if (!denominator1) denominator1 += 1e-8;
		itr1->second->bow /= denominator1;
		if (!itr1->second->bow) itr1->second->bow += 1e-8;
	}
	printf("Done with Computing Back-off!\n");
}

void NgramCounts::computePpl(string fin, string fout) {
	vector<size_t> word_ids;
	vector<size_t> ngram;
	string word;
	ifstream fins;
	ofstream fouts;
	fins.open(fin);
	fouts.open(fout);

	if (!fins) {
		cerr << "Can't open training set." << endl;
		exit(-2);
	}
	size_t id;
	Trie* node;
	int sentence_count = 0, words_count = 0;
	double Prob = 0, logProb = 0, sentLogProb = 0, totalLogProb = 0, ppl, ppl1;

	printf("Computing Perplexity...\n");
	while (fins >> word) {
		id = getId(word);
		words_count++;
		word_ids.push_back(id);
		if (id == 1) {//"</s>"
			sentence_count++;
			if (sentence_count % 1000 == 0) {
				printf("%cSentence Processed: %d", 13, sentence_count);
				fflush(stdout);
			}

			sentLogProb = 0;

			for (size_t i = 0; i < word_ids.size(); i++) fouts << vocab.vector_vocab[word_ids[i]] << " "; //print whole sentence
			fouts << endl;

			for (int i = 1; i < word_ids.size(); i++) {
				for (int j = order - 1; j >= 0; j--) {
					if (i - j >= 0) ngram.push_back(word_ids[i - j]);
				}
				Prob = getProb(ngram);
				logProb = log10(Prob);
				sentLogProb += logProb; 
				fouts << "\tP( " << vocab.vector_vocab[ngram[ngram.size() - 1]] << " | ";
				for (size_t i = 0; i < ngram.size() - 1; i++) fouts << vocab.vector_vocab[ngram[i]] << " ";
				fouts << ")\t=\t" << Prob << endl;
				ngram.clear();
			}
			totalLogProb += sentLogProb;
			fouts << "1 sentence, " << word_ids.size() - 2 << " words" << endl;
			ppl = pow(10, -1 * sentLogProb / (word_ids.size() - 1));
			ppl1 = pow(10, -1 * sentLogProb / (word_ids.size() - 2));
			fouts << "logprob= "<< sentLogProb << " ppl= "<< ppl << " ppl1= "<< ppl1 << endl << endl;
			word_ids.clear();
		}
	}
	fouts << "file " << fin << ": " << sentence_count << " sentences, " << words_count << " words" << endl;
	ppl = pow(10, -1 * totalLogProb / (words_count - sentence_count));
	ppl1 = pow(10, -1 * totalLogProb / (words_count - 2 * sentence_count));
	fouts << "logprob= " << totalLogProb << " ppl= " << ppl << " ppl1= " << ppl1 << endl << endl;

	printf("\nDone with Perplexity!\n");
}

double NgramCounts::getProb(vector<size_t> ngram) {
	Trie* node;
	double bow;
	vector<size_t> n_1gram;
	size_t i = 0, j = 0, len = ngram.size();

	node = &root;
	for (i = 0; i < len; i++) {
		bow = node->bow;
		node = node->findSubTrie(ngram[i]);
		if (node == nullptr) break;
	}
	if (i == len) return node->prob;
	else {
		for (j = 1; j < len; j++) n_1gram.push_back(ngram[j]);
		if (i == len - 1) return bow * getProb(n_1gram);
		return getProb(n_1gram);
	}

}