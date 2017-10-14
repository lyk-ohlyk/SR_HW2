#pragma once
#ifndef NGRAM_H
#define NGRAM_H

#include"LinkList.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
using namespace std;

class Ngram {
public:
	const int order = 3;
	Node *root;
	Node *word1_pos, *word2_pos;
	map<string, size_t> vocab;
	map<size_t, string> id_vocab;

	Ngram() { 
		word1_pos = word2_pos = nullptr;
		root = new Node; 
		root->children = new LinkList;
		root->children->head = new Node;
	}

	size_t getId(string word);
	void Insert(vector<string> word_vec);
	vector<string> wordSplit(string gram);
	size_t getFreq(string gram);
	void readTrainFile(string filename);
	void readVocab(string filename);
	void writeFreq(string filename);
};

size_t Ngram::getId(string word) {
	auto itr = vocab.find(word);
	if (itr != vocab.end()) {
		return vocab[word];
	}
	else
		return 1;  //<unk>
}

void Ngram::Insert(vector<string> word_vec) {

	Node *current_node = root;
	size_t id;
	size_t vec_size = word_vec.size();
	Node *insert_node_father = nullptr;
	id = getId(word_vec[word_vec.size() - 1]);
	if (vec_size == 1)
		current_node = root->children->Search(id);
	if (vec_size == 2)
		current_node = word1_pos->children->Search(id);
	if (vec_size == 3)
		current_node = word2_pos->children->Search(id);
	//for (size_t i = 0; i < word_vec.size(); i++) {
	//	id = getId(word_vec[i]);
	//	if (i == word_vec.size() - 1)
	//		insert_node_father = current_node;
	//		current_node = current_node->children->Search(id);  
	//}  //找到了最后一个词的id对应位置
	if (current_node->word_id == id) {
		if(vec_size == 1)
			word1_pos = current_node;
		if (vec_size == 2)
			word2_pos = current_node;
		current_node->freq++;
	}
	else {
		//insert_node_father->children->Insert(current_node, id);
		if (vec_size == 1) {
			root->children->Insert(current_node, id);
			word1_pos = current_node->sibling;
		}
		if (vec_size == 2) {
			word1_pos->children->Insert(current_node, id);
			word2_pos = current_node->sibling;
		}
		if (vec_size == 3) {
			word2_pos->children->Insert(current_node, id);
		}
	}
}

vector<string> Ngram::wordSplit(string gram) {
	//以空格为分隔符将gram分成vector<string>
	size_t pos, str_size;

	vector<string> words_vec;
	string pattern = " ";
	gram = gram + pattern;  //查找是找空格前的词，所以最后加个空格
	str_size = gram.size();
	string temp_str;
	for (size_t i = 0; i < str_size; i++) {
		pos = gram.find(" ", i); //查找空格
		temp_str = gram.substr(i, pos - i);
		words_vec.push_back(temp_str);
		i = pattern.size() + pos - 1;
	}
	return words_vec;
}

size_t Ngram::getFreq(string gram) {
	//gram 应该为字典中词的组合，词与词之间分开
	vector<string> words_vec;
	words_vec = wordSplit(gram);

	Node *last_word_node = root;
	for (size_t i = 0; i < words_vec.size(); i++) {
		if (!last_word_node->children)
			return 0;  //不存在这玩意
		last_word_node = last_word_node->children->biSearch(vocab[words_vec[i]]);
	} 
		
	return last_word_node->freq;
};

