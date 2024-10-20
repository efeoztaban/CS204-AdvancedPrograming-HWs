// EFE OZTABAN 25202 CS204 HW2

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
 
using namespace std;

struct product 
{							//this struct is used to create product linkedlists for every element 	
	string prod_name;		//of the main customer linkedlist. It keeps only next pointer, so one way 
	int prod_id;			//linkedlist can be constructed with these structs. It keeps the name, id and
	int amount;				//amount of the products for every customer.
	product * next;

	product::product()
	{
		prod_id = 0;
		prod_name = "";
		amount = 0;
		next = nullptr;
	}

	product::product(int prod_id, string prod_name, int amount, product * next) 		
		:prod_id(prod_id),                 // constructer for faster definition
		prod_name(prod_name),
		amount(amount),
		next(next)
	{	
	}

};

struct customer
{							//this struct is used to create the main customer linkedlist. It keeps both 
	int cust_id;			//previous and next pointer in linkedlist, so it allows to create a double		
	string cust_name;		//linkedlist. It also keeps the head pointer for every product linkedlist of 
	customer * prev;		//every customer. It keeps the names and id of customers for every node.
	customer * next;
	product * prod;

	customer::customer()
	{
		cust_id = 0;
		cust_name = "";
		prev = nullptr;
		next = nullptr;
		prod = nullptr;
	}

	customer::customer (int cust_id, string cust_name, customer * prev, customer * next, product * prod)
		:cust_id(cust_id),                               // constructer for faster definition
		cust_name(cust_name),
		prev(prev),
		next(next),
		prod(prod)
	{	
	}

};

void open_file( customer* & head, customer* & tail );
customer* creating_customer_linkedlist(customer* & head, customer* & tail, string customer_name, int customer_id );
void creating_product_linklists(product* & prod_head, product* & prod_tail, customer* current_customer, string product_name, int product_id, int product_amount);

void print_customer_data(customer* head);
void find_customer(customer* head);
void add_customer(customer* & head);
void delete_customer(customer* & head);
void add_product(customer* head);
void delete_product(customer* head);
void list_product_owners(customer* head);

void print_spesific_customer_data(customer* p);
customer* find_customer( customer* head, int id);
customer* find_customer_place( customer* head, int id);
customer* find_customer_by_name( customer* head, string name);
product* find_product( product* head, int id);
product* find_product_place( product* head, int id);

int main()
{
	customer* head= nullptr;	// the head (for the first node) pointer of the main customer linkedlist
	customer* tail= nullptr;	// the tail (for the last node) pointer of the main customer linkedlist

	open_file( head, tail );	// this function reads the input file named "input.txt" and
								//crates customer linkedlist and product linkedlists.
	
	while (true)				// this is a switch case for the options in the programme
	{							// for every option, a computation is done in the funtions

		cout << endl;
		cout 
		<< "****************************************************************************" << endl
		<< "**************** 0 - EXIT PROGRAM	                        ************" << endl
		<< "**************** 1 - PRINT ALL CUSTOMER DATA                    ************" << endl
		<< "**************** 2 - FIND A CUSTOMER                            ************" << endl
		<< "**************** 3 - ADD A CUSTOMER                             ************" << endl
		<< "**************** 4 - DELETE A CUSTOMER                          ************" << endl
		<< "**************** 5 - ADD A PRODUCT TO A CUSTOMER                ************" << endl
		<< "**************** 6 - DELETE A PRODUCT FROM A CUSTOMER           ************" << endl
		<< "**************** 7 - LIST THE BUYERS OF A PRODUCT               ************" << endl
		<< "****************************************************************************" << endl;
		cout << endl;

		int option;
		cout << "Pick an option from above (int number from 0 to 7): ";
		cin >> option;

		switch (option)
		{
			case 0:
				cout << "PROGRAM EXITING ... " << endl;
				return 0;

			case 1:
				print_customer_data(head);
				break;

			case 2:
				find_customer(head);
				break;
			
			case 3:
				add_customer(head);
				break;

			case 4:
			delete_customer(head);
			break;

			case 5:
			add_product(head);
			break;
			
			case 6:
			delete_product(head);
			break;

			case 7:
			list_product_owners(head);
			break;

			default:
			cout << "INVALID OPTION!!! Try again" << endl;

		} 
	}	
}

