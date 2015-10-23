#include <stdlib.h>
#include <sysexits.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "scanner.hpp"
#include "evaluator.hpp"

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

	// Open source file
	std::ifstream source_file(argv[1], std::ifstream::in);

	// Scanner
	std::vector<lexeme> lexemes;
	try {
		lexemes = scan(source_file);
	} catch(const scanner_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	std::cout << "Lexemes scanned:" << std::endl;
	for(auto &lme : lexemes) {
		if(lme.type == lexeme_type::WHITESPACE) {
			continue;
		}
		std::cout << lme.get_type_name() + ": " + lme.get_contents() << std::endl;
	}
	std::cout << std::endl;

	// Evaluator
	std::vector<token> tokens;
	try {
		tokens = evaluate(lexemes);
	} catch(const evaluator_error &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	std::cout << "Tokens evaluated:" << std::endl;
	for(auto &tok : tokens) {
		if(tok.type == token_type::WHITESPACE) {
			continue;
		}
		if(tok.has_contents()) {
			std::cout << tok.get_type_name() + "(" + tok.get_contents() << ")" << std::endl;
		} else {
			std::cout << tok.get_type_name() << std::endl;
		}
	}
	std::cout << std::endl;
}
