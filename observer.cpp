#include "observer.h"
#include <fstream>

observer::observer()
{
}

void observer::save_avg_fit_and_env(const simulation& s)
{
 m_avg_fitnesses.push_back(avg_fitness(s));
 m_var_fitnesses.push_back(var_fitness(s));
 m_env_values.push_back(get_current_env_value(s));
}

void observer::save_best_100_inds(const simulation &s)
{
    m_top_100_inds.push_back(get_best_n_inds(s,100));
}

void save_json(const observer& o, const std::string& filename)
{
  std::ofstream  f(filename);
  nlohmann::json json_out;
  json_out = o;
  f << json_out;
}
