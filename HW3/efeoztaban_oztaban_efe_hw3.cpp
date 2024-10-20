#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "DynamicStrQueue.h"
#include "DynamicIntStack.h"
#include "DynamicStrStack.h"
#include "strutils.h"

using namespace std;

int special_regs[5] = {0};// namely CSA CSB CSC CSP CST
int general_regs[20] = {0}; // namely CSG0 CSG1 CSG2 ... CSG19
string variables[20] = {""}; // to store the variable names defined by the user. (e.g. var1, var2)
DynamicStrQueue instruction_queue; // to store the instructions in the queue
int def_counter = 0; // counting the def statements so that general reg - variable match holds
DynamicIntStack memory_stack; // needed for debugger
int instr_counter = 1; // counter for printing the instruction counts in print remaining instructions

bool is_in_variables( string name, int& num ); // this function checks is the given variable defined before
void transforming_oneline_into_machine_code( string line ); // this function transforms one line CS++ code to machine code
void making_oneline_calculations( int main_variable_index, string calculations ); // this function transforms calculations into machine code
void run_oneline_instruction( string instruction ); // this functions runs one line instruction
int special_index_finder( string reg ); // this shows the index of the special regs

void clear_regs_vars() { // this function clears all the previous instruction, memory and variable data
	def_counter = 0;
	for (int &i : special_regs) {
		i = 0;
	}
	for (int &i : general_regs) {
		i = 0;
	}
	for (string &i : variables) {
		i = "";
	}
	instruction_queue.clear();
	memory_stack.clear();
}

bool transform_source() {    //It requests an input file and if it cannot be opened returns false
							 //If the input file correctly opened, reads the file and 
							 //transform CS++ to Machine Language and stores the Machine Code instructions in `instruction_queue`
	cout << "Please give me the input filename: ";
	string file_name;
	cin >> file_name;

	ifstream reader;
	reader.open( file_name.c_str() );
	
	if( reader.fail() ) // if file don't open returns false
	{
		return false;
	}
	else
	{
		clear_regs_vars();  // when it starts to read a new file, it clears the previous data
		string line;
		getline( reader, line); // it takes the first line

		if( line == "main:" )  // if the first line is main:, it starts to take instructions line by line
		{
			while( line != "return" ) // it takes the instructions until it takes return
			{
				getline( reader, line ); // it takes the new CS++ code line 
				transforming_oneline_into_machine_code( line ); // and send to the transformation function
			}

			instruction_queue.enqueue("ret"); // it push the last machine code into inst_queue as ret
			return true; // it returns true, when file reading finishes
		}
		else // if the first line is not main: it returns false
		{
			return false;
		}
	}
}

void transforming_oneline_into_machine_code( string line ) // this function takes the CS++ code as line
{														   // and pushes these instructions to queue

	stringstream reader(line); // instructions transform to stream again
	string current_word, calculations;
	int variable_index; 

	while( reader >> current_word )
	{
		if( current_word == "int" ) // if the word is int, it creates a new variable and record its name and index
		{
			reader >> current_word;

			for( int i=0; i<20; i++)
			{
				if( variables[i] == "" )
				{
					variables[i] = current_word;
					variable_index = i;
					break;
				}
			}
		}

		else if ( current_word == "print" ) // if the word is print it pushes the intructions to queue
		{
			reader >> current_word;
			is_in_variables(current_word, variable_index);
			string instructions;
			instructions = "push CSG" + itoa(variable_index);
			instruction_queue.enqueue(instructions);
			instruction_queue.enqueue( "print" );
			break;
		}
		else if ( current_word == "=" ) // if the word is =, it takes the rest of the line and send it to calculations function with the variable index
		{
			getline( reader, calculations );
			making_oneline_calculations( variable_index, calculations );
			break;
		}
		else // if the word is a variable name, it finds the variable and record its index
		{
			for( int i=0; i<20; i++)
			{
				if( variables[i] == current_word )
				{
					variable_index = i;
				}
			}
		}
	}
}

