#ifndef _ITERATOR
#define _ITERATOR

#include "Stack.h"

class TrieIterator {
public:

	TrieIterator(const Trie & myTrie);

	void Init();
	bool HasMore() const;
	string Current() const;
	void Next();

private:
	const Trie & myTrie;
	TrieNode* currentNode;
	Stack stack;
	string currentWord;


	void InitHelper(); // helper function for init function
	void NextHelper(); // helper function for next function
};
#endif