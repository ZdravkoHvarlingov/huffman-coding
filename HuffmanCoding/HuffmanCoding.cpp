#include "HuffmanCoding.h"
#include <assert.h>
#include "MinHeap.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <bitset>

HuffmanCoding::HuffmanCoding(bool shouldCompress)
	: shouldCompress(shouldCompress), binaryFormat(true), lastCompressRatio(0)
{
}

void HuffmanCoding::SetEncodingParam(bool shouldCompress)
{
	this->shouldCompress = shouldCompress;
}

void HuffmanCoding::SetBinaryParam(bool binaryFormat)
{
	this->binaryFormat = binaryFormat;
}

void HuffmanCoding::SetInputFileName(const std::string &inputFile)
{
	inputFileName = inputFile;
}

void HuffmanCoding::SetOutputFileName(const std::string &outputFile)
{
	outputFileName = outputFile;
}

void HuffmanCoding::DoWork()
{
	if (shouldCompress)
	{
		Encode();
	}
	else Decode();
}

double HuffmanCoding::GetLastCompressRatio() const
{
	return lastCompressRatio;
}

HuffmanCoding::HuffmanNode * HuffmanCoding::CreateHuffmanTree(std::unordered_map<char, int> symbolTable)
{
	std::vector<NodeWrapper> heapifyVector;

	for (std::unordered_map<char, int>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it)
	{
		heapifyVector.push_back(NodeWrapper(new HuffmanNode(nullptr, it->first, it->second, nullptr)));
	}

	MinHeap<NodeWrapper> minHeap;
	minHeap.Heapify(heapifyVector);
	while (minHeap.GetSize() > 1)
	{
		NodeWrapper fst = minHeap.Pop();
		NodeWrapper snd = minHeap.Pop();

		NodeWrapper newNode(new HuffmanNode(fst.node, -1, fst.node->value + snd.node->value, snd.node));
		minHeap.Push(newNode);
	}

	if (minHeap.GetSize() == 0) return nullptr;

	return minHeap.Pop().node;
}

void HuffmanCoding::DeleteHuffmanTree(HuffmanNode *crrNode)
{
	if (crrNode == nullptr) return;

	DeleteHuffmanTree(crrNode->left);
	DeleteHuffmanTree(crrNode->right);

	delete crrNode;
}

void HuffmanCoding::SerializeHuffmanTree(HuffmanNode *node)
{
	std::ofstream file(outputFileName, std::ofstream::app);
	file << "\n";
	SerializeHuffmanNode(file, node);

	file.close();
}


void HuffmanCoding::SerializeHuffmanNode(std::ostream & outStr, HuffmanNode *node)
{
	if (node == nullptr)
	{
		outStr << "NULL";
		return;
	}

	outStr << "(";
	SerializeHuffmanNode(outStr, node->left);
	outStr << ", " << node->charAscii << " , " << node->value << " , ";
	SerializeHuffmanNode(outStr, node->right);
	outStr << ")";
}

int HuffmanCoding::WriteEncodedTextInBinary(std::unordered_map<char, std::string> &table, const std::string &text)
{
	assert(outputFileName != "");
	std::ofstream file(outputFileName);
	int amountOfBits = 0;
	for (size_t ind = 0; ind < text.length(); ind++)
	{
		file << table[text[ind]];
		amountOfBits += table[text[ind]].length();
	}

	file.close();
	return amountOfBits;
}

int HuffmanCoding::WriteEncodedTextInDecimal(std::unordered_map<char, std::string> &table, const std::string& text)
{
	assert(outputFileName != "");
	std::ofstream file(outputFileName);
	int amountOfBits = 0;
	std::string binString = "";
	for (size_t ind = 0; ind < text.length(); ind++)
	{
		binString.append(table[text[ind]]);
	}
	amountOfBits = binString.length();

	while (binString.length() >= 8)
	{
		int number = std::stoi(binString.substr(0, 8), nullptr, 2);
		file << number;
		binString = binString.substr(8, binString.length() - 8);
		if (binString != "") file << " ";
	}

	if (binString == "")
	{
		file << "\n" << 8;
	}
	else
	{
		file << std::stoi(binString, nullptr, 2) << "\n" << binString.length();
	}
	file.close();

	return amountOfBits;
}

HuffmanCoding::HuffmanNode * HuffmanCoding::DeserializeHuffmanTree()
{
	std::stringstream treeStream(binaryFormat ? ReadInputBitString(1) : ReadInputBitString(2));

	return DeserializeHuffmanNode(treeStream);
}

