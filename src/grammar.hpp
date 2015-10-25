#pragma once

#include <map>

#include <boost/variant.hpp>

#include "token.hpp"

class grammar_rule_part {
public:
	boost::variant<class grammar_rule *, token_type> contents;
	int min_occurs;
	int max_occurs;

	grammar_rule_part(boost::variant<grammar_rule *, token_type> c, int min, int max);
};

class grammar_rule_option {
public:
	std::vector<grammar_rule_part> parts;
};

class grammar_rule {
public:
	const std::string name;
	std::vector<grammar_rule_option> options;

	grammar_rule(const std::string &name);
};

class grammar {
	std::map<std::string, grammar_rule> rules;

public:
	grammar_rule * get_rule_by_name(const std::string &name, bool create = true);
};

grammar parse_grammar_definition(std::istream &input);
