#include "SortedArray.h"

SortedArray::SortedArray() {
	front = nullptr;
}

//impelent copy constructor
SortedArray::SortedArray(const SortedArray & copy) {

	front = copy.createClone();  // it will create a copy of the paramater copy

}

//implement destructor
SortedArray::~SortedArray() {

	while( front != nullptr )  // it will release all the allocated memory from the heap after its scope ends
	{
		ArrayNode * temp = front;
		front = temp->right;
		delete temp;
	}

}

ArrayNode * SortedArray::getFront() const{

	return front;

}

void SortedArray::SetFront(ArrayNode * newFront){

	front = newFront;

}

// implement this function
void SortedArray::cloneHelper(ArrayNode * source, ArrayNode *& destination) const {


	destination->right = new ArrayNode( source->val );  // it will copy the information of the node from old one to new copy

}

// implement this function
ArrayNode * SortedArray::createClone() const{

	if( front == nullptr )
	{
		return nullptr;
	}

	ArrayNode *copy_front = new ArrayNode( front->val );  // this is the first node of the new copy
	
	ArrayNode *temp = front->right;
	ArrayNode *copy_temp = copy_front;

	
	while( temp != nullptr )  // for every node it will copy the information from old one to the new copy
	{ 
		cloneHelper( temp, copy_temp );
		temp = temp->right;
		copy_temp = copy_temp ->right;
	}

	return copy_front; // it will return the head pointer od the new copy
}


bool SortedArray::search(int Value)
{
	ArrayNode * tmp = front;
	while (tmp) {
		if (tmp->val == Value) {
			return true;
		}
		tmp = tmp->right;
	}
	return false;
}

void SortedArray::insert(int Value) {
	if (!search(Value)) {
		ArrayNode * tmp = front;

		ArrayNode * newNode = new ArrayNode(Value);
		if (!front) {
			front = newNode;
		}
		else if (front && Value < front->val) {
			newNode->right = front;
			front = newNode;
		}
		else {
			while (tmp->right && tmp->right->val < Value) {
				tmp = tmp->right;
			}
			newNode->right = tmp->right;
			tmp->right = newNode;
		}
	}
}

void SortedArray::deleteSortedArray() {
	ArrayNode * current = front, *next = front;
	while (current) {
		next = current->right;
		delete current;
		current = next;
	}
	front = nullptr;
}

void SortedArray::print() {
	if (!front) {
		cout << "Array is empty" << endl;
	}
	ArrayNode * tmp = front;
	while (tmp) {
		cout << tmp->val << " ";
		tmp = tmp->right;
	}
	cout << endl;
}

//implement assignment (=) and plus (+) operators


SortedArray SortedArray::operator = (SortedArray rhs)  //this operator creates a clone of the rhs 
{													   //and delete the lhs, then make lhs equal to the copy of rhs
													   // and returns lhs
	ArrayNode* new_front = rhs.createClone();
	
	deleteSortedArray();
	front = new_front;
	
	return *this;
}

SortedArray operator + (SortedArray lhs, SortedArray rhs)
{
														// this operator make a copy of lhs and add the information in the rhs into that copy
														// and returns the copy at the end
	ArrayNode* second_front = rhs.getFront();
	ArrayNode* temp= second_front;

	while( temp!= nullptr )
	{
		int temp_value = temp->val;
		if( !lhs.search(temp_value) )
		{
			lhs.insert(temp_value);
		}

		temp= temp->right;
	}
	
	ArrayNode* new_front = lhs.createClone();
	SortedArray new_array;
	new_array.SetFront( new_front );

	return new_array;
}

SortedArray operator + (SortedArray lhs, int rhs)
{														
														//this operator makes a copy of the lhs and insert rhs in it
														// and return the copy at the end
	if( !lhs.search(rhs) )
	{
		lhs.insert(rhs);
	}

	ArrayNode* new_front = lhs.createClone();
	SortedArray new_array;
	new_array.SetFront( new_front );

	return new_array;
}

SortedArray operator + (int lhs,SortedArray rhs)
{
														//this operator makes a copy of the rhs and insert lhs in it
														// and return the copy at the end
	if( !rhs.search(lhs) )
	{
		rhs.insert(lhs);
	}

	ArrayNode* new_front = rhs.createClone();
	SortedArray new_array;
	new_array.SetFront( new_front );

	return new_array;
}




