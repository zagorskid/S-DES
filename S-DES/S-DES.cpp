#include <iostream>

using namespace std;

const int blockSize = 8;

short int inputBlock[blockSize] = { 1, 0, 1, 0, 1, 0, 0, 1 };

void permuteIP(short int * block);
void permuteP10(short int * block);
void permuteP8(short int * block);
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

	cout << "hejo!" << endl;

	printBlock(inputBlock);
	permuteIP(inputBlock);

	cout << "Po permutacji: " << endl; 
	printBlock(inputBlock);

	system("PAUSE");


	return 0;
}

