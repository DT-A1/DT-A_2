#pragma once
#include "Coding.h"
#include <string>
#include <fstream>
#include <bitset>
#include <direct.h>
#include "dirent.h"
using namespace std;
class Compressor
{
private:
	Coding::Node* root;
	string filePath;
	string inputFile;
	string outputFile;
	bool done;
	string bodyBuffer;
	string headBuffer;
	void createHeader();
	void writeTree(Coding::Node* root);
	void addToBuffer(string& bits, FILE*& outFile);
	const int maxSize = 256;
public:
	void compress();
	Compressor(string inputFileName, string outputFileName);
	~Compressor();
};

