#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;


// config:
bool debug = false; // highly NOT recommended for bigger files!
short int key[10] = { 0, 0, 1, 0, 0, 1, 0, 1, 1, 1 }; // encryption key; keySize = 10
bool crypt = true; // if true, plainFile will be crypted
const string plainFilename = "lipsum-100mb.txt";
bool decrypt = true; // if true, cryptedFile will be decrypted
const string cryptedFilename = "encrypted_lipsum-100mb.txt";
// config end


const short int sBlock0[4 * 4] = {
	1, 0, 3, 2,
	3, 2, 1, 0,
	0, 2, 1, 3,
	3, 1, 3, 2
};

const short int sBlock1[4 * 4] = {
	0, 1, 2, 3,
	2, 0, 1, 3,
	3, 0, 1, 0,
	2, 1, 0, 3
};

short int inputBlock[8] = { 0 }; // blockSize = 8

short int subKeyK1[8] = { 0 };
short int subKeyK2[8] = { 0 };
bool subKeysGenerated = false;

// function declaration
void printBlock(short int * block, short int len);
void permuteIP(short int * block);
void permuteReIP(short int * block);
void permuteKeyP10(short int * key);
void permuteSubKeyP8(short int * inBlock, short int * outBlock);
void shift1(short int * k);
void shift2(short int * k);
void shiftSW(short int * block);
short int binToDec(short int bin1, short int bin2);
void generateSubKeys(short int * key, short int * subKeyK1, short int * subKeyK2);
void functionF(short int * block, short int * subKey);
char sDesCrypt(char plainChar, short int * key);
char sDesDecrypt(char cryptedChar, short int * key);


void printBlock(short int * block, short int len)
{
	for (int i = 0; i < len; ++i)
	{
		cout << block[i] << " ";
	}
	cout << endl;
}

void permuteIP(short int * block)
{
	// IP = [ 2 6 3 1 4 8 5 7 ]
	// IP = [ 1 5 2 0 3 7 4 6 ] -> indexes starts from 0
	short int tmp[8];
	memcpy(tmp, block, sizeof(short int) * 8);

	block[0] = tmp[1];
	block[1] = tmp[5];
	block[2] = tmp[2];
	block[3] = tmp[0];
	block[4] = tmp[3];
	block[5] = tmp[7];
	block[6] = tmp[4];
	block[7] = tmp[6];	
}

void permuteReIP(short int * block)
{
	// IP^-1 = [ 4 1 3 5 7 2 8 6 ]
	// IP^-1 = [ 3 0 2 4 6 1 7 5 ] -> indexes starts from 0
	short int tmp[8];
	memcpy(tmp, block, sizeof(short int) * 8);

	block[0] = tmp[3];
	block[1] = tmp[0];
	block[2] = tmp[2];
	block[3] = tmp[4];
	block[4] = tmp[6];
	block[5] = tmp[1];
	block[6] = tmp[7];
	block[7] = tmp[5];
}


void permuteKeyP10(short int * key)
{
	// block size = 10 elements!
	// P10 = [ 3 5 2 7 4 10 1 9 8 6 ]
	// P10 = [ 2 4 1 6 3 9 0 8 7 5 ] -> indexes starts from 0
	short int tmp[10];
	memcpy(tmp, key, sizeof(short int) * 10);

	key[0] = tmp[2];
	key[1] = tmp[4];
	key[2] = tmp[1];
	key[3] = tmp[6];
	key[4] = tmp[3];
	key[5] = tmp[9];
	key[6] = tmp[0];
	key[7] = tmp[8];
	key[8] = tmp[7];
	key[9] = tmp[5];
}


void permuteSubKeyP8(short int * inBlock, short int * outBlock)
{
	// P8 = [ 6 3 7 4 8 5 10 9 ]
	// P8 = [ 5 2 6 3 7 4 9 8 ] -> indexes starts from 0
	// inBlock = 10 elements
	// outBlock = 8 elements
	outBlock[0] = inBlock[5];
	outBlock[1] = inBlock[2];
	outBlock[2] = inBlock[6];
	outBlock[3] = inBlock[3];
	outBlock[4] = inBlock[7];
	outBlock[5] = inBlock[4];
	outBlock[6] = inBlock[9];
	outBlock[7] = inBlock[8];
}


void shift1(short int * k)
{
	// input =	[ 0 1 2 3 4 5 6 7 8 9 ]
	// output = [ 1 2 3 4 0 6 7 8 9 5 ]
	short int tmp[10];
	memcpy(tmp, k, sizeof(short int) * 10);

	k[0] = tmp[1];
	k[1] = tmp[2];
	k[2] = tmp[3];
	k[3] = tmp[4];
	k[4] = tmp[0];
	k[5] = tmp[6];
	k[6] = tmp[7];
	k[7] = tmp[8];
	k[8] = tmp[9];
	k[9] = tmp[5];
}


