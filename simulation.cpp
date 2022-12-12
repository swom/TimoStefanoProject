#include "simulation.h"

#include <cassert>
#include <vector>
#include<ranges>


#ifndef NDEBUG
void test_simulation() noexcept//!OCLINT test may be many
{

    ///A simulation has a member of type population
    ///The population has a vector of individuals of size 1 by default
    {
        simulation s;
        assert(s.get_pop().get_inds().size() == 1u);
    }


#ifdef FIX_ISSUE_27
    ///A simulation can be initialized by
    /// target values for the environment
    /// and the number of individuals in
    /// the populaation
    {
        double target_valueA = 1.23456;
        double target_valueB = 6.12345;
        int init_pop_size = 123456;
        simulation s{target_valueA, target_valueB ,init_pop_size};
        assert(s.get_env().get_target_valueA() == target_valueA
               && s.get_env().get_target_valueB() == target_valueB
               && s.get_pop().get_ind_vec().size() == static_cast<unsigned int>(init_pop_size));
    }
#endif
    ////A simulation should have a random engine, intialized with a seed that is fed to simulation

    {
        double targetA = 1.0;
        double targetB = 0;
        int pop_size = 1;
        int seed = 123456789;
        simulation s{targetA,
                    targetB,
                    pop_size,
                    seed};
        std::mt19937_64 copy_rng(seed);
        assert ( s.get_mut_rng()() == copy_rng());

    }

    ///A simulation should have a t_change_env_distr bernoulli distribution
    /// that determines the interval of environmental change
    /// Default initialization value is 10
    {
        double targetA = 1.0;
        double targetB = 0;
        int pop_size = 1;
        int seed = 123456789;
        double t_change_interval = 0.2;

        simulation s { targetA, targetB,
                    pop_size, seed, t_change_interval};
        std::bernoulli_distribution mockdistrotchange(static_cast<double>(t_change_interval));
        assert (s.get_t_change_env_distr() == mockdistrotchange);

    }



    //A simulation has an absolute counter and you can access it
    {
        simulation s;
        assert((s.get_time() > 0) || (s.get_time() <= 0));
    }



    //Every tick simulation timer increases by one
    {
        simulation s;
        auto init_timer_value = s.get_time();
        tick(s);
        assert(s.get_time() == init_timer_value + 1);

        simulation s2;
        init_timer_value = s2.get_time();
        int repeats = 123;
        for(int i = 0; i != repeats; i++)
        {
            tick(s2);
        }
        assert(s2.get_time() == init_timer_value + repeats);
    }


    ///Simulation can be initialized with network architecture for inds in pop
    {
        std::vector<int> net_arch{1,33,2,1};
        simulation s{0,0,1,0,0, net_arch};

        assert(get_nth_ind_net(s, 0) == network{net_arch});
    }

#define FIX_ISSUE_30
#ifdef FIX_ISSUE_30
    ///individuals in a pop are selected based on how closely they match the current_env_target_value
    {
        int pop_size = 2;
        simulation s{0,0, pop_size};

        //change target value to match output of ind 0 net
        size_t best_ind = 0;
        change_current_target_value(s, response(get_nth_ind(s, best_ind))[0]);
        auto best_net = get_nth_ind_net(s, best_ind);
        auto resp_best = response(get_nth_ind(s, best_ind))[0];

        size_t worst_ind = 1;
        auto worst_net = change_all_weights(get_nth_ind_net(s,worst_ind), 100);
        change_nth_ind_net(s, worst_ind, worst_net);
        auto resp_worst = response(get_nth_ind(s, worst_ind))[0];

        assert(resp_best == get_current_env_value(s));
        assert(resp_worst != get_current_env_value(s));

        select_inds(s);

        //all inds should now have the network that matches the target values
        for(const auto& ind :get_inds(s))
        {
            assert(ind.get_net() == best_net);
        }
    }
#endif

    ///Fitness of individuals is calculated based on how close they are to the current target value
    {
        int pop_size = 2;
        simulation s{0,0,pop_size};

        size_t first_ind = 0;
        size_t second_ind = 1;
        change_all_weights_nth_ind(s, first_ind, 1);
        change_all_weights_nth_ind(s, second_ind, 0.99);


        //change target value to match output of ind 0 net
        change_current_target_value(s, response(get_nth_ind(s, 0))[0]);

        calc_fitness(s);

        ///ind 0 response should match exactly the target value therefore it will have fitness 1 (max)
        auto first_ind_fit =  get_nth_ind_fitness(s,0) ;
        assert(are_equal_with_tolerance( first_ind_fit, 1.0));

        ///ind 1 response is 0, therefore its fitness would be the lowest in all the population
        auto first_response = response(get_nth_ind(s, 0))[0];
        auto second_response = response(get_nth_ind(s, 1))[0];
        assert(!are_equal_with_tolerance(first_response, second_response));

        auto second_ind_fit =  get_nth_ind_fitness(s,1) ;
        auto min_fit = find_min_fitness(s);

        assert(are_equal_with_tolerance(min_fit,second_ind_fit));

    }

    //#define FIX_ISSUE_34
    {
        simulation s;
        int repeats = 100000;
        int n_switches = 0;
        for(int i = 0; i != repeats; i++)
        {
            if(is_environment_changing(s))
            {
                n_switches++;
            }
        }

        auto expected_repeats = s.get_change_freq() * repeats;
        assert(n_switches - expected_repeats < 20 &&
               n_switches - expected_repeats > -20);
    }


    //#define FIX_ISSUE_38

    {
        //sim_par
        int seed = 10126789;
        double change_freq = 123789;
        double selection_strength = 0.321546;
        int n_generations = 123465;

        sim_param  s_p{seed, change_freq, selection_strength, n_generations};
        all_params params{{}, {}, {}, s_p};
        simulation s{params};

        //test sim
        assert(are_equal_with_tolerance(s.get_change_freq(), change_freq) &&
               are_equal_with_tolerance(s.get_sel_str(), selection_strength) &&
               s.get_seed() == seed &&
               s.get_n_gen() == n_generations);
    }


#define FIX_ISSUE_39
#ifdef FIX_ISSUE_39

    {
        simulation s{0, 0.1, 0};
        int repeats =  100000;
        auto previous_env_value = get_current_env_value(s);

        int number_of_env_change = 0;

        for( int i = 0; i != repeats; i++)
        {
            tick(s);
            if(previous_env_value != get_current_env_value(s))
            {
                previous_env_value = get_current_env_value(s);
                number_of_env_change++;
            }
        }

        auto expected_changes = s.get_change_freq() * repeats;
        assert( number_of_env_change - expected_changes < repeats / 1000 &&
                number_of_env_change - expected_changes > -repeats / 1000);
    }
#endif

#define FIX_ISSUE_40
#ifdef FIX_ISSUE_40
    {
        //create a non-default simulaiton
        simulation s{13, 32, 2, 132, 548, {1,2,3,4,5,6}, 3.14};
        auto name = "sim_save_test";
        save_json(s, name);
        simulation loaded_s = load_json<environment<>>(name);
        assert(s == loaded_s);
    }
#endif

}
#endif


