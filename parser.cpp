#include "parser.h"
#include <algorithm>
#include <cassert>

std::vector<int> arch_str_to_arch_vec(std::string net_arc)
{
    std::vector<int> net_arc_vec;
    size_t pos = 0;
    std::string delimiter = "-";
    std::string token;
    while ((pos = net_arc.find(delimiter)) != std::string::npos) {
        token = net_arc.substr(0, pos);
        net_arc_vec.push_back(std::stod(token));
        net_arc.erase(0, pos + delimiter.length());
    }
    net_arc_vec.push_back(std::stod(net_arc));

    return net_arc_vec;
}

double parse_change_freq(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--change_freq");
    if(value != args.end())
    {
        return std::stod(*(value + 1));
    }
    else{
        return -1156523;
    }
}

std::vector<int> parse_net_arc(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--net_arc");
    std::vector<int> net_arc;
    if(value != args.end())
    {
        return arch_str_to_arch_vec(*(value + 1));
    }
    else{
        return net_arc;
    }
}

int parse_pop_size(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--pop_size");
    if(value != args.end())
    {
        return std::stoi(*(value + 1));
    }
    else{
        return -1156523;
    }
}

int parse_seed(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--seed");
    if(value != args.end())
    {
        return std::stoi(*(value + 1));
    }
    else{
        return -1156523;
    }
}


int parse_sel_str(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--sel_str");
    if(value != args.end())
    {
        return std::stoi(*(value + 1));
    }
    else{
        return -1156523;
    }
}

double parse_targetA(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--targetA");
    if(value != args.end())
    {
        return std::stod(*(value + 1));
    }
    else{
        return -1156523;
    }
}

double parse_targetB(const std::vector<std::string>& args)
{
    auto value = std::find(args.begin(), args.end(),"--targetB");
    if(value != args.end())
    {
        return std::stod(*(value + 1));
    }
    else{
        return -1156523;
    }
}

void test_parser()
{
    ///The change of frequency of environment in the simualtion
    /// can be taken as an argument
    /// from a commnad line
    {

        double expected_change_freq = 0.321456;
        std::string string_expected_change_freq = std::to_string(expected_change_freq);

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "change_freq",
            string_expected_change_freq
        };


        assert(parse_change_freq(args_incorrect) != expected_change_freq);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--change_freq",
            string_expected_change_freq
        };
        assert(parse_change_freq(args_correct) == expected_change_freq);
    }

    /// The architecture of the network
    /// can be taken as an argument
    /// from a commnad line
    {

        std::vector<int> expected_net_architecture{1,23,45};
        std::string string_expected_architecture = "1-23-45";

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "net_arc",
            string_expected_architecture
        };


        assert(parse_net_arc(args_incorrect) != expected_net_architecture);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--net_arc",
            string_expected_architecture
        };
        assert(parse_net_arc(args_correct) == expected_net_architecture);
    }
    /// The targetA environment value
    /// can be taken as an argument
    /// from a commnad line
    {

        double expected_target_A{123.45};
        std::string string_expected_targetA = std::to_string(expected_target_A);

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "targetA",
            string_expected_targetA
        };


        assert(parse_targetA(args_incorrect) != expected_target_A);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--targetA",
            string_expected_targetA
        };
        assert(parse_targetA (args_correct) == expected_target_A);
    }

    /// The targetB environment value
    /// can be taken as an argument
    /// from a commnad line
    {

        double expected_target_B{123.45};
        std::string string_expected_targetB = std::to_string(expected_target_B);

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "targetA",
            string_expected_targetB
        };


        assert(parse_targetB(args_incorrect) != expected_target_B);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--targetB",
            string_expected_targetB
        };
        assert(parse_targetB(args_correct) == expected_target_B);
    }
    /// The population size
    /// can be taken as an argument
    /// from a commnad line
    {

        double expected_pop_size{12345};
        std::string string_expected_pop_size = std::to_string(expected_pop_size);

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "pop_size",
            string_expected_pop_size
        };


        assert(parse_pop_size(args_incorrect) != expected_pop_size);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--pop_size",
            string_expected_pop_size
        };
        assert(parse_pop_size(args_correct) == expected_pop_size);
    }
    /// The seed
    /// can be taken as an argument
    /// from a commnad line
    {

        double expected_seed{12345};
        std::string string_expected_seed = std::to_string(expected_seed);

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "seed",
            string_expected_seed
        };


        assert(parse_seed(args_incorrect) != expected_seed);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--seed",
            string_expected_seed
        };
        assert(parse_seed(args_correct) == expected_seed);
    }
    /// The selection strength
    /// can be taken as an argument
    /// from a commnad line
    {

        double expected_selection_strength{12345};
        std::string string_expected_selection_strength = std::to_string(expected_selection_strength);

        //will not take it because -- is missing from change_freq
        const std::vector<std::string>& args_incorrect{
            "random",
            "gibber",
            "seed",
            string_expected_selection_strength
        };


        assert(parse_sel_str(args_incorrect) != expected_selection_strength);

        const std::vector<std::string>& args_correct{
            "random",
            "gibber",
            "--sel_str",
            string_expected_selection_strength
        };
        assert(parse_sel_str(args_correct) == expected_selection_strength);
    }
}
