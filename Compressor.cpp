#include "Compressor.h"
#include <iostream>

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

void Compressor::writeTree(Coding::Node* root)
{
	if (root == NULL) return;
	if (root->left == NULL && root->right == NULL)
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
	headBuffer += to_string(inputFile.size());
	headBuffer += ' ';
	writeTree(root);
}
void Compressor::addToBuffer(string& bits, FILE* outFile)
{
	bitset<8> bs(bits, 0, 8, '0', '1');
	bodyBuffer.push_back((char)bs.to_ulong());
	bits.erase(0, 8);
	if (bodyBuffer.size() > 256)
	{
		fwrite(&bodyBuffer[0], sizeof(char), bodyBuffer.size(), outFile);
		bodyBuffer.resize(0);
	}
}

void Compressor::compress()
{
	Coding codeGenerator;
	codeGenerator.getFrequencies(inputFile);
	FILE* inFile = fopen(inputFile.c_str(), "rb");
	if (inFile == NULL) throw "Cannot open file!";
	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	string chars(256, '\0');
	int nCharLeft = fileSize - ftell(inFile);
	while (nCharLeft > 256)
	{
		fread(&chars[0], sizeof(char), 256, inFile);
		codeGenerator.getFrequencies(chars);
		nCharLeft = fileSize - ftell(inFile);
	}
	chars = string(nCharLeft, '\0');
	fread(&chars[0], sizeof(char), nCharLeft, inFile);
	chars = chars.substr(0, nCharLeft);
	codeGenerator.getFrequencies(chars);
	root = codeGenerator.generateHuffmanTree();
	unordered_map<char, string> codes = codeGenerator.getCodes(root);
	FILE* outFile = fopen(outputFile.c_str(), "wb");
	done = false;
	//createHeader
	//fprintf(outFile, "  %d ", inputFile.length());
	createHeader();
	fwrite(&headBuffer[0], sizeof(char), headBuffer.size(), outFile);
	//CreateBody
	string bits = "";
	for (int i = 0; i < inputFile.length(); i++)
	{
		bits += codes[inputFile[i]];
		if (bits.size() > 8)
		{
			addToBuffer(bits, outFile);
		}
	}
	fseek(inFile, 0, SEEK_SET);
	while (!done)
	{
		while (bits.size() < 8)
		{
			char cur_char;
			fscanf(inFile, "%c", &cur_char);
			if (feof(inFile))
			{
				done = true;
				break;
			}
			bits += codes[cur_char];
		}
		if (!done) addToBuffer(bits, outFile);
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
	inputFile = inputFileName;
	outputFile = outputFileName;
	if (outputFile == inputFileName)
	{
		outputFile += ".cprs";
	}
	done = false;
}


Compressor::~Compressor()
{
}
