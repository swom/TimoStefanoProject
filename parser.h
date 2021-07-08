#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include "observer.h"


std::vector<int> arch_str_to_arch_vec(std::string net_arc);

std::function<double(double)> parse_act_func(const std::vector<std::string>& args);

double parse_change_freq(const std::vector<std::string>& args);

env_param parse_env_param(const std::vector<std::string>& args);

ind_param parse_ind_param(const std::vector<std::string>& args);

double parse_mut_step(const std::vector<std::string>& args);

double parse_mut_rate(const std::vector<std::string>& args);

net_param parse_net_param(const std::vector<std::string>& args);

std::vector<int> parse_net_arc(const std::vector<std::string>& args);

int parse_n_generations(const std::vector<std::string>& args);

pop_param parse_pop_param(const std::vector<std::string>& args);

int parse_pop_size(const std::vector<std::string>& args);

int parse_seed(const std::vector<std::string>& args);

int parse_sel_str(const std::vector<std::string>& args);


double parse_targetA(const std::vector<std::string>& args);
double parse_targetB(const std::vector<std::string>& args);

sim_param parse_sim_param(const std::vector<std::string>& args);


void test_parser();
#endif // PARSER_H
