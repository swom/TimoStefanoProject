#include "observer.h"
#include <fstream>
#include <iostream>

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

#ifndef NDEBUG
void test_observer()
{
#define FIX_ISSUE_47
#ifdef FIX_ISSUE_47
    ///An observer can store the sim_param of a simulation
    {
        observer o{{}};
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
        observer o_lhs{{}};
        exec(lhs, o_lhs);

        params.s_p.seed = 2;
        simulation rhs{params};
        observer o_rhs{{}};
        exec(rhs, o_rhs);

        assert(o_lhs.get_env_values() == o_rhs.get_env_values());


    }

    ///It is possible to save all_inds networks
    {
        observer o{{}};
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
