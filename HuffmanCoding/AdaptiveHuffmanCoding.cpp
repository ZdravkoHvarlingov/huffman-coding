#include "AdaptiveHuffmanCoding.h"
#include <algorithm>
#include <assert.h>
#include <bitset>
#include <sstream>

AdaptiveHuffmanCoding::AdaptiveHuffmanCoding()
	: root(new HuffmanNode(-1, 0, 512, nullptr, nullptr, nullptr, true))
{
	NYTNode = root;
}

void AdaptiveHuffmanCoding::UpdateTreeModel(int symbol)
{
	HuffmanNode *leafToIncrement = nullptr;
	HuffmanNode *symbolNode = GetSymbolNode(symbol, root);

	if (symbolNode == nullptr)
	{
		NYTNode->isNYT = false;
		int currentMinOrder = NYTNode->order;
		NYTNode->left = new HuffmanNode(-1, 0, currentMinOrder - 2, nullptr, nullptr, NYTNode, true);
		NYTNode->right = new HuffmanNode(symbol, 0, currentMinOrder - 1, nullptr, nullptr, NYTNode, false);
		symbolNode = NYTNode->right;
		NYTNode = NYTNode->left;
	}
			
	SlideAndIncrement(symbolNode);	
}

std::string AdaptiveHuffmanCoding::Encode(int symbol)
{
	HuffmanNode *symbolNode = GetSymbolNode(symbol, root);
	if (symbolNode != nullptr)
	{
		std::string result = GetPathToSymbol(root, symbolNode, "");
		UpdateTreeModel(symbol);
		return result;
	}
	
	std::stringstream ss;
	ss << GetPathToSymbol(root, NYTNode, "") << std::bitset<9>(symbol);

	UpdateTreeModel(symbol);
	return ss.str();
}

int AdaptiveHuffmanCoding::Decode(std::istream &inputStr)
{
	int result = -1;
	HuffmanNode *crr = root;
	while (result == -1)
	{
		if (crr->value != -1)
		{
			result = crr->value;
			UpdateTreeModel(crr->value);
			crr = root;
		}
		else if (crr->isNYT)
		{
			char numberArr[10];
			inputStr.getline(numberArr, 10);
			inputStr.clear();
			int number = std::stoi(numberArr, nullptr, 2);
			result = number;
			UpdateTreeModel(number);
			crr = root;
		}
		else if(inputStr.get() == '0')
		{
			crr = crr->left;
		}
		else crr = crr->right;
	}

	return result;
}

AdaptiveHuffmanCoding::~AdaptiveHuffmanCoding()
{
	DeleteHuffmanTree(root);
}

AdaptiveHuffmanCoding::HuffmanNode * AdaptiveHuffmanCoding::GetSymbolNode(int symbol, HuffmanNode *crr) const
{
	if (crr == nullptr || crr->value == symbol) 
	{
		return crr;
	}

	HuffmanNode *leftResult = GetSymbolNode(symbol, crr->left);
	
	return leftResult == nullptr ? GetSymbolNode(symbol, crr->right) : leftResult;
}

void AdaptiveHuffmanCoding::FindBlockLeader(HuffmanNode *crr, HuffmanNode *&crrMax) const
{
	if (crr == nullptr || crrMax == root)
	{
		return;
	}

	if (crr->weight == crrMax->weight && crr != crrMax->parent  && crr->order > crrMax->order)
	{
		crrMax = crr;
	}

	FindBlockLeader(crr->left, crrMax);
	FindBlockLeader(crr->right, crrMax);
}

void AdaptiveHuffmanCoding::SwapNodes(HuffmanNode *first, HuffmanNode *second)
{
	if (first->parent == nullptr || second->parent == nullptr) return;
	
	if (first->parent == second || second->parent == first) return;

	HuffmanNode *&firstRef = first->parent->left == first ? first->parent->left : first->parent->right;
	HuffmanNode *&secondRef = second->parent->left == second ? second->parent->left : second->parent->right;

	std::swap(firstRef, secondRef);
	std::swap(firstRef->parent, secondRef->parent);
	std::swap(firstRef->order, secondRef->order);
}

std::string AdaptiveHuffmanCoding::GetPathToSymbol(HuffmanNode *crr, HuffmanNode *result, std::string currentPath) const
{
	if (crr == result)
	{
		return currentPath;
	}
	if (crr == nullptr)
	{
		return "";
	}
	
	std::string left = GetPathToSymbol(crr->left, result, currentPath + "0");
	return left != "" ? left : GetPathToSymbol(crr->right, result, currentPath + "1");
}

void AdaptiveHuffmanCoding::SlideAndIncrement(HuffmanNode *node)
{
	if (node == nullptr)
	{
		return;
	}

	HuffmanNode *blockLeader = node;
	FindBlockLeader(root, blockLeader);
	if (blockLeader != node)
	{
		SwapNodes(blockLeader, node);
	}

	++node->weight;
	SlideAndIncrement(node->parent);
}

void AdaptiveHuffmanCoding::DeleteHuffmanTree(HuffmanNode *crrNode)
{
	if (crrNode == nullptr) return;

	DeleteHuffmanTree(crrNode->left);
	DeleteHuffmanTree(crrNode->right);

	delete crrNode;
}

