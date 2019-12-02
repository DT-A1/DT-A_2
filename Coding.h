#pragma once
#include <queue>
#include <vector>
#include <unordered_map>
using namespace std;
class Coding
{
public:
	struct Node
	{
		char ch;
		int freq;
		Node* left;
		Node* right;
	};
private:
	struct compare
	{
		bool operator() (Node* a, Node* b) { return a->freq > b->freq; }
	};
	unordered_map<char, int> freqs;
	unordered_map<char, string> codes;
	Node* createNode(char ch, int freq, Node* left, Node* right);
	Node* generateHuffmanTree();
	void encode(Node* root, string code);
public:
	void getFrequencies(string text);
	void updateFrequencies(char ch);
	unordered_map<char, string> getCodes();
	Coding();
	~Coding();
};

