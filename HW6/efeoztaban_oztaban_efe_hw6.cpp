#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

template <typename T>
class Encrypter {
private:
	const T key;
	const int nShifts;

public:
	vector<int> listFlips;
	Encrypter(T, int, vector<int>);
	T encrypt(const T decrypted);
	T decrypt(const T encrypted);
};


template<typename T>
Encrypter<T> :: Encrypter(T cripto_key, int shift_key, vector<int> key_list)
			  :key(cripto_key), nShifts(shift_key), listFlips(key_list)
{
} // this is the constructor for Encrypter class


template<typename T>
T Encrypter<T> :: encrypt( const T decrypted )
{
	
	T result = decrypted ^ key;       // this is the xor operation
	cout << "Xor result: " << result << endl;

	int mod_shift= nShifts % (sizeof(result)*8);         // this find the non_circular shifts
	result = ( result<<mod_shift ) | ( result >> (sizeof(result)*8 - mod_shift ));   //this finds the circular shifts

	cout << "Shift result: "<< result << endl;

	vector<int> newlist;
	
	for( int i : listFlips )     // this rearranges the list and deletes the numbers which is bigger than the bit size of the message
	{
		if( i < sizeof( decrypted )*8 )
		{
			newlist.push_back(i);
		}
	}

	listFlips.clear();
	
	T flip_idx = 1;                 // this is the flipping opeartion
	for( int i : newlist )
	{
		result = result ^ (flip_idx << i); // to flip a spesific bit, a bit series with only one 1 in it. 
		                                   //This 1 is on the index of flipping operation index
		                                   // then xor opeartion is done between message and that series of bits
		listFlips.push_back(i);
		flip_idx=1;
	}

	return result;
}

template<typename T>
T Encrypter<T> :: decrypt( const T encrypted )
{                  // during decrypting operartion same operations are done with encrypting but in reverse order
	T result = encrypted;                           
	
	T flip_idx = 1;
	for( int i : listFlips )
	{
		result = result  ^  ( flip_idx << i);
		flip_idx = 1;
	}

	int mod_shift= nShifts % (sizeof(result)*8);
	result = ( result>>mod_shift ) | ( result << (sizeof(result)*8 - mod_shift ));

	result = result ^ key;

	return result;
}
int main() {

	int nShifts = 12;
	vector<int> listFlips; //= { 0,1,2,3,4,9,15,30,35 };
							// this type of vector initilization does not work in my compiler 

	// that's why i added push_back's above
	listFlips.push_back(0);	
	listFlips.push_back(1);	
	listFlips.push_back(2);	
	listFlips.push_back(3);	
	listFlips.push_back(4);	
	listFlips.push_back(9);	
	listFlips.push_back(15);
	listFlips.push_back(30);	
	listFlips.push_back(35);	





	unsigned char key1 = 101;
	unsigned char chClear = 'b';
	Encrypter<unsigned char> encrypterChar(key1, nShifts, listFlips);

	cout << "Char to encrypt: " << chClear << endl;
	unsigned char encryptedCh = encrypterChar.encrypt(chClear);
	unsigned char decryptedCh = encrypterChar.decrypt(encryptedCh);

	cout << "Char encrypted: " << encryptedCh << endl;
	cout << "Char decrypted: " << decryptedCh << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterChar.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;

	unsigned short int key2 = 101;
	unsigned short int intShortClear = 500;
	Encrypter<unsigned short int> encrypterShortInt(key2, nShifts, listFlips);

	cout << "Short Int to encrypt: " << intShortClear << endl;
	unsigned short int encryptedShortInt = encrypterShortInt.encrypt(intShortClear);
	unsigned short int decryptedShortInt = encrypterShortInt.decrypt(encryptedShortInt);

	cout << "Short Int encrypted: " << encryptedShortInt << endl;
	cout << "Short Int decrypted: " << decryptedShortInt << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterShortInt.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;

	unsigned int key3 = 101;
	unsigned int intClear = 32800;
	Encrypter<unsigned int> encrypterInt(key3, nShifts, listFlips);

	cout << "Int to encrypt: " << intClear << endl;
	unsigned int encryptedInt = encrypterInt.encrypt(intClear);
	unsigned int decryptedInt = encrypterInt.decrypt(encryptedInt);

	cout << "Int encrypted: " << encryptedInt << endl;
	cout << "Int decrypted: " << decryptedInt << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterInt.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;

	unsigned long long int key4 = 101;
	unsigned long long int intLLClear = 34359738368;
	Encrypter<unsigned long long int> encrypterLLInt(key4, nShifts, listFlips);

	cout << "Long Long Int to encrypt: " << intLLClear << endl;
	unsigned long long int encryptedLLInt = encrypterLLInt.encrypt(intLLClear);
	unsigned long long int decryptedLLInt = encrypterLLInt.decrypt(encryptedLLInt);

	cout << "Long Long Int encrypted: " << encryptedLLInt << endl;
	cout << "Long Long Int decrypted: " << decryptedLLInt << endl;
	cout << "List of flipped bits: ";
	for (int i : encrypterLLInt.listFlips) {
		cout << i << " ";
	}
	cout << endl << endl;


	system("pause");
	return 0;
}