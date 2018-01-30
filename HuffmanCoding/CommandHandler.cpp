#include "CommandHandler.h"
#include <sstream>

void CommandHandler::Start()
{
	HuffmanCoding huffAlgorithm;
	huffAlgorithm.SetInputFileName("input.txt");
	huffAlgorithm.SetOutputFileName("output.txt");
	huffAlgorithm.SetEncodingParam(true);

	std::string line;
	std::getline(std::cin, line);
	std::stringstream ss;
	ss << line;

	std::string word;
	ss >> word;
	while (word != "-end")
	{
		if (word == "-encode")
		{
			huffAlgorithm.SetEncodingParam(true);
			std::cout << "Work set to Encoding.\n";
		}
		else if (word == "-decode")
		{
			huffAlgorithm.SetEncodingParam(false);
			std::cout << "Work set to Decoding.\n";
		}
		else if (word == "-inputFile")
		{
			ss >> word;
			huffAlgorithm.SetInputFileName(word);
			std::cout << "Input file set to: " << word << ".\n";
		}
		else if (word == "-outputFile")
		{
			ss >> word;
			huffAlgorithm.SetOutputFileName(word);
			std::cout << "Output file set to: " << word << ".\n";
		}
		else if (word == "-binary")
		{
			huffAlgorithm.SetBinaryParam(true);
			std::cout << "Encoding set to binary.\n";
		}
		else if (word == "-decimal")
		{
			huffAlgorithm.SetBinaryParam(false);
			std::cout << "Encoding set to decimal.\n";
		}
		else if (word == "-work")
		{
			huffAlgorithm.DoWork();
			std::cout << "Work finished. Last compression ratio is: " << huffAlgorithm.GetLastCompressRatio() * 100 << "%.\n";
		}
		else
		{
			std::cout << "Wrong command!\n";
		}

		std::getline(std::cin, line);
		ss.clear();
		ss << line;
		ss >> word;
	}
}