void Ngram::readTrainFile(string filename) {
	string word, word1, word2, word3;
	ifstream fs;
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open training set." << endl;
		exit(-2);
	}
	size_t word_count = 0;
	size_t line_count = 0;
	vector<string> word_vec;
	while (fs >> word) {
		if (word == "<s>") {
			word2 = word3 = "";
			if(line_count % 100 == 0)
				cout << "reading line: " << line_count << endl;
			line_count++;
			word_count = 0;
		}
		word_count++;
		if(word_count >3){
			word1 = word2; word2 = word3; word3 = word;
		}
		if (word_count == 1)
			word1 = word;
		if (word_count == 2) 
			word2 = word;
		if (word_count == 3)
			word3 = word;
		if (word_count >= 3) {
			word_vec = { word1 };
			Insert(word_vec);
			word_vec = { word1, word2 };
			Insert(word_vec);
			word_vec = { word1, word2, word3 };
			Insert(word_vec);
		}
		if (word == "</s>") {
			word_vec = { word2 };
			Insert(word_vec);
			word_vec = { word2, word3 };
			Insert(word_vec);
			word_vec = { word3 };
			Insert(word_vec);
			word1_pos = word2_pos = nullptr;
		}

		//word1 = word2; word2 = word3; word3 = word;
		//word_vec = { word3 };
		//Insert(word_vec);
		//if (word_count > 1) {
		//	word_vec = { word2, word3 };
		//	Insert(word_vec);
		//}
		//if (word_count > 2) {
		//	word_vec = { word1,word2,word3 };
		//	Insert(word_vec);
		//}
	}
};

void Ngram::readVocab(string filename) {
	ifstream fs;
	string word;
	size_t id = 1; //ID=1为<UNK>
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open vocab file." << endl;
		exit(-2);
	}
	vocab.insert(map<string, size_t>::value_type("<unk>", 1));  //<unk>的id为0
	//Node *current_node = root->children->head;
	//root->children->Insert(current_node, id);
	//current_node->freq = 0;
	while (fs >> word) {
		id++;
		vocab.insert(map<string, size_t>::value_type(word, id));
		//current_node = current_node->sibling;    //
		//root->children->Insert(current_node, id);
		//current_node->freq = 0;
	}
	//current_node->sibling->freq = 0;

	for (auto itr = vocab.begin(); itr != vocab.end(); itr++) {
		id_vocab.insert(map<size_t, string>::value_type(itr->second, itr->first));
		//建立id - word之间的关系
	}

	//
}

void Ngram::writeFreq(string filename) {
	ofstream fs;
	fs.open(filename);
	if (!fs) {
		cerr << "Can't open the write file." << endl;
		exit(-3);
	}
	cout << "-------------------------------------------------" << endl;
	cout << "||||||||||||||strat writing file||||||||||||||" << endl;

	fs << "\\data\\" << endl;
	fs << "Ngram 1 = " << root->children->list_size << endl << endl;

	Node *gram1 = root->children->head;
	fs << "\\1-grams:" << endl;
	for (size_t i = 0; i < root->children->list_size; i++) {
		gram1 = gram1->sibling;
		if (gram1 != root->children->tail) {
			fs << id_vocab[gram1->word_id] << "\t\t" << gram1->freq << endl;
		}
	}

	gram1 = root->children->head;
	fs << endl << "\\2-grams:" << endl;
	for (size_t i = 0; i < root->children->list_size; i++) {
		gram1 = gram1->sibling;
		if (gram1 != root->children->tail) {
			Node *gram2 = gram1->children->head;
			for (size_t j = 0; j < gram1->children->list_size; j++) {
				gram2 = gram2->sibling;
				if (gram2 != gram1->children->tail) {
					fs << id_vocab[gram1->word_id] <<" "<< id_vocab[gram2->word_id]
						<< "\t\t" << gram2->freq << endl;
				}
			}
		}
	}

	gram1 = root->children->head;
	fs << endl << "\\3-grams:" << endl;
	for (size_t i = 0; i < root->children->list_size; i++) {
		gram1 = gram1->sibling;
		if (gram1 != root->children->tail) {
			Node *gram2 = gram1->children->head;
			for (size_t j = 0; j < gram1->children->list_size; j++) {
				gram2 = gram2->sibling;
				if (gram2 != gram1->children->tail) {
					Node *gram3 = gram2->children->head;
					for (size_t m = 0; m < gram2->children->list_size; m++) {
						gram3 = gram3->sibling;
						if (gram3 != gram2->children->tail) {
							fs << id_vocab[gram1->word_id]<<" "<<id_vocab[gram2->word_id]<<" "<< id_vocab[gram3->word_id]
								<< "\t\t" << gram3->freq << endl;
						}
					}					
				}
			}
		}
	}//3gram

	cout << "----------------------DONE------------------------" << endl;
}

#endif // NGRAM_H

