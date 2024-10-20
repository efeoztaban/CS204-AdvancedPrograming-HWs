// EFE OZTABAN 25202 CS204 HW1

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct num_of_occurance       // this is a struct to save the words in the matrix and their number of occurances 
{
	string word;
	int num;
};

bool reading_file(vector<vector<char>> & main_matrix, int & word_length);
void finding_words(vector<vector<char>> & main_matrix,vector<num_of_occurance> & word_list, int word_length);
void adding_words(vector<num_of_occurance> & word_list, string word);
void writing_output(vector<num_of_occurance> & word_list);
void recursive_finder(int x_pos, int y_pos,vector<char> & temp_word, vector<vector<int>> & previous_cells, vector<num_of_occurance> & word_list, int word_length, int matrix_size, vector<vector<char>> & main_matrix );
bool checker(int x, int y, vector<vector<int>> & previous_cells);

int main()
{
	vector<vector<char>> main_matrix;   // this is the vector of vectors for the save giving matrix
	vector<num_of_occurance> word_list;  // this is the vector to save the words find in the matrix
	int word_length;  // this the the length of words which are searched by the algorithm

	if (reading_file(main_matrix, word_length))  // firstly input file is read
	{											
												//if the reading process is successful

		finding_words(main_matrix,word_list,word_length);  // it search for the words in the matrix
		writing_output(word_list); // then it prints the words which it found
		return 0;
	}
	else // if reading process is not successful it prints an error message
	{
		cout << "Input file is not as expected." << endl;
		return 0;
	}
	
}

bool reading_file(vector<vector<char>> & main_matrix, int & word_length)  // this function takes empty matrix vector and word length integer as parameter
{																		  // it reads the input file and fill the main matrix vector and takes word length
	string file_name, temp_line;										  // it returns a boolean value for the success of the file reading process
	int matrix_size;
	ifstream reader;
	vector<char> temp_matrix;

	cout<< "Please enter the input file name:" ;
	cin >> file_name;

	reader.open(file_name.c_str());
	while( reader.fail() )
	{
		cout << "Couldn’t open file. Please enter again:" ;
		cin >> file_name;
		reader.open(file_name.c_str());
	}

	reader >> matrix_size;          // firstly it takes the size of the matrix from file
	getline(reader, temp_line);		
	temp_line.clear();

	for(int i=0; i<matrix_size; i++)
	{
		getline( reader, temp_line );
		if( temp_line.length() == matrix_size)      // then it controls all the lengths of the lines to check row number
		{   
			stringstream ss(temp_line);
		
			for(int j=0; j< matrix_size; j++)
			{
				char ch;
				ss.get(ch);
				if( ch <='Z' && ch >= 'A')         // then it controls all the characters in the each line to check if the
												   //elements in the row are letters or not
				{
					temp_matrix.push_back(ch);
					
				}
				else
					return false;
			}
		}
		else
			return false;

		main_matrix.push_back(temp_matrix);
		temp_matrix.clear();
		temp_line.clear();
	}

	getline( reader, temp_line);
	if ( temp_line == "")                 // it controls the location of the empty line to check column size
	{
		reader >> word_length;           // then it takes the word length from the end of the input file
		return true;
	}

	else
		return false;
}

void finding_words(vector<vector<char>> & main_matrix,vector<num_of_occurance> & word_list, int word_length) 
{																		// this funtion takes matrix vector, word list vector and word length as parameters
	int matrix_size = main_matrix.size();							    // and it finds all the words in the main vector whichs length is equeal to wordlength integer
																		// and it saves that worda in the word list vector
	for(int x=0; x<matrix_size; x++) // this x is the x position for the search
	{
		for(int y=0; y<matrix_size; y++) // this y is the y position for the search
		{
			vector<char> temp_word;
			vector<vector<int>> previous_cells;
			
			temp_word.push_back(main_matrix[y][x]);

			vector<int> temp_pos;
			temp_pos.push_back(x);
			temp_pos.push_back(y);

			previous_cells.push_back(temp_pos); // this vector saves the cells which program went before to not go there again
		
			recursive_finder(x, y,temp_word, previous_cells, word_list, word_length, matrix_size, main_matrix); // this find all the words start from x,y

			temp_word.clear();
			temp_pos.clear();
			previous_cells.clear();
		}
	}
}

