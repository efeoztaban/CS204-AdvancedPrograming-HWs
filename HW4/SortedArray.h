#ifndef _SORTEDARRAY
#define _SORTEDARRAY

#include <iostream>

using namespace std;

class ArrayNode {
public:
	int val;
	ArrayNode *right;
	ArrayNode() {};
	ArrayNode(int val) :val(val), right(nullptr) {};
};


class SortedArray {
public:
	SortedArray();

	//we need deep copy implementation for copy constructor
	//createClone and cloneHelper will do the real work
	SortedArray(const SortedArray &);

	//we need to destruct all the nodes in the linkedlist
	//complete the implementation of this
	~SortedArray();

	bool search(int Value);
	void insert(int Value);
	void deleteSortedArray();
	void print();
	ArrayNode * getFront()const;
	void SetFront(ArrayNode * newFront);

	//implement this function in the cpp file
	ArrayNode* createClone() const;

	//Define and implement = operator for assignments and cascading assignment
	SortedArray SortedArray::operator = (SortedArray rhs); 

private:
	
	//complete the implementation of this function in cpp file
	void cloneHelper(ArrayNode* source, ArrayNode*& destination) const;

	ArrayNode *front;

};
//Define and implement + operators


/*

there is 3 possibilities for the + operator
one for adding two sortedlists
and two for adding a sorted list with an integer 1) integer first, sorted array second
												 2) sorted array first, integer second

*/

SortedArray operator + (SortedArray lhs, SortedArray rhs);
SortedArray operator + (SortedArray lhs, int rhs);
SortedArray operator + (int lhs, SortedArray rhs);



#endif