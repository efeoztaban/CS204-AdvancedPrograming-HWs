#ifndef _TRIE
#define _TRIE

#include <string>
#include <iostream>

using namespace std;

const int ALPHABET_SIZE = 26;

struct TrieNode {
	bool isWord;
	TrieNode * children[ALPHABET_SIZE];
	TrieNode(){
		this->isWord = false;
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			this->children[i] = nullptr;
		}
	};
};


class Trie {
	friend class TrieIterator;

public:
	Trie(); // Constructor
	Trie(const Trie &); // Copy constructor
	Trie(Trie &&); // Move constructor
	~Trie(); // Destructor


	bool searchWord(string word);
	void insertWord(string word);
	void deleteWord(string word);
	bool isEmpty() const;
	void deleteTrie();
	int length();

	/*******************************************************************/
	/*                      DEFINE YOUR OPERATORS HERE                 */


	bool operator == (Trie &rhs); // check if tries are equal or not
	bool operator != (Trie &rhs); // check if tries are different or not
	void operator += (Trie &rhs); // to add two tries
	void operator += (string rhs); // to add one word to a trie
	Trie& operator = (Trie& rhs); // to make trie equal to another trie
	Trie operator + (Trie& rhs); // to make an addition between two tries
	friend Trie operator + (string lhs, Trie& rhs); // to make an addition between one trie and one word
	friend Trie operator + (Trie& lhs, string rhs); // to make an addition between one trie and one word
	friend ostream& operator << (ostream& lhs, Trie& rhs); // to print all the words in a trie one by one


	/*******************************************************************/

	//implement this function
	TrieNode * createClone() const;

private:
	TrieNode * root;
	int size; // update size at every insertion and deletion
			  // so that, every time length function called 
			  // don't iterate all list.


	// helper function
	void lower(string & word);
	bool deleteWordHelper(TrieNode *& , string);
	bool hasChildren(TrieNode * node, int) const;
	void printHelper(ostream & os, TrieNode * node, string & word) const;
	void deleteTrieHelper(TrieNode * node);


	//complete the implementation of this function
	void cloneHelper(TrieNode* source, TrieNode*& destination) const;

};
#endif
