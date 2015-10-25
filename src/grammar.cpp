#include <string>
#include <iostream>
#include <regex>
#include <stdlib.h>

#include <boost/algorithm/string.hpp>

#include "grammar.hpp"

grammar_rule_part::grammar_rule_part(boost::variant<grammar_rule *, token_type> c, bool at_least_once, bool at_most_once)
	: contents(c)
	, at_least_once(at_least_once)
	, at_most_once(at_most_once)
{
}

grammar_rule_part::grammar_rule_part(boost::variant<grammar_rule *, token_type> c)
	: grammar_rule_part(c, true, true)
{
}

grammar_rule::grammar_rule(const std::string &name)
	: name(name)
{
}

grammar_rule *
grammar::get_rule_by_name(const std::string &name, bool create_if_not_exists /*  = true */)
{
	auto search = rules.find(name);
	if(search != rules.end()) {
		// Already exists
		return &search->second;
	}
	if(!create_if_not_exists) {
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
				option.parts.emplace_back(rule, false, true);
			} else if(m[2] == "*") {
				option.parts.emplace_back(rule, false, false);
			} else if(m[2] == "+") {
				option.parts.emplace_back(rule, true, false);
			} else {
				option.parts.emplace_back(rule, true, true);
			}
		} else if(std::regex_match(part, m, token_part)) {
			auto token_type = get_token_type_by_name(m[1]);
			option.parts.emplace_back(token_type);
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

std::ostream&
operator<<(std::ostream& str, const grammar &gr)
{
	for(auto const &it : gr.rules) {
		auto rule = it.second;
		str << rule << std::endl;
	}
	return str;
}

std::ostream&
operator<<(std::ostream& str, const grammar_rule &rule)
{
	str << rule.name << ":" << std::endl;
	for(auto const &option : rule.options) {
		str << "-" << option << std::endl;
	}
	return str;
}

std::ostream&
operator<<(std::ostream& str, const grammar_rule_option &option)
{
	for(auto const &part : option.parts) {
		str << " " << part;
	}
	return str;
}

std::ostream&
operator<<(std::ostream& str, const grammar_rule_part &part)
{
	if(part.contents.type() == typeid(token_type)) {
		str << get_token_type_name(boost::get<token_type>(part.contents));
	} else {
		// TODO: min/max
		auto rule = boost::get<grammar_rule *>(part.contents);
		str << "$" << rule->name;
	}
	return str;
}
