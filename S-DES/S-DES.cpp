#include <iostream>

using namespace std;

bool debug = true;

const short int blockSize = 8;
const short int keySize = 10;
const short int subKeySize = blockSize;
const short int sBlockSize = blockSize / 2;

const short int sBlock0[sBlockSize * sBlockSize] = {
	1, 0, 3, 2,
	3, 2, 1, 0,
	0, 2, 1, 3,
	3, 1, 3, 2
};

const short int sBlock1[sBlockSize * sBlockSize] = {
	0, 1, 2, 3,
	2, 0, 1, 3,
	3, 0, 1, 0,
	2, 1, 0, 3
};

short int inputBlock[blockSize] = { 1, 0, 1, 0, 0, 1, 0, 1 };
short int key[keySize] = { 0, 0, 1, 0, 0, 1, 0, 1, 1, 1 };


short int subKeyK1[subKeySize] = { 0 };
short int subKeyK2[subKeySize] = { 0 };


void printBlock(short int * block, short int len);
void permuteIP(short int * block);
void permuteReIP(short int * block);
void permuteKeyP10(short int * key);
void permuteSubKeyP8(short int * inBlock, short int * outBlock);
void shift1(short int * k);
void shift2(short int * k);
void shiftSW(short int * block);
void generateSubKeys(short int * key, short int * subKeyK1, short int * subKeyK2);


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
	short int tmp[blockSize];
	memcpy(tmp, block, sizeof(short int) * blockSize);

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
	short int tmp[blockSize];
	memcpy(tmp, block, sizeof(short int) * blockSize);

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
	short int tmp[keySize];
	memcpy(tmp, key, sizeof(short int) * keySize);

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
	short int tmp[keySize];
	memcpy(tmp, k, sizeof(short int) * keySize);

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
	short int tmp[keySize];
	memcpy(tmp, k, sizeof(short int) * keySize);

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







void generateSubKeys(short int * key, short int * subKeyK1, short int * subKeyK2)
{
	// permutation
	permuteKeyP10(key);

	// shifts (applied for both subKeys):
	shift1(key);

	// get final subKey1
	permuteSubKeyP8(key, subKeyK1);

	// generate subKey2
	shift2(key);
	permuteSubKeyP8(key, subKeyK2);
}




int main(int argc, char* argv[])
{	
	if (debug)
	{
		cout << "Input block:\t";
		printBlock(inputBlock, blockSize);
		cout << "Input key:\t";
		printBlock(key, keySize);
		cout << endl;
	}
	
	// generate subKeys:	
	generateSubKeys(key, subKeyK1, subKeyK2);

	if (debug)
	{
		cout << "subKey1:\t";
		printBlock(subKeyK1, subKeySize);
		cout << "subKey2:\t";
		printBlock(subKeyK2, subKeySize);
		cout << endl;
	}

	// permutation IP of inputBlock
	permuteIP(inputBlock);

	if (debug)
	{
		cout << "IP block:\t";
		printBlock(inputBlock, blockSize);
	}
	

	system("PAUSE");


	return 0;
}