void open_file( customer* & head, customer* & tail )	// this function reads the input file <nd send
{														// parameter to other two functions to create
														// linkedlists with the information

	ifstream reader;
	reader.open("input.txt");

	if( !reader.fail() )
	{
		int customer_id, product_id, product_amount;     // customer id and customer name will be used in
		string customer_name, product_name, line, word;	 // the main customer linkedlist. product id, product
		char character;									 // name and amount will be used in the product 
		reader >> word;									 // þ,nledlists.

		while( ! reader.eof() )
		{
			
			stringstream atoi(word.substr(1,word.length()-2));  // this is for transforming string into an integer
			atoi >> customer_id;
			atoi.clear();
			word.clear();

			do
			{
				word.clear();
				reader >> word;

				customer_name += (" "+ word);

			}while(word.substr(word.length()-1,1) != ":" );

			customer_name= customer_name.substr( 1, customer_name.length()-2 );

			customer* current_customer = creating_customer_linkedlist( head, tail, customer_name, customer_id ); 
			product* prod_head = nullptr;                     // this funtion takes the customer name and id
			product* prod_tail = nullptr;                     // to add one element to the customer linkedlist
															  // it retuns the last added elements pointer
			customer_name.clear();							  // to link with the product linkedlist.
			word.clear();

			do
			{
				line.clear();
				getline(reader,line);

				stringstream seeker(line);
				bool checker = true;

				seeker >> product_id;

				while( checker )
				{
					seeker.get(character);
					if( character != '(' )
					{
						product_name += character;
					}
					else
						checker = false;
				}

				checker = true;

				while( checker )
				{
					seeker.get(character);
					if( character != ')' )
					{
						word += character;
					}
					else
						checker = false;
				}

				seeker.get(character);
				stringstream atoi(word);  // this is for transforming string into an integer
				atoi >> product_amount;
				atoi.clear();
				product_name = product_name.substr(1, product_name.length()-2 );
				word.clear();

				creating_product_linklists(prod_head, prod_tail, current_customer, product_name, product_id, product_amount);
														// this function creates product linklist with
				product_name.clear();					// product name, id and amount informations
														// and attach this limklist to last added customer
				string a=(line.substr(line.length()-1,1));

			}while( character != '.' ); // one customers information is end if there is dot

            reader >> word; // this is for checking wheter we reach EOF
		}
	}
}

customer* creating_customer_linkedlist(customer* & head, customer* & tail, string customer_name, int customer_id )
{										// this funtion create and add elements to customer linkedlist
										// it takes header and tail of the linkedlist and customer informations
										// and returns the pointer of the last added alement
	customer* new_node = new customer(customer_id, customer_name, nullptr, nullptr, nullptr);
														//new node of the linkedlist (in heap memory)
	if( head == nullptr ) // if it is the first element
	{
		head = new_node;
		tail = head;
	}

	else if( (new_node->cust_id) < (head->cust_id) ) //if its id smaller than the first node
	{
		new_node -> next = head;
		head -> prev = new_node;
		head = new_node;
	}

	else if( (new_node->cust_id) > (tail->cust_id) ) //if its id larger than the last node
	{
		new_node -> prev = tail;
		tail -> next = new_node;
		tail = new_node;
	}

	else    // if its id is in the middle, it finds the right place to add in order to create a sorted list
	{
		customer* prev_node = head;
		customer* next_node = (head->next);

		while(true) // searching the right interval to add
		{
			if( ((new_node->cust_id) > (prev_node->cust_id)) && ((new_node->cust_id) < (next_node->cust_id)) )
			{
				prev_node->next = new_node;

				new_node ->prev = prev_node;
				new_node ->next = next_node;

				new_node->prev = new_node;

				break;
			}
			else
			{
				prev_node = next_node;
				next_node = next_node->next;
			}
		}
	}

	return new_node; // returns the pointer of the last added element
}

