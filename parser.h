#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include "observer.h"

//Version 2.1 of https://github.com/jarro2783/cxxopts
#include <cxxopts.hpp>

#include "Stopwatch.hpp"

///Executes a simulation for n generations
template<class S, class O>
void exec(S &s , O &o)
{
    using namespace stopwatch;
    Stopwatch sw;
    o.store_par(s);

    while(s.get_time() < s.get_n_gen())
    {
        tick(s);
        o.store_avg_fit_and_env(s);
        o.store_data_about_inds(s);
        if(s.get_time() % 1000 == 0)
        {
            std::cout << "Cycle " << s.get_time() << ". Elapsed: " << sw.lap<stopwatch::s>() << " seconds." << std::endl;
        }
    }
}







std::vector<int> arch_str_to_arch_vec(std::string net_arc);

///Creates a parser using the cxxopts parser
cxxopts::Options create_parser();

///Functions to conver the parsed arguments into parameter objects
///NOT TESTED!!!
env_param convert_env_args(const cxxopts::ParseResult& results);

ind_param convert_ind_args(const cxxopts::ParseResult& results);

net_param convert_net_args(const cxxopts::ParseResult& results);

pop_param convert_pop_args(const cxxopts::ParseResult& results);

sim_param convert_sim_args(const cxxopts::ParseResult& results);

obs_param convert_obs_args(const cxxopts::ParseResult& results);

///---------------------

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
