#pragma once
#include <string>
#include <vector>
#include <bitset>
#include "Coding.h"
class Extractor
{
private:
	struct Node {
		char ch;
		Node* left;
		Node* right;
	};
	Node* codeTree;
	string inputFile;
	string outputFile;
	int endBlanks;
	int nameLength;
	bool done;
	int fileSize;
	int nCharLeft;
	string bitsBuffer;
	string charsBuffer;
	void readBits(FILE*& inFile);
	string readFileName(FILE*& inFile);
	Node* readTree(FILE*& inFile);
	char decodeBits(Node* codeTree, int index);
	char getNextChar(FILE*& inFile);
	const int maxSize = 256;
public:
	void extract();
	Extractor(string inputFileName);
	~Extractor();
};

