#include "Extractor.h"
#include <iostream>

void Extractor::readBits(FILE*& inFile)
{
	char ch;
	while (nCharLeft > 0)
	{
		fread(&ch, sizeof(char), 1, inFile);
		bitsBuffer += bitset<8>(ch).to_string();
		nCharLeft = fileSize - ftell(inFile);
		if (bitsBuffer.size() >= maxSize) break;
	}
	if (nCharLeft == 0 && !done)
	{
		bitsBuffer.erase(bitsBuffer.end() - endBlanks, bitsBuffer.end());
		done = true;
	}
}
char Extractor::decodeBits(Node* codeTree, int index)
{
	if (codeTree->left == NULL && codeTree->right == NULL)
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
char Extractor::getNextChar(FILE*& inFile)
{
	readBits(inFile);
	int index = -1;
	return decodeBits(codeTree, index);	
}
string Extractor::readFileName(FILE*& inFile)
{
	string result = "";
	while (result.size() < nameLength)
	{
		result += getNextChar(inFile);
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
	outputFile = readFileName(inFile);
	FILE* outFile = fopen(outputFile.c_str(), "wb");
	//read file's content;
	while (bitsBuffer.size() > 0)
	{
		char nextChar = getNextChar(inFile);
		charsBuffer.push_back(nextChar);
		if (charsBuffer.size() > 256)
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
