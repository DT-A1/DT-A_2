#include "Compressor.h"

void Compressor::SetInputFile(string inputFile)
{
	this->inputFile = inputFile;
}
void Compressor::SetOutputFile(string outputFile)
{
	if (outputFile == this->inputFile)
	{
		this->outputFile = this->inputFile;
		this->outputFile.erase(this->outputFile.end() - 3, this->outputFile.end());
		this->outputFile += "cprs";
		return;
	}
	this->outputFile = outputFile;
}

void Compressor::compress()
{
	Coding codeGenerator;
	ifstream inFile;
	inFile.open(inputFile, ios::binary);
	while (!inFile.eof())
	{
		codeGenerator.updateFrequencies(inFile.get());
	}
	unordered_map<char, string> codes = codeGenerator.getCodes();
	//createHeader

	//CreateBody
	ofstream outFile;
	outFile.open(outputFile, ios::binary);
	string bits = "";
	bool done = false;
	inFile.seekg(0, ios_base::beg);
	while (!done)
	{
		while (bits.size() < 8)
		{
			char cur_char = inFile.get();
			if (inFile.eof())
			{
				done = true;
				break;
			}
			bits += codes[cur_char];
		}
		bitset<8> bs(bits, 0, 8, '0', '1');
		char in_char[1];
		in_char[0] = (char)bs.to_ulong();
		outFile.write(in_char, 1);
		bits.erase(0, 8);
	}
	int endBlanks = 0;
	while (bits.size() < 8)
	{
		bits += "0";
		endBlanks++;
	}
}

Compressor::Compressor()
{
}


Compressor::~Compressor()
{
}
