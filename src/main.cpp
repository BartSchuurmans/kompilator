#include <stdlib.h>
#include <sysexits.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "scanner.hpp"

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

	std::ifstream source_file(argv[1], std::ifstream::in);

	std::vector<lexeme> lexemes;
	try {
		lexemes = scan(source_file);
	} catch(const scanner_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	std::cout << "Lexemes scanned:" << std::endl;
	for(auto &lme : lexemes) {
		std::cout << "'" << lme.get_contents() << "'" << "\t(" << lme.get_type_name() << ")" << std::endl;
	}
}