void creating_product_linklists(product* & prod_head, product* & prod_tail, customer* current_customer, string product_name, int product_id, int product_amount)
 {					//this funtion creates product linkedlists and link tham with corresponding customers
					//takes  pointer of the customer node and product informations
	 product* new_node = new product(product_id, product_name, product_amount, nullptr);

	 if( prod_head == nullptr) // if it is the first element
	 {
		 current_customer -> prod = new_node;
		 prod_head = new_node;
		 prod_tail =  prod_head;
	 }

	 else if( (new_node->prod_id) < (prod_head->prod_id) ) // if its id is smaller than the first element
	{
		current_customer ->prod = new_node;
		new_node -> next = prod_head;
		prod_head = new_node;
	}

	else if( (new_node->prod_id) > (prod_tail->prod_id) ) // if its id is larger than the last elemnt
	{
		prod_tail -> next = new_node;
		prod_tail = new_node;
	}

	else  // if its id is in the middle, it finds the right place to add in order to create a sorted list
	{
		product* prev_node = prod_head;
		product* next_node = ( prod_head->next );

		while(true)  // searching the right interval to add
		{
			if( ((new_node-> prod_id) > (prev_node-> prod_id)) && ((new_node-> prod_id) < (next_node-> prod_id)) )
			{
				prev_node->next = new_node;
				new_node ->next = next_node;

				break;
			}
			else
			{
				prev_node = next_node;
				next_node = next_node->next;
			}
		}

	}
 }

 void print_customer_data(customer* head)   // this funtion is for the 1st option in the switch case
 {											// it prints all the informstion saved
	customer* temp = head;

	while( temp != nullptr )  // passes the next node until it reachs the nullpointer(after the last node)
	{
		cout << "ID:  " << temp -> cust_id << " Name:  " << temp ->cust_name <<  endl; //customer info
		cout << "Items in the cart:" << endl;

		product* temp2 = temp ->prod;

		while( temp2 != nullptr ) // product info
		{
			cout << "Item ID:  " << temp2->prod_id << " Item name:  "<< temp2 -> prod_name <<".  Amount: "<< temp2 ->amount << endl;
			temp2 = temp2 ->next;
		}

		temp = temp -> next;

		cout << endl;
	}
 }

void print_spesific_customer_data( customer* p ) // this funtion is for printing the informstion of
{                                                // a spesific costumer
	cout << "ID:  " << p -> cust_id << " Name:  " << p ->cust_name <<  endl;
	
	product* temp = p ->prod;

	while( temp != nullptr )
	{
		cout << "Item ID:  " << temp->prod_id << " Item name:  "<< temp -> prod_name <<".  Amount: "<< temp ->amount << endl;
		temp = temp ->next;
	}
}
void find_customer(customer* head)  // this funtion is the 2nd option of switch case
{									// it finds a spesific customer and prints its information
	cout << "Enter customer's ID or name and surname: ";
	string answer;

	getline( cin, answer );  // this is for clearing the cin
	getline( cin, answer );

	bool checker = false;
	customer* current = head;

	if( answer.substr(0,1) >= "0" && answer.substr(0,1) <= "9" )  //checks wheter it is an integer or string
	{
		stringstream atoi(answer);
		int id_finder;
		atoi >> id_finder;
		atoi.clear();

		customer* cp = find_customer(head, id_finder);  // search by customer id
		if (cp != nullptr)
		{
			cout << "Customer found!" << endl; 
			print_spesific_customer_data(cp);  // prints the information
			checker = true;
		}
	}
	else
	{
		string name_finder = answer;

		customer* cp = find_customer_by_name(head, name_finder);  // serach by customer name

		if (cp != nullptr)
		{
			cout << "Customer found!" << endl; 
			print_spesific_customer_data(cp);  // prints the informstion
			checker = true;
		}
	}

	if( checker == false )  // if it could not find
	{
		cout << "CUSTOMER DOESN'T EXIST. GOING BACK TO MAIN MENU" << endl;
	}
}