void shift2(short int * k)
{
	// input =	[ 0 1 2 3 4 5 6 7 8 9 ]
	// output = [ 2 3 4 0 1 7 8 9 5 6 ]
	short int tmp[10];
	memcpy(tmp, k, sizeof(short int) * 10);

	k[0] = tmp[2];
	k[1] = tmp[3];
	k[2] = tmp[4];
	k[3] = tmp[0];
	k[4] = tmp[1];
	k[5] = tmp[7];
	k[6] = tmp[8];
	k[7] = tmp[9];
	k[8] = tmp[5];
	k[9] = tmp[6];
}


void shiftSW(short int * block)
{
	// input =	[ 0 1 2 3 4 5 6 7 ]
	// output = [ 4 5 6 7 0 1 2 3 ]
	short int tmp = block[0];
	block[0] = block[4];
	block[4] = tmp;
	tmp = block[1];
	block[1] = block[5];
	block[5] = tmp;
	tmp = block[2];
	block[2] = block[6];
	block[6] = tmp;
	tmp = block[3];
	block[3] = block[7];
	block[7] = tmp;
}


short int binToDec(short int bin1, short int bin2)
{	
	if (bin1 == 0 && bin2 == 1)
	{
		return 1;
	}
	else if (bin1 == 1 && bin2 == 0)
	{
		return 2;
	}
	else if (bin1 == 1 && bin2 == 1)
	{
		return 3;
	}
	else // bin1 == 0 && bin2 == 0
	{
		return 0;
	}
}


void generateSubKeys(short int * key, short int * subKeyK1, short int * subKeyK2)
{
	short int tmpKey[10];
	memcpy(tmpKey, key, sizeof(short int) * 10);

	// permutation
	permuteKeyP10(tmpKey);

	// shifts (applied for both subKeys):
	shift1(tmpKey);

	// get final subKey1
	permuteSubKeyP8(tmpKey, subKeyK1);

	// generate subKey2
	shift2(tmpKey);
	permuteSubKeyP8(tmpKey, subKeyK2);

	subKeysGenerated = true;
}


void functionF(short int * block, short int * subKey)
{
	// split input block L / R
	short int blockLeft[4] = { block[0], block[1], block[2], block[3] };
	short int blockRight[4] = { block[4], block[5], block[6], block[7] };

	// 4 -> 8-bit block extension (Extending Permutation E)
	short int blockExt[8] = { blockRight[3], blockRight[0], blockRight[1], blockRight[2], blockRight[1], blockRight[2], blockRight[3], blockRight[0] };

	// blockExt XOR subKey
	for (int i = 0; i < 8; ++i)
	{
		blockExt[i] = blockExt[i] ^ subKey[i];
	}

	// S-Block S0 transformation
	short int numS0 = sBlock0[binToDec(blockExt[0], blockExt[3]) * 4 + binToDec(blockExt[1], blockExt[2])]; // row * widh + column
	short int numS1 = sBlock1[binToDec(blockExt[4], blockExt[7]) * 4 + binToDec(blockExt[5], blockExt[6])]; // row * widh + column

	// block P4 -> stored in tmpBlock array
	short int tmpBlock[4] = { 0 };
	// P4 permutation: [ 1 3 2 0 ]
	// L-part:
	if (numS0 == 0)
	{
		tmpBlock[3] = 0; // tmpBlock[0] before permutation P4
		tmpBlock[0] = 0; // tmpBlock[1] before permutation P4
	}
	else if (numS0 == 1)
	{
		tmpBlock[3] = 0; // tmpBlock[0] before permutation P4
		tmpBlock[0] = 1; // tmpBlock[1] before permutation P4
	}
	else if (numS0 == 2)
	{
		tmpBlock[3] = 1; // tmpBlock[0] before permutation P4
		tmpBlock[0] = 0; // tmpBlock[1] before permutation P4
	}
	else
	{
		tmpBlock[3] = 1; // tmpBlock[0] before permutation P4
		tmpBlock[0] = 1; // tmpBlock[1] before permutation P4
	}

	// R-part:
	if (numS1 == 0)
	{
		tmpBlock[2] = 0; // tmpBlock[2] before permutation P4
		tmpBlock[1] = 0; // tmpBlock[3] before permutation P4
	}
	else if (numS1 == 1)
	{
		tmpBlock[2] = 0; // tmpBlock[2] before permutation P4
		tmpBlock[1] = 1; // tmpBlock[3] before permutation P4
	}
	else if (numS1 == 2)
	{
		tmpBlock[2] = 1; // tmpBlock[2] before permutation P4
		tmpBlock[1] = 0; // tmpBlock[3] before permutation P4
	}
	else
	{
		tmpBlock[2] = 1; // tmpBlock[2] before permutation P4
		tmpBlock[1] = 1; // tmpBlock[3] before permutation P4
	}

	// blockLeft XOR block P4 (stored in tmpBlock)
	for (int i = 0; i < 4; ++i)
	{
		block[i] = blockLeft[i] ^ tmpBlock[i];
	}

	// final block -> stored in block array
	// block[0] : block[3] -> blockLeft XOR block P4
	// block[4] : block[7] -> blockRight
	block[4] = blockRight[0];
	block[5] = blockRight[1];
	block[6] = blockRight[2];
	block[7] = blockRight[3];

	if (debug)
	{
		cout << "functionFk:\t";
		printBlock(block, 8);
	}	
}


