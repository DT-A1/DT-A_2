#pragma once
#include "Coding.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <bitset>
using namespace std;
class Compressor
{
	//fileName
	Coding::Node* root;
	string inputFile;
	string outputFile;
public:
	void SetInputFile(string inputFile);
	void SetOutputFile(string outputFile);
	void compress();
	void decompress();
	Compressor();
	~Compressor();
};

