#include "parser.h"
#include <algorithm>
#include <cassert>

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
}
