#include "observer.h"
#include <fstream>
#include <iostream>
#include "Stopwatch.hpp"

observer::observer()
{
}


bool operator==(const all_params& lhs, const all_params& rhs)
{
    return lhs.e_p.targetA == rhs.e_p.targetA &&
            lhs.e_p.targetB == rhs.e_p.targetB &&
            lhs.i_p.net_par.net_arc ==  rhs.i_p.net_par.net_arc &&
            lhs.p_p.mut_rate == rhs.p_p.mut_rate &&
            lhs.p_p.mut_step == rhs.p_p.mut_step &&
            lhs.p_p.number_of_inds == rhs.p_p.number_of_inds &&
            lhs.s_p.change_freq == rhs.s_p.change_freq &&
            lhs.s_p.seed == rhs.s_p.seed &&
            lhs.s_p.selection_strength == rhs.s_p.selection_strength;

}

bool operator!=(const all_params& lhs, const all_params& rhs)
{
    return !(lhs == rhs);
}



void observer::store_avg_fit_and_env(const simulation& s)
{
    m_avg_fitnesses.push_back(avg_fitness(s));
    m_var_fitnesses.push_back(var_fitness(s));
    m_env_values.push_back(get_current_env_value(s));
}

void observer::save_best_n_inds(const simulation &s)
{
    auto best_inds = get_best_n_inds(s, m_n_inds);
    std::mt19937_64 rng;

    std::vector<ind_data> ind_data_v(best_inds.size());

    for(auto i = best_inds.begin(); i != best_inds.end(); i++)
    {
        ind_data_v[std::distance(best_inds.begin(), i)] = ind_data{*i, s.get_time()};
    }

    m_top_inds.push_back(ind_data_v);
}

void observer::save_best_n_inds_mut_spectrum(const simulation &s)
{
    auto best_inds = get_best_n_inds(s, m_n_inds);
    std::mt19937_64 rng;

    std::vector<ind_spectrum> ind_data_v(best_inds.size());

    for(auto i = best_inds.begin(); i != best_inds.end(); i++)
    {
        ind_data_v[std::distance(best_inds.begin(), i)] = ind_spectrum{*i,
                calculate_mutational_spectrum(*i,
                                              s.get_params().p_p.mut_step,
                                              m_n_mutations,
                                              rng,
                                              m_n_bins,
                                              {-1,1}),
                s.get_time()
    };

    }

    m_top_inds_spectrum.push_back(ind_data_v);
}

void save_json(const observer& o, const std::string& filename)
{
    std::ofstream  f(filename);
    nlohmann::json json_out;
    json_out = o;
    f << json_out;
}

void exec(simulation& s , observer& o)
{
    stopwatch::Stopwatch sw;
    o.store_par(s);

    while(s.get_time() < s.get_n_gen())
    {
        tick(s);
        o.store_avg_fit_and_env(s);
        if(s.get_time() % o.m_best_ind_saving_freq  == 0)
        {
            o.save_best_n_inds(s);
        }
        if(s.get_time() % o.m_best_ind_spectrum_saving_freq == 0)
        {
            o.save_best_n_inds_mut_spectrum(s);
            std::cout << "Cycle " << s.get_time() << ". Elapsed: " << sw.lap<stopwatch::s>() << " seconds." << std::endl;
        }
    }
}

#ifndef NDEBUG
void test_observer()
{
#define FIX_ISSUE_47
#ifdef FIX_ISSUE_47
    ///An observer can store the sim_param of a simulation
    {
        observer o;
        //Give sim some non-default params
        env_param e_p{132465, 123465};
        all_params params = {e_p,{},{},{}};

        simulation s{params};
        assert(o.get_params() != params);

        o.store_par(s);

        assert(o.get_params() == params);
    }
#endif


    ///Simualtions have one rng for environmetal switches that is always seeded to 0
    {
        int generations = 200;
        double change_f = 0.9;

        all_params params{};
        params.s_p.change_freq = change_f;
        params.s_p.n_generations = generations;
        params.p_p.number_of_inds = 10;

        params.s_p.seed = 1;
        simulation lhs{params};
        observer o_lhs;
        exec(lhs, o_lhs);

        params.s_p.seed = 2;
        simulation rhs{params};
        observer o_rhs;
        exec(rhs, o_rhs);

        assert(o_lhs.get_env_values() == o_rhs.get_env_values());


    }

}
#endif
