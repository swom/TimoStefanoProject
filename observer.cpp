#include "observer.h"
#include <fstream>
#include "Stopwatch.hpp"

observer::observer()
{
}

void observer::store_avg_fit_and_env(const simulation& s)
{
    m_avg_fitnesses.push_back(avg_fitness(s));
    m_var_fitnesses.push_back(var_fitness(s));
    m_env_values.push_back(get_current_env_value(s));
}

void observer::save_best_n_inds(const simulation &s, int n)
{
    m_top_inds.push_back(get_best_n_inds(s, n));
}

void save_json(const observer& o, const std::string& filename)
{
    std::ofstream  f(filename);
    nlohmann::json json_out;
    json_out = o;
    f << json_out;
}

void exec(simulation& s , observer& o, int n_generations)
{
    stopwatch::Stopwatch sw;
    for (int i = 0; i < n_generations; i++)
    {
        tick (s);
        o.store_avg_fit_and_env(s);
        if(i % 1000 == 0)
        {
            o.save_best_n_inds(s,10);
        }
        if(i % 1000 == 0)
        {
            std::cout << "Cycle " << i << ". Elapsed: " << sw.lap<stopwatch::s>() << " seconds." << std::endl;
        }
    }
}

#ifndef NDEBUG
void test_observer()
{
#ifdef FIX_ISSUE_47
    ///An observer can store the sim_param of a simulation
    {
        observer o;
        all_params params;
        simulation s{params};
        assert(o.get_params() != params);

        o.store_sim_par(s);

        assert(o.get_sim_par() == sim_par);
    }
#endif
}
#endif
