#include "Extractor.h"
#include <iostream>

void Extractor::readBits(FILE*& inFile, int& i)
{
	if (i >= fileBuffer.size())
	{
		if (nCharLeft >= maxSize)
		{
			fread(&fileBuffer[0], sizeof(char), maxSize, inFile);
		}
		else
		{
			fread(&fileBuffer[0], sizeof(char), nCharLeft, inFile);
			fileBuffer.resize(nCharLeft);
			done = true;
		}
		nCharLeft -= fileBuffer.size();
		i = 0;
	}
	while (bitsBuffer.size() < maxSize && i < fileBuffer.size())
	{
		bitsBuffer += bitset<8>(fileBuffer[i]).to_string();
		++i;
		if (i == fileBuffer.size() && done) bitsBuffer.erase(bitsBuffer.end() - endBlanks, bitsBuffer.end());
	}
}
char Extractor::decodeBits(Node* codeTree, int index)
{
	if (codeTree->left == NULL)
	{
		bitsBuffer.erase(0, index + 1);
		return codeTree->ch;
	}
	index++;
	if (bitsBuffer[index] == '0')
	{
		return decodeBits(codeTree->left, index);
	}
	else
	{
		return decodeBits(codeTree->right, index);
	}
}
char Extractor::getNextChar(FILE* inFile, int& i)
{
	readBits(inFile, i);
	int index = -1;
	return decodeBits(codeTree, index);	
}
string Extractor::readFileName(FILE*& inFile, int& i)
{
	string result = "";
	while (result.size() < nameLength)
	{
		result += getNextChar(inFile, i);
	}
	return result;
}
Extractor::Node* Extractor::readTree(FILE*& inFile)
{
	char next;
	fread(&next, sizeof(char), 1, inFile);
	Node* root = new Node;
	if (next == '0')
	{
		root->ch = fgetc(inFile);
		root->left = root->right = NULL;
	}
	else
	{
		root->ch = '\0';
		root->left = readTree(inFile);
		root->right = readTree(inFile);
	}
	return root;
}
void  Extractor::extract()
{
	FILE* inFile = fopen(inputFile.c_str(), "rb");
	if (inFile == NULL) throw "Cannot open file!";
	fseek(inFile, 0, SEEK_END);
	fileSize = ftell(inFile);
	nCharLeft = fileSize;
	fseek(inFile, 0, SEEK_SET);
	//read endBlanks;
	endBlanks = fgetc(inFile);
	//read nameLength;
	fscanf(inFile, "%d", &nameLength);
	//read Huffman tree;
	fgetc(inFile);
	codeTree = readTree(inFile);
	//read original file's name;
	fileBuffer = string(maxSize, '\0');
	nCharLeft -= ftell(inFile);
	int i = maxSize;
	outputFile = readFileName(inFile, i);
	FILE* outFile = fopen(outputFile.c_str(), "wb");
	//read file's content;
	char nextChar;
	while (bitsBuffer.size() > 0)
	{
		nextChar = getNextChar(inFile, i);
		charsBuffer.push_back(nextChar);
		if (charsBuffer.size() >= maxSize)
		{
			fwrite(&charsBuffer[0], sizeof(char), maxSize, outFile);
			charsBuffer = "";
		}
	}
	if (done)
	{
		fwrite(&charsBuffer[0], sizeof(char), charsBuffer.size(), outFile);
	}
	fclose(inFile);
	fclose(outFile);
}

Extractor::Extractor(string inputFileName)
{
	inputFile = inputFileName;
	done = false;

}


Extractor::~Extractor()
{

}