char sDesCrypt(char plainChar, short int * key)
{
	// conversion from char to binary
	for (int i = 7; i >= 0; --i)
	{	
		inputBlock[7 - i] = (short int)((plainChar & (1 << i)) ? 1 : 0);
	}

	if (debug)
	{
		cout << "Input block:\t";
		printBlock(inputBlock, 8);
		cout << "Input key:\t";
		printBlock(key, 10);
		cout << endl;
	}

	if (!subKeysGenerated)
	{
		generateSubKeys(key, subKeyK1, subKeyK2);
	}	

	if (debug)
	{
		cout << "subKey1:\t";
		printBlock(subKeyK1, 8);
		cout << "subKey2:\t";
		printBlock(subKeyK2, 8);
		cout << endl;
	}

	// permutation IP of inputBlock
	permuteIP(inputBlock);

	if (debug)
	{
		cout << "IP block:\t";
		printBlock(inputBlock, 8);
	}

	// FunctionF with subKeyK1
	functionF(inputBlock, subKeyK1);

	// SW
	shiftSW(inputBlock);

	if (debug)
	{
		cout << "SW block:\t";
		printBlock(inputBlock, 8);
	}

	// FunctionF with subKeyK2
	functionF(inputBlock, subKeyK2);

	// permutation ReIP of inputBlock
	permuteReIP(inputBlock);

	if (debug)
	{
		cout << "final block:\t";
		printBlock(inputBlock, 8);
	}

	// conversion from binary to char
	short int outputVal = 0;
	short int factor = 128;
	for (int i = 0; i < 8; ++i)
	{
		outputVal += (factor * inputBlock[i]);
		factor = factor >> 1;		
	}
	
	return (char)outputVal;
}


char sDesDecrypt(char cryptedChar, short int * key)
{
	// conversion from char to binary
	for (int i = 7; i >= 0; --i)
	{
		inputBlock[7 - i] = (short int)((cryptedChar & (1 << i)) ? 1 : 0);
	}

	if (debug)
	{
		cout << "Input block:\t";
		printBlock(inputBlock, 8);
		cout << "Input key:\t";
		printBlock(key, 10);
		cout << endl;
	}

	// generate subKeys:
	if (!subKeysGenerated)
	{
		generateSubKeys(key, subKeyK1, subKeyK2);
	}	

	if (debug)
	{
		cout << "subKey1:\t";
		printBlock(subKeyK1, 8);
		cout << "subKey2:\t";
		printBlock(subKeyK2, 8);
		cout << endl;
	}

	// permutation IP of inputBlock
	permuteIP(inputBlock);

	if (debug)
	{
		cout << "IP block:\t";
		printBlock(inputBlock, 8);
	}

	// FunctionF with subKeyK2
	functionF(inputBlock, subKeyK2);

	// SW
	shiftSW(inputBlock);

	if (debug)
	{
		cout << "SW block:\t";
		printBlock(inputBlock, 8);
	}

	// FunctionF with subKeyK1
	functionF(inputBlock, subKeyK1);

	// permutation ReIP of inputBlock
	permuteReIP(inputBlock);

	if (debug)
	{
		cout << "final block:\t";
		printBlock(inputBlock, 8);
	}

	// conversion from binary to char
	short int outputVal = 0;
	short int factor = 128;
	for (int i = 0; i < 8; ++i)
	{
		outputVal += (factor * inputBlock[i]);
		factor = factor >> 1;
	}

	return (char)outputVal;
}


