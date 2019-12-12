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
	struct cmp
	{
		bool operator() (Node*& a, Node*& b) { return a->freq > b->freq; }
	};
	vector<int> frequencies;
	vector<string> codes;
	Node* createNode(char ch, int freq, Node* left, Node* right);
	void encode(Node* root, string code);
public:
	void getFrequencies(string& text);
	void updateFrequencies(char ch);
	Node* generateHuffmanTree();
	vector<string> getCodes(Node* root);
	Coding();
	~Coding();
};

