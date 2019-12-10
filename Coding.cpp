#include "Coding.h"

Coding::Node* Coding::createNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node;
	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}
void Coding::getFrequencies(string& text)
{
	for (int i = 0; i < text.length(); i++)
	{
		freqs[text[i]]++;
	}
}
void Coding::updateFrequencies(char ch)
{
	++freqs[ch];
}
Coding::Node* Coding::generateHuffmanTree()
{
	priority_queue<Node*, vector<Node*>, cmp> pQueue;
	for (auto i = freqs.begin(); i != freqs.end(); i++)
	{
		pQueue.push(createNode(i->first, i->second, NULL, NULL));
	}

	while (pQueue.size() != 1)
	{
		Node* left = pQueue.top();
		pQueue.pop();
		Node* right = pQueue.top();
		pQueue.pop();

		int newFreq = left->freq + right->freq;
		pQueue.push(createNode('\0', newFreq, left, right));
	}

	return pQueue.top();
}
void Coding::encode(Node* root, string code)
{
	if (root == NULL) return;
	if (root->left == NULL && root->right == NULL) codes[root->ch] = code;
	encode(root->left, code + "0");
	encode(root->right, code + "1");
}

unordered_map<char, string> Coding::getCodes(Node* root)
{
	encode(root, "");

	return codes;
}

Coding::Coding()
{
}


Coding::~Coding()
{
}