void making_oneline_calculations( int main_variable_index, string calculations )
{													// this function transform CS++ code calculations into machine code
													// it takes the rest of the code after '=' sign and transform it
	
	DynamicStringStack computation; // this stack saves the computation instructions word by word
	stringstream seeker(calculations);
	string current_character, character1, character2, character3, character_empty, instruction ;
	int index1, index3;

	while( seeker >> current_character )
	{
		if( current_character == ")" ) // if the end of the parantesis, it pops last 4 element in the computation stack
		{							   // then it makes the calculations and machine code transformation
									   // then it pushes new instructions into queue and push the result to computation stack
			computation.pop(character3);
			computation.pop(character2);
			computation.pop(character1);
			computation.pop(character_empty);

			// it finds the values of the element1 and element3 which are the elements of computation

			if( (character3 == "CSA" || character3 == "CSB"|| character3 == "CSC" )	&& (character1 != "CSA" && character1 != "CSC" && character1 != "CSB"  ) )
			{
				instruction_queue.enqueue("pop");
				character3 = "CSP";
			}
			if( ( character3 != "CSA" && character3 != "CSC" && character3 != "CSB" ) && ( character1 == "CSA" || character1 == "CSB"|| character1 == "CSC"  ) )
			{
				instruction_queue.enqueue("pop");
				character1 = "CSP";
			}
			if( (character3 == "CSA" || character3 == "CSB"|| character3 == "CSC" ) && ( character1 == "CSA" || character1 == "CSB"|| character1 == "CSC"  )  )
			{
				instruction_queue.enqueue("pop");
				instruction_queue.enqueue("mov CST CSP");
				instruction_queue.enqueue("pop");

				character3 = "CST";
				character1 = "CSP";
			}

			if( is_in_variables(character1,index1) )
			{
				character1 = "CSG" + itoa(index1);
			}
			if( is_in_variables(character3,index3) )
			{
				character3 = "CSG" + itoa(index3);
			}
	
			if( character2 == "+" ) // if adding
			{
				instruction = "add " + character1 + " " + character3;
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSA");
				computation.push("CSA");
			}
			else if( character2 == "-" ) // if substracting
			{
				instruction = "sub " + character1 + " " + character3;
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSA");
				computation.push("CSA");
			}
			else if( character2 == "*" ) // if multiplication
			{
				instruction = "mull " + character1 + " " + character3;
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSA");
				computation.push("CSA");
			}
			else if( character2 == "/" ) // if division
			{
				instruction = "div " + character1 + " " + character3;
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSB");
				computation.push("CSB");
			}
			else if( character2 == "%" ) // if modula operation
			{
				instruction = "div " + character1 + " " + character3;
				instruction_queue.enqueue(instruction);
				instruction_queue.enqueue("push CSC");
				computation.push("CSC");
			}

		}
		else // if it is not end of the pararntesis, it continue to push into computation stack
		{
			computation.push( current_character );
		}
		
	}

	string answer;
	computation.pop(answer); // after all calculations it takes the result and creates the last instruction

	if( answer.substr(0,1) <= "9" && answer.substr(0,1) >= "0" )
	{
		instruction = "mov CSG" + itoa(main_variable_index) + " " + answer;
		instruction_queue.enqueue(instruction);
	}
	else if( answer == "CSA" || answer == "CSB" || answer == "CSC" )
	{
		instruction_queue.enqueue("pop");
		answer = "CSP";
		instruction = "mov CSG" + itoa(main_variable_index) + " " + answer;
		instruction_queue.enqueue(instruction);
	}
	else
	{
		int index_ans;
		is_in_variables(answer, index_ans);
		instruction = "mov CSG" + itoa(main_variable_index) + " CSG" + itoa(index_ans);
		instruction_queue.enqueue(instruction);
	}
}

bool is_in_variables( string name, int& num ) // this function checks if the variable defined before or not
{
	for( int i=0; i<20; i++ )
	{
		if( variables[i] == name )
		{
			num = i;
			return true;
		}
	}

	return false;
}



void run_one_instruction() {//dequeue one instruction from `instruction_queue` and run it accordingly

	if( !instruction_queue.isEmpty() )
	{
		string instruction = "";
		instruction_queue.dequeue(instruction); // takes the current instruction
		cout<< endl << "Executed instruction: " << instruction << endl;
		run_oneline_instruction( instruction ); // and send it to instruction run function
	}
}

void run_until_end() {//run all the instructions in the queue

	if( instruction_queue.isEmpty() )
	{
		cout<< "There is no instruction left." << endl;
	}
	else
	{
		string instruction;
		instruction_queue.dequeue(instruction); // takes the current instruction

		while( instruction != "ret" ) // and send it to run function until it takes ret instruction
		{
			run_oneline_instruction( instruction );
			instruction_queue.dequeue(instruction);
		}
	}
}

