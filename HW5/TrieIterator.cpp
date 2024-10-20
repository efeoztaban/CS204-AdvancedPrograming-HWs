#include "Stack.h"
#include "Trie.h"
#include "TrieIterator.h"

//  these two global variable, for checking has more and checking IsWord
bool found = false;  // stops the searching when it sees IsWord as true
int counter = 0; // counts the iteration number

TrieIterator::TrieIterator(const Trie & myTrie)
	: myTrie(myTrie), currentNode(nullptr), currentWord("") {
}

// TODO: Implement this function
void TrieIterator::Init() {

	currentNode = myTrie.root;  // 
	counter=myTrie.size;    // takes the number of words in trie
	found=false;           // it makes found for new iterating operation
	InitHelper();		  // calls its helper function

}
// TODO: Implement this function

void TrieIterator::Next() {

	found=false;  // makes false for a new word finding operation
	NextHelper();
}
// TODO: Implement this function

bool TrieIterator::HasMore() const {

	counter--;  
	return counter>=0; // if all words in the trie is printed then it stops iteration
					   // if not it allows operation to continue

}
// TODO: Implement this function
string TrieIterator::Current() const {

	return currentWord.substr(0,currentWord.length()-1);   // retuns the last word found
															// the every word found has " " at the end because the operation 
															// starts with a letter pop. So it prevent last letter to pop
} 

void TrieIterator::InitHelper() {
											// it finds the first word in the trie
	
	for(int i = 0; i < ALPHABET_SIZE; i++)
	{
		if(found)   // if the word is found operation ends
		{
			return;
		}
		if(currentNode->children[i])  // it search for the isWord true , at the same time it collect letters to create word
		{
			currentWord += char( i + 'a' );
			stack.push(currentNode,i);

			if(currentNode->children[i]->isWord)
			{
				currentNode= currentNode->children[i];
				stack.push(currentNode,-1);
				currentWord += " ";
				found=true;
				return;
			}
			else
			{
				currentNode = currentNode->children[i];
				InitHelper();
			}
		}
	}

}

void TrieIterator::NextHelper() {

	int last_index;
	stack.pop( currentNode, last_index);           // pop the last node to go back in the trie                 
	last_index += 1;
	currentWord= currentWord.substr(0,currentWord.length()-1);
	if (counter==0)         // if it is the last time for searching it cleans the stack for another operation
	{
		found=true;
		TrieNode* deleter_ptr;
		int deleter_int;
		while( !stack.isEmpty() )
		{
			stack.pop(deleter_ptr, deleter_int);
		}
	}

	for(int i = last_index; i < ALPHABET_SIZE; i++)  // it finds the next word like in the init function
	{
		if(found)
		{
			return;
		}
		if(currentNode->children[i])
		{
			currentWord += char( i + 'a' );
			stack.push(currentNode,i);                  // add the node and the index to continue the searching from where it left of
			currentNode= currentNode->children[i];
			stack.push(currentNode,-1);           

			if(currentNode->isWord)
			{
				currentWord += " ";
				found=true;
				return;
			}
			else
			{
				currentWord += " ";
				NextHelper();
			}
		}
	}
	if(!found)  // if there is no word in front, then it go back in the trie. It calls itself for this operation becuase
				// when the function starts it pops the last node
	{
		NextHelper();
	}


}