#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>

double parse_change_freq(const std::vector<std::string>& args);

std::vector<int> parse_net_arc(const std::vector<std::string>& args);

std::vector<int> arch_str_to_arch_vec(std::string net_arc);

void test_parser();
#endif // PARSER_H