void adding_words(vector<num_of_occurance> & word_list, string word) // this function takes a string and word list vector as parameters
{																	 // it checks if this word exist in the word list or not
	bool existance = false;											 
																	 
	for(int i=0; i< word_list.size(); i++)
	{
		if (word == word_list[i].word)   // if the word is new for the list it creates a new element for the vector 
		{
			existance = true;
			word_list[i].num ++;
			break;
		}
	}

	if (!existance)                     // if the word exist in the list before then it increase the number of occurance of that element
	{
		num_of_occurance element;
		element.word = word;
		element.num = 1;

		word_list.push_back(element);
	}

}

void writing_output(vector<num_of_occurance> & word_list)   // this function takes the word list vector with structs as parameter
{															// it prints all the words and their number of occurances 
	cout << "Given matrix includes:" << endl;

	for(int i=0; i< word_list.size(); i++)
	{
		cout << "[+] " << word_list[i].word << " exists " << word_list[i].num << " many times." << endl;
	}
}

void recursive_finder(int x_pos, int y_pos,vector<char> & temp_word, vector<vector<int>> & previous_cells, vector<num_of_occurance> & word_list, int word_length, int matrix_size, vector<vector<char>> & main_matrix )
{														//this function takes position(x,y), main matrix, previous cell vector word list vector, 
														//word length and the matrix size as parameters
	for(int i=0; i<4; i++)								// it finds all the words which starts from x,y position and whose length is equal to word length
	{													// then send them to another function to add them in word list
		int x_previous = x_pos; // saves x position to go back
		int y_previous = y_pos; // saves y position to go back 

		if(i == 0)              // 4 moves can be done in the searching process
			x_pos--; // left move
		else if(i == 1)
			x_pos++; // right move
		else if(i == 2)
			y_pos++; // up
		else
			y_pos--; // down

		if( (x_pos>=0 && x_pos<matrix_size) && (y_pos>=0 && y_pos<matrix_size) )  // after the move it check are we still in the boundrys of the matrix
		{
			if ( checker(x_pos, y_pos, previous_cells) ) // then it checks did we go that cell before
			{
				vector<int> temp_pos;
				temp_pos.push_back(x_pos);
				temp_pos.push_back(y_pos);

				previous_cells.push_back(temp_pos);    // if it is in the boundrys and it is a new cell it saves it in the cells gone before list

				temp_pos.clear();

				temp_word.push_back(main_matrix[y_pos][x_pos]);  // and takes the letter in that cell to create the word

				if(temp_word.size() == word_length)  // if the word is the same length with the searching size
				{
					string word="";
					
					for(int el=0; el<word_length; el++)
					{
						word.push_back(temp_word[el]);
					}

					adding_words(word_list, word);  // it adds the word in word list
				}
				else // if still word is shorter then the expected
				{
					recursive_finder(x_pos, y_pos,temp_word, previous_cells, word_list, word_length, matrix_size, main_matrix); // it continue to move and search letters
				}

				previous_cells.pop_back(); // after it finds the words it go back remove that cells from preciously gone cells
				temp_word.pop_back();
			}
		}

		x_pos = x_previous; // after it finds the words it go back and seach for the new words
		y_pos = y_previous;
	}
}

bool checker(int x, int y, vector<vector<int>> & previous_cells)  // this function takes x and y positions and the previous cell vector
{																  // then it checks whether x,y position is in the previous cells or not
	for(int i=0; i< previous_cells.size(); i++)
	{
		if( previous_cells[i][0] == x )
		{
			if( previous_cells[i][1] == y )
			{
				return false;
			}
		}
	}
	return true;
}

