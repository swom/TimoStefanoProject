#include "env_change_type.h"
#include <stdexcept>


std::string convert_env_change_type_to_string(env_change_type e)
{
    std::string string;

    switch (e) {
    case env_change_type::two_optima :
        string = "two_optima";
        return string;
        break;

    case env_change_type::drift :
        string = "drift";
        return string;
        break;

    case env_change_type::None :
        string = "None";
        return string;
        break;

    default:
        throw std::runtime_error{"could not convert env_change_type type into string"};
        return "failed";
    }
}
