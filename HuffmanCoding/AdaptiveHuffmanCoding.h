#pragma once

#include <string>

class AdaptiveHuffmanCoding
{
public:
	AdaptiveHuffmanCoding();
	void UpdateTreeModel(int);
	std::string Encode(int);
	int Decode(std::istream&);
	
	~AdaptiveHuffmanCoding();
	//Used in order to specify the end of the compressed text => on live decompressing the decoder will know when to stop
	static const int PSEUDO_EOF = 256;
private:
	AdaptiveHuffmanCoding(const AdaptiveHuffmanCoding&); //both copy constructor and assignment operator are useless in this case
	AdaptiveHuffmanCoding& operator= (const AdaptiveHuffmanCoding&);

	struct HuffmanNode
	{
		int value;      //8-bit character contained in tree node
		int weight;   //number of times val has occured in file so far
		int order;    //ordering system to track weights

		HuffmanNode* left;
		HuffmanNode* right;
		HuffmanNode* parent;

		bool isNYT;

		HuffmanNode::HuffmanNode(int value, int weight, int order, HuffmanNode* left, HuffmanNode* right,
			HuffmanNode* parent, bool isNYT = false)
			:value(value), weight(weight), order(order), left(left), right(right), parent(parent), isNYT(isNYT)
		{}
		HuffmanNode::HuffmanNode()
			: value(0), weight(0), order(0), left(nullptr), right(nullptr), parent(nullptr), isNYT(false)
		{}

		HuffmanNode::HuffmanNode(bool isNYT, int value)
			: HuffmanNode()
		{
			this->isNYT = isNYT;
			this->value = value;
		}
	};

	HuffmanNode* GetSymbolNode(int, HuffmanNode*) const;
	void FindBlockLeader(HuffmanNode*, HuffmanNode*&) const;
	void SwapNodes(HuffmanNode*, HuffmanNode*);
	std::string GetPathToSymbol(HuffmanNode*, HuffmanNode*, std::string) const;
	void SlideAndIncrement(HuffmanNode*);

	void DeleteHuffmanTree(HuffmanNode*);

	HuffmanNode	*NYTNode;
	HuffmanNode *root;
};

const int AdaptiveHuffmanCoding::PSEUDO_EOF;