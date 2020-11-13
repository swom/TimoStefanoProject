#include "simulation.h"

#include <cassert>
#include <vector>

simulation::simulation(double target_valueA,
                       double target_valueB,
                       int init_pop_size,
                       int seed,
                       int t_change_interval):
    m_environmentA{target_valueA},
    m_environmentB{target_valueB},
    m_population{init_pop_size},
    m_rng{seed},
    m_t_change_env_distr{1.0/static_cast<double>(t_change_interval)}
{

}



void test_simulation() noexcept//!OCLINT test may be many
{
    ///Checks that simulation does have two environment members, environmentA and environmentB
    ///The value 123456789 is irrelevant is just needed to call
    ///get_env()
    {
        simulation s;//!OCLINT
        assert (s.get_envA().get_target_value() < 123456789);
        assert (s.get_envB().get_target_value() < 123456789);
    }


    ///A simulation has a member of type population
    ///The population has a vector of individuals of size 1 by default
    {
        simulation s;
        assert(s.get_pop().get_ind_vec().size() == 1u);
    }
    ///A simulation has two members of type environment envA and envB
    ///The environementA has a target value of value 0 by default environmentB has a target value of 1 by default
    {
        simulation s;
        assert(s.get_envA().get_target_value() == 0);
        assert(s.get_envB().get_target_value() == 1);
    }

    ///A simulation can be initialized by a
    /// target for the environment
    /// and the number of individuals in
    /// the populaation
    {
        double target_valueA = 1.23456;
        double target_valueB = 6.12345;
        int init_pop_size = 123456;
        simulation s{target_valueA, target_valueB ,init_pop_size};
        assert(s.get_envA().get_target_value() == target_valueA
               && s.get_envB().get_target_value() == target_valueB
               && s.get_pop().get_ind_vec().size() == static_cast<unsigned int>(init_pop_size));
    }

    ////A simulation should have a random engine, intialized with a seed that is fed to simulation

    {
        double targetA = 1.0;
        double targetB = 0;
        int pop_size = 1;
        int seed = 123456789;
        simulation s{targetA, targetB, pop_size,seed};

        std::minstd_rand copy_rng(seed);
        assert ( s.get_rng()() == copy_rng());

    }

    ///A simulation should have a t_change_env_distr geometric distribution
    /// that determines the interval of environmental change
    /// Default initialization value is 10
    {
        double targetA = 1.0;
        double targetB = 0;
        int pop_size = 1;
        int seed = 123456789;
        int t_change_interval = 20;

        simulation s { targetA, targetB, pop_size, seed, t_change_interval};
        std::geometric_distribution<int> mockdistrotchange(1.0 / static_cast<double>(t_change_interval));
        assert (s.get_t_change_env_distr() == mockdistrotchange);

    }




}
