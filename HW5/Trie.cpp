#include "Trie.h"
#include "TrieIterator.h"
#include <iostream>
#include <sstream>
#include <vector> // add this class for "==" operator

Trie::Trie(){
	this->root = new TrieNode();
	this->size = 0;
}

 //TODO: Implement this function
Trie::Trie(const Trie & copy){

	cout << "Copy constructor called" << endl;

	this->root = copy.createClone(); // it creates a clone and make root equal with that clone
	this->size = copy.size; // it makes size equal with the copy's size
}

// TODO: Implement this function
Trie::Trie(Trie && move):root(nullptr), size(0){

	cout << "Move constructor called" << endl;
												// it creates an empty trie and swaps it's root and size with copy	
	root = move.root; 
	move.root = nullptr;

	size = move.size;
	move.size = 0;

}

// TODO: Implement this function
Trie::~Trie(){

	//cout << "Destructor called" << endl;

	deleteTrie(); // it uses pre-defined delete function
}


// TODO: Implement this function
TrieNode * Trie::createClone() const {

	TrieNode* new_trie;                   // creates new trie and its pointer 
	TrieNode* temp = root;

	cloneHelper(temp, new_trie);          // and copy all the data from previous trie to new trie
	
	return new_trie;

}

// TODO: Implement this function
void Trie::cloneHelper(TrieNode* source, TrieNode*& destination) const {

	if( source )  // if the source is nat nullpointer
	{
		destination = new TrieNode;                 // creates a new node for the new trie and copy the data
		destination->isWord = source ->isWord;
	}

	for( int i=0; i< ALPHABET_SIZE; i++)           // it checks for the every index of the array and
	{											   // if any of the index is not equal to nullptr it calls itself again
		if( source->children[i] )
		{
			cloneHelper( source->children[i] , destination->children[i] );
		}
	}
	
}


/*******************************************************************/
/*                      DEFINE YOUR OPERATORS HERE                 */

bool Trie::operator == (Trie &rhs)
{
	if( (*this).size == rhs.size )           // if their size is equal
	{
		vector<string> left_words, right_words;      // create to two vectors to save words

		TrieIterator iter_l( *this );
	
		for (iter_l.Init(); iter_l.HasMore(); iter_l.Next())   // fills the first vector with first trie's data by iterating it
		{
			left_words.push_back( iter_l.Current() );
		}
		TrieIterator iter_r( rhs );
	
		for (iter_r.Init(); iter_r.HasMore(); iter_r.Next()) // fills the second vector with first trie's data by iterating it
		{
			right_words.push_back( iter_r.Current() );
		}

		for( int i = 0; i< right_words.size()-1; i++)  // then checks the equality of the every word step by step
		{
			if( right_words[i] != left_words[i] )
			{
				return false; // if the words are different
			}
		}
		return true; // if every word is same
	}
	else
	{
		return false; // if the sizes are different than tries cannot be equal
	}
}
bool Trie::operator != (Trie &rhs)
{
	return !(*this == rhs); // uses the negation of "==" operator
}

void Trie::operator += (Trie &rhs)
{
	TrieIterator iter_r( rhs );
	
	for (iter_r.Init(); iter_r.HasMore(); iter_r.Next())  // adds all the words in trie2 to trie1 by iterating in trie2
	{
		(*this).insertWord( iter_r.Current() );
	}
}
void Trie::operator += (string rhs)
{
	(*this).insertWord( rhs ); // add only a string to the trie
}
Trie& Trie::operator = (Trie& rhs)
{

	if( this != &rhs) 
	{
		deleteTrie();  // delete old information
		root = rhs.createClone();  // then copy the information in the right hand side
		size = rhs.size;
	}
	return (*this);  // then return the result for cascading operation
}
Trie Trie::operator + (Trie& rhs)
{
	Trie result;           // creates a new trie and adds both rhs and lhs tries

	result += rhs;    
	result += (*this);

	return result;
}