int main(int argc, char* argv[])
{	
	/*
	// single char encryption / decryption:
	char plainSample = 'L';
	char cryptedSample;
	char decryptedSample;
	cout << "Input char:\t" << plainSample << endl;
	cryptedSample = sDesCrypt(plainSample, key);
	cout << "Encrypted char:\t" << cryptedSample << endl;
	cout << "\n=====> Decryption:" << endl;
	decryptedSample = sDesDecrypt(cryptedSample, key);
	cout << "Decrypted char:\t" << decryptedSample << endl;
	*/

	
	if (crypt)
	{
		// load file into memory
		cout << "S-DES encryption of file: " << plainFilename << "." << endl;

		clock_t begin_load = clock(); // time capture

		ifstream input;
		unsigned __int64 fileLength = 0;
		char * inputText;
		char * outputText;
		input.open(plainFilename, ios::binary); // open input file
		if (!input.is_open())
		{
			cout << "Error opening input file!" << endl;
			return 1;
		}
		input.seekg(0, input.end);			// go to the end
		fileLength = input.tellg();			// report location (this is the length)		
		input.seekg(0, input.beg);			// go back to the beginning
		inputText = new char[fileLength];	// allocate memory for a buffer of appropriate dimension
		input.read(inputText, fileLength);	// read the whole file into the buffer
		input.close();						// close file handle

		cout << "File loaded in\t\t" << double(clock() - begin_load) / CLOCKS_PER_SEC << " s" << endl;
		cout << "File size: \t\t" << fileLength << " bytes." << endl;
		
		// ENCRYPTION
		clock_t begin_encryption = clock();  // time capture		
		outputText = new char[fileLength];
		for (unsigned long long int i = 0; i < fileLength; ++i)
		{
			outputText[i] = sDesCrypt(inputText[i], key);
		}
		cout << "Text encrypted in\t" << double(clock() - begin_encryption) / CLOCKS_PER_SEC << " s" << endl;

		// Save result to file
		clock_t begin_saving = clock();  // time capture		
		ofstream output;
		output.open("encrypted_" + plainFilename, ios::binary);
		if (!output.is_open())
		{
			cout << "Error opening file to save results!" << endl;
			return 1;
		}
		output.write(outputText, fileLength);
		output.close();
		cout << "File saved in\t\t" << double(clock() - begin_saving) / CLOCKS_PER_SEC << " s" << endl;

		// SUMMARY
		cout << "Total time elapsed:\t" << double(clock() - begin_load) / CLOCKS_PER_SEC << " s" << endl;		

		delete[] inputText;
		delete[] outputText;
	}

	if (decrypt)
	{
		// load file into memory
		cout << "S-DES decryption of file: " << cryptedFilename << "." << endl;

		clock_t begin_load = clock(); // time capture

		ifstream input;
		unsigned __int64 fileLength = 0;
		char * inputText;
		char * outputText;
		input.open(cryptedFilename, ios::binary); // open input file
		if (!input.is_open())
		{
			cout << "Error opening input file!" << endl;
			return 1;
		}
		input.seekg(0, input.end);			// go to the end
		fileLength = input.tellg();			// report location (this is the length)		
		input.seekg(0, input.beg);			// go back to the beginning
		inputText = new char[fileLength];	// allocate memory for a buffer of appropriate dimension
		input.read(inputText, fileLength);	// read the whole file into the buffer
		input.close();						// close file handle

		cout << "File loaded in\t\t" << double(clock() - begin_load) / CLOCKS_PER_SEC << " s" << endl;
		cout << "File size: \t\t" << fileLength << " bytes." << endl;

		// ENCRYPTION
		clock_t begin_encryption = clock();  // time capture		
		outputText = new char[fileLength];
		for (unsigned long long int i = 0; i < fileLength; ++i)
		{
			outputText[i] = sDesDecrypt(inputText[i], key);
		}
		cout << "Text decrypted in\t" << double(clock() - begin_encryption) / CLOCKS_PER_SEC << " s" << endl;

		// Save result to file
		clock_t begin_saving = clock();  // time capture		
		ofstream output;
		output.open("decrypted_" + cryptedFilename, ios::binary);
		if (!output.is_open())
		{
			cout << "Error opening file to save results!" << endl;
			return 1;
		}
		output.write(outputText, fileLength);
		output.close();
		cout << "File saved in\t\t" << double(clock() - begin_saving) / CLOCKS_PER_SEC << " s" << endl;

		// SUMMARY
		cout << "Total time elapsed:\t" << double(clock() - begin_load) / CLOCKS_PER_SEC << " s" << endl;

		delete[] inputText;
		delete[] outputText;
	}

	if (!crypt && !decrypt)
	{
		cout << "Hey, there's nothig to do!" << endl;
	}	
	
	system("PAUSE");
	return 0;
}

