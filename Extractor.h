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
	string desDir;
	string inputFile;
	string outputFile;
	int endBlanks;
	int nameLength;
	bool done;
	int fileSize;
	int nCharLeft;
	string bitsBuffer;
	string charsBuffer;
	string fileBuffer;
	void readBits(FILE*& inFile, int& i);
	string readFileName(FILE*& inFile, int& i);
	Node* readTree(FILE*& inFile);
	char decodeBits(Node* codeTree, int index);
	char getNextChar(FILE* inFile, int& i);
	const int maxSize = 256;
public:
	void extract();
	Extractor(string inputFileName, string desDirectory);
	~Extractor();
};

