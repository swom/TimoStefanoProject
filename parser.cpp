#include "parser.h"
#include <algorithm>
#include <string>
#include <vector>
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
}
