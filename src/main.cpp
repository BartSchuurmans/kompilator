#include <stdlib.h>
#include <sysexits.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "lexer.hpp"

void
usage(char *executable)
{
	std::cerr << "Usage: " << executable << " <sourceFile>" << std::endl;
	exit(EX_USAGE);
}

int
main(int argc, char *argv[])
{
	if(argc != 2) {
		usage(argv[0]);
	}

	std::ifstream sourceFile(argv[1], std::ifstream::in);

	std::vector<Token *> tokens;
	try {
		tokens = tokenize(sourceFile);
	} catch(const TokenizeError &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	std::cout << "Tokens parsed:" << std::endl;
	for(Token *t : tokens) {
		std::cout << t->getContents() << "\t(" << t->getTypeName() << ")" << std::endl;
	}
}
