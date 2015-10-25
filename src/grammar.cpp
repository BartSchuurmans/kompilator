#include <string>
#include <iostream>
#include <regex>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>

#include "grammar.hpp"

grammar_rule_part::grammar_rule_part(boost::variant<grammar_rule *, token_type> c, int min, int max)
	: contents(c)
	, min_occurs(min)
	, max_occurs(max)
{
}

grammar_rule::grammar_rule(const std::string &name)
	: name(name)
{
}

grammar_rule *
grammar::get_rule_by_name(const std::string &name, bool create /*  = true */)
{
	auto search = rules.find(name);
	if(search != rules.end()) {
		// Already exists
		return &search->second;
	}
	if(!create) {
		return nullptr;
	}

	auto result = rules.emplace(std::make_pair(std::string(name), grammar_rule(name)));
	return &result.first->second;
}

grammar_rule_option
parse_rule_option(grammar &gr, const std::string &str)
{
	auto option = grammar_rule_option();

	static const std::regex rule_part("^\\$([a-zA-Z]+)([?*+]?)$");
	static const std::regex token_part("^([A-Z_]+)$");

	std::vector<std::string> fields;
	boost::split(fields, str, boost::is_any_of(" "), boost::token_compress_on);
	for(auto const &part : fields) {
		std::smatch m;
		if(std::regex_match(part, m, rule_part)) {
			auto rule = gr.get_rule_by_name(m[1]);
			if(m[2] == "?") {
				option.parts.emplace_back(rule, 0, 1);
			} else if(m[2] == "*") {
				option.parts.emplace_back(rule, 0, -1);
			} else if(m[2] == "+") {
				option.parts.emplace_back(rule, 1, -1);
			} else {
				option.parts.emplace_back(rule, 1, 1);
			}
		} else if(std::regex_match(part, m, token_part)) {
			auto token_type = get_token_type_by_name(m[1]);
			option.parts.emplace_back(token_type, 1, 1);
		} else {
			throw std::runtime_error("Unable to parse option part: " + part);
		}
	}

	return option;
}

grammar
parse_grammar_definition(std::istream &input)
{
	auto gr = grammar();

	static const std::regex rule("^([a-zA-Z]+) = (.+)");
	static const std::regex rule_option("\\s*\\| (.+)");

	std::string line;
	grammar_rule *current = nullptr;
	while(std::getline(input, line).good()) {
		std::smatch m;
		if(std::regex_match(line, m, rule)) {
			// New rule
			current = gr.get_rule_by_name(m[1]);
			auto option = parse_rule_option(gr, m[2]);
			current->options.push_back(option);
		} else if(std::regex_match(line, m, rule_option)) {
			// Alternative for existing rule
			if(current == nullptr) {
				throw std::runtime_error("Alternative found before rule: " + line);
			}
			auto option = parse_rule_option(gr, m[1]);
			current->options.push_back(option);
		} else {
			throw std::runtime_error("Invalid line in grammar: " + line);
		}
	}

	return gr;
}