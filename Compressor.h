#pragma once
#include "Coding.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <bitset>
using namespace std;
class Compressor
{
private:
	Coding::Node* root;
	string inputFile;
	string outputFile;
	bool done;
	string bodyBuffer;
	string headBuffer;
	void createHeader();
	void writeTree(Coding::Node* root);
	void addToBuffer(string& bits, FILE* outFile);
public:
	void SetInputFile(string inputFile);
	void SetOutputFile(string outputFile);
	//void setPseudoEOF(unordered_map<char, string> &codes);
	void compress();
	Compressor(string inputFileName, string outputFileName);
	~Compressor();
};