HuffmanCoding::HuffmanNode * HuffmanCoding::DeserializeHuffmanNode(std::istream &inputStr)
{
	assert(inputStr.peek() == '(' || inputStr.peek() == 'N');

	if (inputStr.peek() == 'N')
	{
		inputStr.get(); inputStr.get(); inputStr.get(); inputStr.get(); //remove NULL
		return nullptr;
	}

	inputStr.get(); //remove the open bracket
	HuffmanNode *left = DeserializeHuffmanNode(inputStr);
	inputStr.get(); //remove the comma
	
	int asciiCode;
	inputStr >> asciiCode;
	inputStr.get(); //remove space after the number 
	inputStr.get(); //remove comma

	int value;
	inputStr >> value;
	inputStr.get(); //remove space after the number
	inputStr.get(); //remove comma
	inputStr.get(); //remove space

	HuffmanNode *right = DeserializeHuffmanNode(inputStr);
	inputStr.get(); //remove the closing bracket

	return new HuffmanNode(left, asciiCode, value, right);
}

void HuffmanCoding::WriteDecodedText(HuffmanNode *root, std::string &bitString, std::ostream &outStr)
{
	HuffmanNode *crr = root;
	size_t ind = 0;
	while (crr != nullptr && ind <= bitString.size())
	{
		if (crr->charAscii != -1)
		{
			outStr << (char)crr->charAscii;
			crr = root;
		}
		else if (bitString[ind] == '0')
		{
			crr = crr->left;
			++ind;
		}
		else
		{
			crr = crr->right;
			++ind;
		}
	}
}

std::string HuffmanCoding::ReadTextFromInputFile()
{
	assert(inputFileName != "");

	std::ifstream file(inputFileName);
	std::string text;

	while (!file.eof())
	{
		std::string currentLine;
		std::getline(file, currentLine);

		text += currentLine + "\n";
	}

	return text.substr(0, text.length() - 1);
}

void HuffmanCoding::GetEncodingTable(std::unordered_map<char, std::string>& table, HuffmanNode *crrNode,
	std::string crrCode)
{
	if (crrNode == nullptr)
	{
		return;
	}

	if (crrNode->charAscii != -1)
	{
		table[(char)crrNode->charAscii] = crrCode;
		return;
	}

	GetEncodingTable(table, crrNode->left, crrCode + "0");
	GetEncodingTable(table, crrNode->right, crrCode + "1");
}

void HuffmanCoding::Encode()
{
	std::string text = ReadTextFromInputFile();

	std::unordered_map<char, int> symbolTable;
	for (size_t ind = 0; ind < text.length(); ind++)
	{
		if (symbolTable.count(text[ind]) == 0)
		{
			symbolTable[text[ind]] = 0;
		}

		++symbolTable[text[ind]];
	}

	HuffmanNode *tree = CreateHuffmanTree(symbolTable);
	
	std::unordered_map<char, std::string> table;
	GetEncodingTable(table, tree, "");
	
	int amountOfBits = binaryFormat ? WriteEncodedTextInBinary(table, text) : WriteEncodedTextInDecimal(table, text);

	lastCompressRatio = (double)amountOfBits / (text.length() * sizeof(char) * 8);
	SerializeHuffmanTree(tree);
	DeleteHuffmanTree(tree);
}

void HuffmanCoding::Decode()
{
	std::string inputBitString = ReadInputBitString();
	HuffmanNode *root = DeserializeHuffmanTree();

	assert(outputFileName != "");
	std::ofstream file(outputFileName);
	WriteDecodedText(root, inputBitString, file);
	file.close();

	DeleteHuffmanTree(root);
}

std::string HuffmanCoding::ReadInputBitString(int line)
{
	int lineCopy = line;
	assert(inputFileName != "");

	std::ifstream file(inputFileName);
	std::string text;
	while (line >= 0)
	{
		std::getline(file, text);
		--line;
	}
	

	if (lineCopy == 0 && binaryFormat == false)
	{
		std::string secondLine;
		std::getline(file, secondLine);
		int amountOfLastBits = std::stoi(secondLine);

		text = GetBinaryStringFromDecimals(text, amountOfLastBits);
	} 

	file.close();
	return text;
}

std::string HuffmanCoding::GetBinaryStringFromDecimals(std::string text, int amountOfLastBits)
{
	std::stringstream ss;
	ss << text;
	text = "";
	int number;
	ss >> number;
	while (!ss.eof())
	{
		text += std::bitset<8>(number).to_string();
		ss >> number;
	}
	text += std::bitset<8>(number).to_string();
	text = text.substr(0, text.length() - 8) + text.substr(text.length() - amountOfLastBits, amountOfLastBits);

	return text;
}

HuffmanCoding::NodeWrapper::NodeWrapper(HuffmanNode *node)
	: node(node)
{
}

bool HuffmanCoding::NodeWrapper::operator>(const NodeWrapper &other)
{
	return node->value > other.node->value;
}

bool HuffmanCoding::NodeWrapper::operator<(const NodeWrapper &other)
{
	return node->value < other.node->value;
}

HuffmanCoding::HuffmanNode::HuffmanNode(HuffmanNode *left, int charAscii, int value, HuffmanNode *right)
	: left(left), charAscii(charAscii), value(value), right(right)
{
}
