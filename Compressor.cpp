#include "Compressor.h"
#include <iostream>

void Compressor::writeTree(Coding::Node* root)
{
	if (root == NULL) return;
	if (root->left == NULL)
	{
		headBuffer.push_back('0');
		headBuffer.push_back(root->ch);
	}
	else
	{
		headBuffer.push_back('1');
		writeTree(root->left);
		writeTree(root->right);
	}
}
void Compressor::createHeader()
{
	headBuffer += ' ';
	headBuffer += to_string(inputFile.size() - filePath.size() - 1);
	headBuffer += ' ';
	writeTree(root);
}
void Compressor::addToBuffer(string& bits, FILE*& outFile)
{
	bitset<8> bs(bits, 0, 8, '0', '1');
	bodyBuffer.push_back((char)bs.to_ulong());
	bits.erase(0, 8);
	if (bodyBuffer.size() >= maxSize)
	{
		fwrite(&bodyBuffer[0], sizeof(char), bodyBuffer.size(), outFile);
		bodyBuffer = "";
	}
}

void Compressor::compress()
{
	Coding codeGenerator;

	string onlyFileName = inputFile.substr(filePath.size() + 1, inputFile.size());
	codeGenerator.getFrequencies(onlyFileName);

	FILE* inFile = fopen(inputFile.c_str(), "rb");
	if (inFile == NULL) throw "Cannot open file!";
	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);

	fseek(inFile, 0, SEEK_SET);
	string chars(maxSize, '\0');
	int nCharLeft = fileSize;
	while (nCharLeft > 256)
	{
		fread(&chars[0], sizeof(char), maxSize, inFile);
		codeGenerator.getFrequencies(chars);
		nCharLeft -= maxSize;
	}
	chars = string(nCharLeft, '\0');
	fread(&chars[0], sizeof(char), nCharLeft, inFile);
	chars = chars.substr(0, nCharLeft);
	codeGenerator.getFrequencies(chars);

	root = codeGenerator.generateHuffmanTree();
	vector<string> codes = codeGenerator.getCodes(root);

	FILE* outFile = fopen(outputFile.c_str(), "wb");
	done = false;
	//createHeader
	createHeader();
	fwrite(&headBuffer[0], sizeof(char), headBuffer.size(), outFile);
	//CreateBody
	string bits = "";
	for (int i = 0; i < onlyFileName.length(); i++)
	{
		bits += codes[(unsigned char)onlyFileName[i]];
		if (bits.size() > 8)
		{
			addToBuffer(bits, outFile);
		}
	}

	fseek(inFile, 0, SEEK_SET);
	nCharLeft = fileSize;
	string charsBuffer;
	while (!done)
	{
		if (nCharLeft >= maxSize)
		{
			charsBuffer = string(maxSize, '\0');
			fread(&charsBuffer[0], sizeof(char), maxSize, inFile);
		}
		else
		{
			charsBuffer = string(nCharLeft, '\0');
			fread(&charsBuffer[0], sizeof(char), nCharLeft, inFile);
			done = true;
		}
		int bufferSize = charsBuffer.size();
		for (int i = 0; i < bufferSize; i++)
		{
			bits += codes[(unsigned char)charsBuffer[i]];
			if (bits.size() >= 8)
				addToBuffer(bits, outFile);
		}
		nCharLeft = fileSize - ftell(inFile);
	}
	fwrite(&bodyBuffer[0], sizeof(char), bodyBuffer.size(), outFile);
	int endBlanks = 0;
	while (bits.size() > 0 && bits.size() < 8)
	{
		bits += "0";
		endBlanks++;
	}
	if (bits.size() == 8)
	{
		char lastChar = (char)bitset<8>(bits).to_ulong();
		fwrite(&lastChar, sizeof(char), 1, outFile);
	}
	fseek(outFile, 0, SEEK_SET);
	fwrite(&endBlanks, sizeof(char), 1, outFile);

	fclose(inFile);
	fclose(outFile);
}

Compressor::Compressor(string inputFileName, string outputFileName)
{
	int pos = inputFileName.rfind("\\");
	if (pos != inputFileName.npos)
	{
		filePath = inputFileName.substr(0, pos);
	}
	inputFile = inputFileName;
	outputFile = outputFileName;
	if (outputFile.size() < 4 || outputFile.substr(outputFile.length() - 5, outputFile.length() - 1) != ".cprs")
	{
		outputFile += ".cprs";
	}
	outputFile = filePath + "\\" + outputFile;
	done = false;
}


Compressor::~Compressor()
{
}
