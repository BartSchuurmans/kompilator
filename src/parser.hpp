#pragma once

#include <vector>

#include "token.hpp"
#include "grammar.hpp"

void parse(const std::vector<token> &tokens, const grammar_rule *root);