void add_customer(customer* & head)  // this funtion is the 3rd option of switch case
{                                    // it adds a new customer node to the linkedlist

	cout << "Enter an ID for the user: ";
	int new_id ;
	string new_name;
	cin >> new_id;

	if( head != nullptr ) // if the linkedlist not empty
	{
		customer* cp = find_customer( head, new_id);

		if( cp != nullptr) // if it is already exists
		{
			cout << "User with ID: " << new_id << " already exixts. Going back to main menu." << endl;
		}
		else  // if it is new
		{
			cout << "Enter the customer's name and surname: " ;

			getline( cin, new_name);  // it is for clearing the cin
			getline( cin, new_name);

			customer* new_node = new customer( new_id, new_name, nullptr, nullptr, nullptr);

			customer* previous = find_customer_place(head, new_id);

			if( previous == nullptr) // if its id is smaller than the first node
			{
				head->prev = new_node;
				new_node->next = head;
				head = new_node;
			}
			else
			{
				if( previous->next != nullptr)  // if it is in the middle
				{
					previous->next->prev = new_node;
					new_node->next = previous->next;
					new_node->prev = previous;
					previous->next = new_node;
				}
				else // if it is in the end
				{
					previous ->next = new_node;
					new_node ->prev = previous;
				}
			}

			cout << "User with ID: " << new_id << " is inserted to the list." << endl;
		}
	}
	else // if it is going to be the first node
	{
		cout << "Enter the customer's name and surname: " ;

		getline( cin, new_name);
		getline( cin, new_name);

		customer* new_node = new customer( new_id, new_name, nullptr, nullptr, nullptr);

		head = new_node;
	}
}

void delete_customer(customer* & head) // this funtion is the 4th option of switch case
{									   // it deletes a spesific costumer from the linkedlist
	cout << "Enter an ID for the customer to be deleted: ";
	int id_deleting;
	cin >> id_deleting;

	if( head != nullptr) // if the linklist is not empty
	{
		customer* cp = find_customer( head, id_deleting);

		if( cp == nullptr ) // if there is not an element with that id
		{
			cout << "User with ID: " << id_deleting << " doesn't exixts. Going back to main menu." << endl;
		}

		else  // if this id exists
		{
			customer* previous = find_customer_place(head, id_deleting); 
														//it finds the location of the customer

			if( cp->prev == nullptr && cp->next == nullptr ) // if it is the only node of the linkedlist
			{
				head = nullptr;
				delete cp;
			}
			else if( cp->prev == nullptr )  // if it is the first node
			{
				cp->next->prev = nullptr;
				head = cp->next;
				delete cp;
			}
			else if ( cp->next == nullptr)  // if it is the last node
			{
				cp->prev->next = nullptr;
				delete cp;
			}
			else  // if it is a node from the middle
			{
				previous->next = cp->next;
				cp->next->prev = previous;
				delete cp;
			}
	cout<< "Customer is deleted succesfully." << endl;
		}
	}
	else
	{
		cout << "User with ID: " << id_deleting << " doesn't exixts. Going back to main menu." << endl;
	}
}

void add_product(customer* head)  // this funtion is the 5th option of switch case
{
	cout << "Enter an ID for the Customer to add product: ";
	int id;
	cin >> id;

	customer* cp = find_customer(head, id);  // first it finds the customer

	if( cp == nullptr )
	{
		cout << "Customer with ID: " << id << " doesn't exixts. Going back to main menu." << endl;
	}

	else
	{
		bool checker = true;

		while(checker) // if user wants to continue
		{

			cout << "Would you like to add item to cart(Y/y): ";
			string answer;
			cin >> answer;

			if( answer == "Y" || answer== "y")
			{
				int item_id, item_amount;
				string item_name;

				cout << "ID of the item to add to cart: ";
				cin >> item_id;

				
				if( cp->prod == nullptr)  // if it is the first product for that costumer
				{
					cout << "Name of the product: ";
					cin >> item_name;
					cout << "Amount: ";
					cin >> item_amount;

					product* new_node = new product(item_id, item_name, item_amount, nullptr);

					cp->prod = new_node;
				}
				else
				{
					product* pp = find_product( cp->prod, item_id); // it finds the location of the product

					if( pp != nullptr) // if there is a product with the same id
					{
						cout << "Prouct already exists. How many would you like to add more: ";
						int add_value;
						cin >> add_value;

						pp->amount += add_value;

						cout << "Product's amount in the cart increaded to: " << pp->amount << endl;

					}
					else // if the id is new
					{
						product* previous = find_product_place( cp->prod, item_id);

						cout << "Name of the product: ";
						cin >> item_name;
						cout << "Amount: ";
						cin >> item_amount;

						product* new_node = new product(item_id, item_name, item_amount, nullptr);

						if (previous == nullptr) // adds to beginning
						{
							new_node ->next = cp ->prod;
							cp->prod = new_node;
						}
						else // adds other than begining
						{
							new_node -> next = previous->next;
							previous->next = new_node;
						}
					}
				}
			}
			else
			{
				cout << "Going back to main menu." << endl;
				checker = false;
			}
		}
	}
}

