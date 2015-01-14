#include <iostream>

using namespace std;

const int blockSize = 8;

short int inputBlock[blockSize] = { 1, 0, 1, 0, 1, 0, 0, 1 };

void permuteIP(short int * block);
void permuteReIP(short int * block);
void permuteP10(short int * block);
void permuteP8(short int * inBlock, short int * outBlock);
void printBlock(short int * block);


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


void permuteP10(short int * block)
{
	// P10 = [ 3 5 2 7 4 10 1 9 8 6 ]
	// p10 = [ 2 4 1 6 3 9 0 8 7 5 ] -> indexes starts from 0
	short int tmp[blockSize];
	memcpy(tmp, block, sizeof(short int) * blockSize);

	block[0] = tmp[2];
	block[1] = tmp[4];
	block[2] = tmp[1];
	block[3] = tmp[6];
	block[4] = tmp[3];
	block[5] = tmp[9];
	block[6] = tmp[0];
	block[7] = tmp[8];
	block[8] = tmp[7];
	block[9] = tmp[5];
}


void permuteP8(short int * inBlock, short int * outBlock)
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


void printBlock(short int * block)
{
	for (int i = 0; i < blockSize; ++i)
	{
		cout << block[i] << " ";
	}
	cout << endl;
}


int main(int argc, char* argv[])
{

	printBlock(inputBlock);
	permuteIP(inputBlock);

	cout << "Permutation IP: " << endl; 
	printBlock(inputBlock);

	permuteReIP(inputBlock);

	cout << "Permutation IP^-1: " << endl;
	printBlock(inputBlock);

	system("PAUSE");


	return 0;
}

