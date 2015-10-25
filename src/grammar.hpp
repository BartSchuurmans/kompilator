#pragma once

#include <map>

#include <boost/variant.hpp>

#include "token.hpp"

class grammar_rule_part {
public:
	boost::variant<class grammar_rule *, token_type> contents;
	bool at_least_once;
	bool at_most_once;

	grammar_rule_part(boost::variant<grammar_rule *, token_type> c, bool at_least_once, bool at_most_once);
	grammar_rule_part(boost::variant<grammar_rule *, token_type> c);
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
public:
	std::map<std::string, grammar_rule> rules;

	grammar_rule * get_rule_by_name(const std::string &name, bool create_if_not_exists = true);
};

grammar parse_grammar_definition(std::istream &input);

std::ostream& operator<<(std::ostream& str, const grammar &gr);
std::ostream& operator<<(std::ostream& str, const grammar_rule &rule);
std::ostream& operator<<(std::ostream& str, const grammar_rule_option &option);
std::ostream& operator<<(std::ostream& str, const grammar_rule_part &part);