ostream& operator << (ostream& lhs, Trie& rhs)
{
	if(rhs.size >0)	        // if size is bigger than 0, then iterate over trie and print all the words
	{
		TrieIterator iter_r( rhs );
	
		for (iter_r.Init(); iter_r.HasMore(); iter_r.Next()) 
		{
			cout << iter_r.Current() << endl;
		}
	}
	else     // if size is 0, then print trie is empty
	{
		cout << "Trie is empty." << endl;
	}
	return lhs;
}

// the two free function below is for adding a new word to trie

Trie operator + (Trie& lhs, string rhs)
{
	Trie result;        

	result += rhs;
	result += lhs;

	return result;
}

Trie operator + (string lhs, Trie& rhs)
{
	Trie result;

	result += lhs;
	result += rhs;

	return result;
}
/*******************************************************************/



bool Trie::searchWord(string word) {
	lower(word);
	TrieNode * ptr = root;
	int len = word.length();
	for (int i = 0; i < len; i++) {
		int targetIndex = word.at(i) - 'a';
		if (!ptr->children[targetIndex]) {
			return false;
		}
		ptr = ptr->children[targetIndex];
	}
	if (ptr->isWord) {
		return true;
	}
	return false;
}

void Trie::lower(string & word) {
	string res;
	for (char c : word) {
		if (c >= 'A' && c < 'a') {
			res += (c - 'A' + 'a');
		}
		else {
			res += c;
		}
	}
	word = res;
}


void Trie::insertWord(string word) {
	lower(word);
	if (!searchWord(word)) {
		TrieNode * ptr = root;
		int len = word.length();
		for (int i = 0; i < len; i++) {
			int targetIndex = word.at(i) - 'a';
			if (!ptr->children[targetIndex]) {
				ptr->children[targetIndex] = new TrieNode();
			}
			//			ptr->isLeaf = false;
			ptr = ptr->children[targetIndex];
		}
		ptr->isWord = true;
		size += 1;
	}
}

bool Trie::hasChildren(TrieNode * node, int i = 0) const {
	if (node) {
		for (i; i < ALPHABET_SIZE; i++) {
			if (node->children[i]) {
				return true;
			}
		}
	}
	return false;
}

void Trie::deleteWord(string word) {
	lower(word);
	if (searchWord(word)) {
		if (deleteWordHelper(root, word)) {
			size--;
		}
	}
}

bool Trie::isEmpty() const {
	return !(this->hasChildren(root));
}

bool Trie::deleteWordHelper(TrieNode *& node, string word) {
	if (word.length()) {
		if (node != nullptr &&
			node->children[word[0] - 'a'] != nullptr &&
			deleteWordHelper(node->children[word[0] - 'a'], word.substr(1)) &&
			!node->isWord
			) {
			if (!hasChildren(node)) {
				delete node;
				node = nullptr;
				return true;
			}
			else {
				node->isWord = false;
				return true;
			}

		}
	}
	if (word.length() == 0 && node->isWord) {
		if (!hasChildren(node)) {
			delete node;
			node = nullptr;
			return true;
		}
		else {
			node->isWord = false;
			return false;
		}
	}
}

void Trie::printHelper(ostream & os, TrieNode * node, string & word) const {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (node->children[i]) {
			word += char(i + 'a');
			if (node->children[i]->isWord) {
				os << word << endl;
				printHelper(os, node->children[i], word);
			}
			else {
				printHelper(os, node->children[i], word);
			}
		}
		else if (i == 26) {
			word = word.substr(0, word.length() - 1);
		}
	}
	word = word.substr(0, word.length() - 1);
}

void Trie::deleteTrie() {
	if (root) {
		deleteTrieHelper(root);
		root = nullptr;
		size = 0;
	}
}

void Trie::deleteTrieHelper(TrieNode * node) {
	if (hasChildren(node)) {
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			if (node->children[i]) {
				deleteTrieHelper(node->children[i]);
			}
		}
	}
	delete node;
}

int Trie::length() {
	return size;
}