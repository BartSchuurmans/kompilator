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

	std::ifstream source_file(argv[1], std::ifstream::in);

	std::vector<token *> tokens;
	try {
		tokens = tokenize(source_file);
	} catch(const tokenize_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	std::cout << "Tokens parsed:" << std::endl;
	for(token *t : tokens) {
		std::cout << "'" << t->get_contents() << "'" << "\t(" << t->get_type_name() << ")" << std::endl;
	}
}
