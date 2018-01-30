// HuffmanCoding.cpp : Defines the entry point for the console application.
//

//Needed it order to check for memory leaks
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
//-----------------------------------------

#include <iostream>
#include "HuffmanCoding.h"
#include "AdaptiveHuffmanCoding.h"
#include <sstream>
#include <fstream>
#include <assert.h>
#include "CommandHandler.h"

void TestAdaptiveHuffmanCoding()
{
	std::ifstream inputFile("AHuffmanTest.txt");
	
	AdaptiveHuffmanCoding encoder;
	std::stringstream encodedStream;
	std::stringstream text;
	char symbol = inputFile.get();
	while (!inputFile.eof())
	{
		text << symbol;
		encodedStream << encoder.Encode(symbol);
		
		symbol = inputFile.get();
	}
	encodedStream << encoder.Encode(AdaptiveHuffmanCoding::PSEUDO_EOF);

	AdaptiveHuffmanCoding decoder;
	std::stringstream decodedStream;
	int symbolAscii = decoder.Decode(encodedStream);
	while (symbolAscii != AdaptiveHuffmanCoding::PSEUDO_EOF)
	{
		decodedStream << (char)symbolAscii;

		symbolAscii = decoder.Decode(encodedStream);
	}
	//std::cout << text.str() << "\n" << encodedStream.str() << "\n" << decodedStream.str() << "\n";
	assert(text.str() == decodedStream.str());
	
	inputFile.close();
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //It checks for memory leaks

	TestAdaptiveHuffmanCoding();

	CommandHandler::Start();
    return 0;
}

