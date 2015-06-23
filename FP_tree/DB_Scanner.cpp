#include "DB_Scanner.h"

DB_Scanner::DB_Scanner()
	: Total_Transaction(0), limit(0)
{
	for (int i = 0; i < 15000; i++)
	{

		allItemTable[i] = 0;
	}
};

void DB_Scanner::setFileName(string iFileName){
	fileName = iFileName;
};

void DB_Scanner::setMinSup(int iMinSup)
{
	minSup = iMinSup;
};
void DB_Scanner::firstCheck()
{
	inFile.open(fileName, ios::in);

	string newFileName(fileName);
	newFileName = "S_" + newFileName;
	ofstream out(newFileName.c_str(), ios::binary);

	if (!inFile.is_open() || !out.is_open()) return;

	int temp = 0;
	char inputC;
	int buf_index = 0;
	while (!inFile.eof())
	{

		int buf[1000];


		inputC = inFile.get();
		if (inputC > 47)
		{
			temp *= 10;
			temp += inputC - 48;
		}
		else if (inputC == ',')
		{
			++allItemTable[temp];
			buf[++buf_index] = temp;
			temp = 0;
		}
		else if (inputC == '\n')
		{
			//存入buf
			++allItemTable[temp];
			buf[++buf_index] = temp;
			temp = 0;
			//將buf存入二進制

			buf[0] = buf_index;
			out.write((char *)&buf, sizeof(int)*(buf_index + 1));
			++Total_Transaction;
			buf_index = 0;
		}
	}

	inFile.close();

}

bool DB_Scanner::openBinaryDatabase()
{
	string newFileName(fileName);
	newFileName = "S_" + newFileName;
	inFileBinary.open(newFileName.c_str(), ios::binary);
	if (inFileBinary.is_open() && !inFileBinary.eof())return true;
	else
		return false;
}
void DB_Scanner::closeBinaryDatabase()
{
	inFileBinary.close();
}

bool DB_Scanner::readLine(list<int> &inArr)
{
	if (!inFileBinary.is_open() || inFileBinary.eof())return false;

	inArr.clear();
	int count;
	inFileBinary.read((char *)& count, sizeof(count));

	for (int i = 0; i < count; i++)
	{

		int buf;
		inFileBinary.read((char *)& buf, sizeof(buf));

		inArr.push_back(buf);
	}

	return true;


}
