#pragma once

#include <string>
#include <unordered_map>

class HuffmanCoding
{
public:
	HuffmanCoding(bool = true);
	void SetEncodingParam(bool);
	void SetBinaryParam(bool);
	void SetInputFileName(const std::string&);
	void SetOutputFileName(const std::string&);
	void DoWork();
	double GetLastCompressRatio() const;

private:
	struct HuffmanNode
	{
		HuffmanNode *left;
		//charAscii is going to be -1 if the node is not a leaf(does not represent a specific character)
		int charAscii;
		int value;
		HuffmanNode *right;

		HuffmanNode(HuffmanNode*, int, int, HuffmanNode*);
	};

	struct NodeWrapper
	{
		HuffmanNode *node;

		NodeWrapper(HuffmanNode* = nullptr);
		bool operator>(const NodeWrapper&);
		bool operator<(const NodeWrapper&);
	};

	void DeleteHuffmanTree(HuffmanNode*);

	void Encode();
	std::string ReadTextFromInputFile();
	HuffmanNode* CreateHuffmanTree(std::unordered_map<char, int>);
	void GetEncodingTable(std::unordered_map<char, std::string>&, HuffmanNode*, std::string);
	void SerializeHuffmanTree(HuffmanNode*);
	void SerializeHuffmanNode(std::ostream& outStr, HuffmanNode*);
	int WriteEncodedTextInBinary(std::unordered_map<char, std::string>&, const std::string&);
	int WriteEncodedTextInDecimal(std::unordered_map<char, std::string>&, const std::string&);

	void Decode();
	std::string ReadInputBitString(int = 0);
	std::string GetBinaryStringFromDecimals(std::string, int);
	HuffmanNode* DeserializeHuffmanTree();
	HuffmanNode* DeserializeHuffmanNode(std::istream&);
	void WriteDecodedText(HuffmanNode*, std::string&, std::ostream&);

	std::string inputFileName;
	std::string outputFileName;
	bool shouldCompress;
	bool binaryFormat;
	double lastCompressRatio;
};