void delete_product(customer* head) // this funtion is the 6th option of switch case
{									//delete a product from a spesific customer
	cout << "Enter an ID for the Customer to delete product: ";
	int id;
	cin >> id;

	customer* cp = find_customer(head, id);

	if( cp == nullptr ) // if the id is not valid
	{
		cout << "Customer with ID: " << id << " doesn't exixts. Going back to main menu." << endl;
	}

	else
	{
		cout << "ID of the item to delte from cart: ";
		int item_id;
		cin >> item_id;

		product* pp = find_product( cp->prod, item_id);

		if( pp == nullptr) // if the prioduct id is invalid
		{
			cout << "Product doesn't exists. Going back to main menu." << endl;
		}

		else
		{
			product* previous = find_product_place( cp->prod, item_id);

			if(previous == nullptr) //delete from begining
			{
				cp->prod = pp->next;
				delete pp;
				cout << "Product has been deleted." << endl;
			}
			else //delete from other than beginning
			{
				previous->next = pp->next;
				delete pp;
				cout << "Product has been deleted." << endl;

			}

			cout << "Current items in the cart:" << endl;
			print_spesific_customer_data( cp );
		}
	}
}

void list_product_owners(customer* head)  // this funtion is the 7th option of switch case
{										  //list all the customers and product amounts for a product id
	cout << "ID of the product to search buyers: ";
	int product_id;
	cin >> product_id;

	bool checker = false;

	customer* current = head;

	while( current->next != nullptr) // until it end
	{
		current = current->next;
	}

	while( current != nullptr) // for all customers
	{
		product* pp = find_product( current->prod , product_id ); 

		if( pp != nullptr )
		{
			if( checker == false ) // if it is the first
			{
				cout << "Buyers of the product with ID: " << product_id << " is listed below:" << endl;
				checker = true;
			}

			cout<< "Customer ID: " << current-> cust_id 
				<< " Customer name: " << current ->cust_name
				<< " and Amount: " << pp->amount <<endl;
		}

		current = current->prev;
	}

	if(checker == false) // if there is no product with that id
	{
		cout << "Product doesn't exists. Going back to main menu." << endl;
	}
}

customer* find_customer( customer* head, int id)  // it search for a spesific customer location in the linkedlist
{
	customer* current = head;

	while( current != nullptr)
	{
		if( id == current->cust_id )
		{
			return current;
		}

		current = current->next;
	}

	return current;
}

customer* find_customer_place( customer* head, int id) // if there is not the same than it finds its possible place
{
	customer* current = head ;

	if( id <= head->cust_id )
	{
		return nullptr;
	}

	while( current->next  != nullptr)
	{
		if( id > current->cust_id && id <= current->next->cust_id )
		{
			return current;
		}

		current = current->next;
	}

	return current;
}

customer* find_customer_by_name( customer* head, string name) // search the location of customer by name in the linkedlist
{
	customer* current = head;

	while( current != nullptr)
	{
		if( name == current->cust_name )
		{
			return current;
		}

		current = current->next;
	}

	return current;
}

product* find_product( product* head, int id)  // search the location of a spesific product
{
	product* current = head;

	while( current != nullptr)
	{
		if( id == (current->prod_id) )
		{
			return current;
		}

		current = current->next;
	}

	return current;
}

product* find_product_place( product* head, int id)//if there is not the same then it search possible interval
{
	product* current = head ;

	if( id <= head->prod_id )
	{
		return nullptr;
	}

	while( current->next  != nullptr)
	{
		if( id > current->prod_id && id <= current->next->prod_id )
		{
			return current;
		}

		current = current->next;
	}

	return current;
}