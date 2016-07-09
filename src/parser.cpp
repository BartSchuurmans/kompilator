#include <iterator>
#include <iostream>

#include "parser.hpp"

bool
parse_recursive(std::vector<token>::const_iterator &tok, const grammar_rule *rule, int level = 0)
{
	auto prefix = std::string(level * 3, ' ');

	std::cout << prefix << "Trying to parse " << rule->name << " at token [" << tok->get_type_name() << "]" << std::endl;
	for(auto const &option : rule->options) {
		std::cout << prefix << " Trying option" << option << std::endl;
		auto option_tok = tok;
		bool success = true;
		for(auto const &part : option.parts) {
			std::cout << prefix << "  Trying part " << part << std::endl;

			int found = 0;
			bool end = false;
again:
			if(part.is_rule()) {
				auto option_rule = part.get_rule();
				if(option_rule == rule && option_tok == tok) {
					// TODO: always valid?
					std::cout << "!" << prefix << "   Skipping rule " << option_rule->name << " that would recurse at the same token " << std::endl;
					success = false;
					break;
				}
				if(parse_recursive(option_tok, option_rule, level + 1)) {
					std::cout << prefix << "   Successfully parsed rule " << option_rule->name << "; new token is " << option_tok->get_type_name() << std::endl;
					found++;
				} else if(found > 0 || !part.at_least_once) {
					std::cout << prefix << "   Satisfied rule " << option_rule->name << " after " << found << " matches" << std::endl;
					end = true;
				} else {
					std::cout << prefix << "   Unable to parse rule " << option_rule->name << std::endl;
					success = false;
					break;
				}
			} else {
				if(option_tok->type == part.get_token_type()) {
					std::cout << prefix << "   Successfully parsed token " << option_tok->get_type_name() << std::endl;
					found++;
					option_tok++;	// XXX check for end
				} else {
					assert(part.at_least_once && part.at_most_once);	// TODO
					std::cout << prefix << "   Unable to parse token " << option_tok->get_type_name() << std::endl;
					success = false;
					break;
				}
			}
			if(!part.at_most_once && !end) {
				std::cout << prefix << "   Trying to parse more than " << found << " of part " << part << std::endl;
				goto again;
			}
		}
		if(success) {
			// FIXME try other options too
			std::cout << prefix << "  Successfully parsed " << rule->name << " with option " << option << std::endl;
			tok = option_tok;
			return true;
		}
	}
	std::cout << prefix << " Unable to parse " << rule->name << "; no option matched at token " << tok->get_type_name() << std::endl;
	return false;
}

void
parse(const std::vector<token> &tokens, const grammar_rule *root)
{
	auto token = tokens.cbegin();
	bool success = parse_recursive(token, root);
	if(!success) {
		throw std::runtime_error("Unable to parse " + root->name + " from tokens (no match)");
	}
	if(token != tokens.cend()) {
		throw std::runtime_error("Unable to parse " + root->name + " from tokens (leftover tokens)");
	}
	std::cout << "Success!" << std::endl;
}
