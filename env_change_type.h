#ifndef ENV_CHANGE_TYPE_H
#define ENV_CHANGE_TYPE_H


#include <map>
#include <string>

enum class env_change_type
{
    two_optima,
    drift,
    None
};

static std::map<std::string, env_change_type> string_to_env_change_map
{
    {"two_optima",env_change_type::two_optima},
    {"drift", env_change_type::drift},
    {"None", env_change_type::None}
};

std::string convert_env_change_type_to_string(env_change_type e);

#endif // ENV_CHANGE_TYPE_H
