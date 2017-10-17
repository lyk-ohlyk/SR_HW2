#include "Trie_Gram_Vocab.h"

Trie* Trie::insertTrie(size_t word_id)
{
	Trie *newTrie;
	auto itr = sub_trie.find(word_id);

	if (itr == sub_trie.end()){
		newTrie = new Trie;
		sub_trie.insert(map<size_t, Trie*>::value_type(word_id, newTrie));
		return newTrie;
	}
	return itr->second;
}

Trie* Trie::findSubTrie(size_t word_id)
{
	auto itr = sub_trie.find(word_id);
	if (itr == sub_trie.end()) return nullptr;
	return itr->second;
}