void run_oneline_instruction( string instruction ) // this function takes the machine code instruction and execute that instruction
{
	if( instruction == "pop" ) // for pop instruction
	{
		int pop_item;
		memory_stack.pop(pop_item);
		special_regs[3] = pop_item;
	}
	else if( instruction == "print" ) // for print instruction
	{
		int pop_item;
		memory_stack.pop(pop_item);
		cout<< "Print Output: " << pop_item << endl;
	}
	else
	{
		string type, element1, element2;
		int index1, index2, element1_val, element2_val ;
		stringstream long_instruction(instruction);
		long_instruction >> type;
		long_instruction >> element1;
		
		if ( element1[0] >= '0' && element1[0] <= '9' )
		{
				element1_val = atoi(element1);
		}
		else if( element1[0] == 'C' )
		{
			
			if( element1[2] == 'G' )
			{
				index1 = atoi( element1.substr(3,1) );
				element1_val =  general_regs[index1];
			}
			else
			{
				index1 = special_index_finder( element1 );
				element1_val = special_regs[index1];
			}
		}
		if( type != "push" ) // for push instruction
		{
			long_instruction >> element2;
			if ( element2[0] >= '0' && element2[0] <= '9' )
			{
				element2_val = atoi(element2);
			}

			else if( element2[0] == 'C' )
			{
				if( element2[2] == 'G' )
				{
					index2 = atoi( element2.substr(3,1) );
					element2_val =  general_regs[index2];
				}
				else
				{
					index2 = special_index_finder( element2 );
					element2_val = special_regs[index2];
				}
			}

			if( type == "mov") // for move instruction
			{
			
				if( element1[2] == 'G' )
				{
					general_regs[index1] = element2_val;
				}
				else
				{
					index1 = special_index_finder( element1 );
					special_regs[index1] = element2_val;
				}

			}
			else if( type == "add") // for adding instruction
			{
				int sum = element1_val + element2_val;
				special_regs[0] = sum;
			}
			else if( type == "sub") // for substraction instruction
			{
				int subs = element1_val - element2_val;
				special_regs[0] = subs;
			}
			else if( type == "mull") // for multiplication instruction
			{
				int mult = element1_val * element2_val;
				special_regs[0] = mult;
			}
			else if( type == "div") // for division instruction
			{
				int divi = element1_val / element2_val;
				int reminder = element1_val % element2_val;

				special_regs[1] = divi;
				special_regs[2] = reminder;
			}
		}
		else // for push instruction
		{
			if( element1[2] == 'G' )
			{
				memory_stack.push(element1_val);
			}
			else
			{
				memory_stack.push(element1_val);
			}
		}
	}
}

int special_index_finder( string reg ) // this function finds the index of the special regis
{
	if( reg == "CSA")
	{
		return 0;
	}
	else if( reg == "CSB")
	{
		return 1;
	}
	else if( reg == "CSC")
	{
		return 2;
	}
	else if( reg == "CSP")
	{
		return 3;
	}
	else if( reg == "CST")
	{
		return 4;
	}
	
}


void print_stack() {  //print the values in the `memory_stack`

	if( memory_stack.isEmpty() )
	{
		cout<< "Currently stack is empty." << endl;
	}
	else
	{
		DynamicIntStack temp_memory_stack;
		int memory;

		cout<<endl <<"	Current Stack" << endl << endl;

		while( !memory_stack.isEmpty() ) // emptying the memory stack into the temporary stack 	
		{								 //and printing the memory at the same time
			memory_stack.pop(memory);
			cout << "	     " << memory << endl;
			temp_memory_stack.push(memory);
		}
		while( !temp_memory_stack.isEmpty() ) // fill the memory stack again from temporary stack
		{
			temp_memory_stack.pop(memory);
			memory_stack.push(memory);
		}

		temp_memory_stack.clear();
	}

}


void print_registers() {   //print the registers

	cout<< "			" << "SPECIAL REGISTERS" << endl << endl;

	cout<<"				" << "CSA: " << special_regs[0] << endl;
	cout<<"				" << "CSB: " << special_regs[1] << endl;
	cout<<"				" << "CSC: " << special_regs[2] << endl;
	cout<<"				" << "CSP: " << special_regs[3] << endl;
	cout<<"				" << "CST: " << special_regs[4] << endl <<endl;

	cout<< "			" << "GENERAL USE REGISTERS" << endl << endl;

	for(int i=0; i<20; i++)
	{
		cout<<"				" << "CSG" << i <<": " << general_regs[i] << endl;
	}

}


