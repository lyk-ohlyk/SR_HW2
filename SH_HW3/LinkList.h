#pragma once
#ifndef LINKLIST_H
#define LINKLIST_H
#include <iostream>
using namespace std;
class LinkList;

class Node {
public:
	unsigned int word_id, freq;
	float prob, bow;
	Node *sibling;
	LinkList* children;

	Node() {
		word_id = 0; freq = 0; bow = 0; sibling = nullptr; //children = new LinkList;
	};
	~Node() {
		delete sibling; delete children;
	}
};
class LinkList {
public:
	size_t list_size;
	Node *head;
	Node *tail;
	LinkList() { 
		head = new Node;  //list head is useless
		list_size = 0;  //  if count <unk>, this should be 1
	} 
	~LinkList() { makeEmpty(); delete head; } 
	void makeEmpty();
	Node* getHead() const { return head; }
	Node* Locate(int i);
	Node* biSearch(unsigned int x);
	Node* Search(unsigned int id);
	bool Insert(Node* current, unsigned int &id);
};

void LinkList::makeEmpty() {
	Node *q = nullptr;  //加上初始化nullptr
	while (head->sibling != nullptr) {
		q = head->sibling;
		head->sibling = q->sibling;
		delete q;
	}
};
Node* LinkList::Locate(int i) {
	if (i < 0)  return nullptr;
	Node *current = head;
	int k = 0;
	while (current != nullptr && k < i) {
		current = current->sibling; k++;
	}
	return current;
};


Node* LinkList::Search(unsigned int id) {
	Node *current = head;
	Node *prev = nullptr;
	while (current != nullptr && id > current->word_id) {
		prev = current;
		current = current->sibling;
	}
	if (current == nullptr || current->word_id != id) return prev;
	return current;
};


Node* LinkList::biSearch(unsigned int id) {//return *list[y] where max{list[y].id|list[y].id <= id}
	Node *midNode;
	size_t high, low = 1, mid; //skip list head
	high = this->list_size;   
	while (low <= high) {
		mid = low + (high - low) / 2;
		midNode = Locate(mid);
		if (id > midNode->word_id) low = mid + 1;
		else if (id < midNode->word_id) high = mid - 1;
		else return midNode;
	}
	return Locate(high);
};

bool LinkList::Insert(Node* current, unsigned int& id) {
	if (current == nullptr) return false;
	Node* newNode = new Node;
	newNode->children = new LinkList;

	if (newNode == nullptr) { cerr << "存储分配错误" << endl; exit(1); }
	newNode->word_id = id; newNode->freq = 1; 
	newNode->sibling = current->sibling;
	current->sibling = newNode;
	list_size++;
	return true;
};

#endif