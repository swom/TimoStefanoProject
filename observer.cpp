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

    std::vector<ind_spectrum> ind_spectrum_v(best_inds.size());

    for(auto i = best_inds.begin(); i != best_inds.end(); i++)
    {
        auto index = std::distance(best_inds.begin(), i);

        value_range range = {-1,1};
        ind_spectrum_v[index] = ind_spectrum{*i,
                response(*i),
                calculate_mutational_spectrum(*i,
                                              s.get_params().p_p.mut_step,
                                              m_n_mutations,
                                              rng,
                                              m_n_bins,
                                              range),
                s.get_time()
    };

    }

    m_top_inds_spectrum.push_back(ind_spectrum_v);
}

void observer::store_data_about_inds(simulation &s)
{
    ///after tick new population is in place,
    /// old populaiton is still stored in new_inds vector
    /// to save data from individuals that just went through the
    /// generation it is necessary to swap back old pop to ind vec
    s.get_pop().get_inds().swap(s.get_pop().get_new_inds());

    if(s.get_time() %m_best_ind_saving_freq  == 0)
    {
        save_best_n_inds(s);
    }
    if(s.get_time() < (400) ||
            s.get_time() > (s.get_n_gen() - 200))
    {
        save_best_n_inds_mut_spectrum(s);
        if(is_time_to_record_all_inds_nets(s)) store_all_inds_nets(s);
    }

    ///Swap back
    s.get_pop().get_inds().swap(s.get_pop().get_new_inds());
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
        o.store_data_about_inds(s);
        if(s.get_time() % 1000 == 0)
        {
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

    ///It is possible to save all_inds networks
    {
        observer o;
        simulation s;

        o.store_all_inds_nets(s);
        assert(o.get_all_inds_nets().back().m_nets.size() == s.get_pop().get_inds().size());
    }

    ///It is possible to save all individuals' networks every n generations
    {
        int generations = 200;
        int all_inds_rec_freq = 2;

        all_params params{};
        params.s_p.n_generations = generations;
        params.p_p.number_of_inds = 10;

        obs_param o_p;
        o_p.all_inds_rec_freq = all_inds_rec_freq;

        simulation s{params};
        observer o{o_p, params};
        exec(s, o);

        unsigned int expected_number_of_records = generations / all_inds_rec_freq;
        assert(o.get_all_inds_nets().size() == expected_number_of_records);
    }

}
#endif