void print_next_instruction() {//print the next Machine Code instruction to be executed

	DynamicStrQueue temp_instruction_queue ;
	string instruction;
	
	if( instruction_queue.isEmpty() )
	{
		cout<< "There is no instruction left." << endl;
	}
	else // if there is still instruction in the queue
	{
		instruction_queue.dequeue(instruction);

		cout<< endl << "Next Instruction is: " << instruction << endl; // print the next instruction

		temp_instruction_queue.enqueue(instruction); // save it into temporary queue
		
		while( !instruction_queue.isEmpty() )  // empty all the instructions from the instruction queue to temporary queue
		{
			instruction_queue.dequeue(instruction);
			temp_instruction_queue.enqueue(instruction);
		}
		while( !temp_instruction_queue.isEmpty() ) // fill the instruction queue again from the temporary queue
		{
			temp_instruction_queue.dequeue(instruction);
			instruction_queue.enqueue(instruction);
		}
		temp_instruction_queue.clear();
	}
}


void print_remaining_instructions(int instr_counter) {//print the remaining instructions to execute in the `instruction_queue`

	DynamicStrQueue temp_instruction_queue;
	string instruction;
	
	if( instruction_queue.isEmpty() )
	{
		cout<< endl << "There is no instruction left." << endl;
	}
	else
	{
		cout<< endl << "******** INSTRUCTIONS ********" << endl;
		int num=1;

		while( !instruction_queue.isEmpty() ) // empty all the instruction queue to temporary queue
		{									  // at the same time it prints all the instructions
			instruction_queue.dequeue(instruction);
			cout<<"["<<num<<"] "<< instruction << endl;
			num++;
			temp_instruction_queue.enqueue(instruction);
		}

		while( !temp_instruction_queue.isEmpty() ) // it fills isntruction queue again
		{
			temp_instruction_queue.dequeue(instruction);
			instruction_queue.enqueue(instruction);
		}
	}
	temp_instruction_queue.clear();
}


void print_variables() {          //print the defined variables and their values

	cout<< "******** DEFINED VARIABLES ********" << endl;

	for(int i=0; i<20; i++)  // for all 20 variables prints the names and values
	{
		if( variables[i] == "")
		{
			break;
		}
		else
		{
			cout <<"		" << variables[i] << " :" << general_regs[i] << endl;
		}
	}
}


int main() {
	bool input_OK = false;
	while (true) {
		cout << endl;
		cout << "***********************************************************************" << endl
		  	 << "**************** 0 - EXIT PROGRAM                          ************" << endl
			 << "**************** 1 - GIVE INPUT FILE                       ************" << endl
			 << "**************** 2 - RUN UNTIL THE END                     ************" << endl
			 << "**************** 3 - RUN ONE INSTRUCTION                   ************" << endl
			 << "**************** 4 - PRINT CURRENT STACK                   ************" << endl
			 << "**************** 5 - PRINT REGISTER VALUES                 ************" << endl
			 << "**************** 6 - PRINT NEXT INSTRUCTION                ************" << endl
			 << "**************** 7 - PRINT REMAINING INSTRUCTIONS          ************" << endl
			 << "**************** 8 - PRINT DEFINED VARIABLES               ************" << endl
			 << "***********************************************************************" << endl;
		cout << endl;
		int option;
		cout << "Pick an option from above (int number from 0 to 8): ";
		cin >> option;
		switch (option)
		{
		case 0:
			cout << "PROGRAM EXITING ... " << endl;
			system("pause");
			exit(0);
		case 1:
			if (transform_source()) {
				input_OK = true;
			}
			else {
				cout << "There is a problem with the input file." << endl;
			}
			break;
		case 2:
			if (input_OK) {
				run_until_end();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 3:
			if (input_OK) {
				run_one_instruction();
				print_next_instruction();
			}
		else {
				cout << "First give an input file." << endl;
			}
			break;
		case 4:
			if (input_OK) {
				print_stack();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 5:
			if (input_OK) {
				print_registers();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 6:
			if (input_OK) {
				print_next_instruction();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 7:
			if (input_OK) {
				print_remaining_instructions(instr_counter);
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		case 8:
			if (input_OK) {
				print_variables();
			}
			else {
				cout << "First give an input file." << endl;
			}
			break;
		default:
			cout << "INVALID OPTION!!! Try again" << endl;
		}
	}

	return 0;
}