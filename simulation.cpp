#include "simulation.h"

#include <cassert>
#include <vector>

simulation::simulation(double init_target_value, int init_pop_size):
  m_environment{init_target_value},
  m_population{init_pop_size}
{

}



void test_simulation() noexcept//!OCLINT test may be many
{
  ///Checks that simulation does have a environment member
  ///The value 123456789 is irrelevant is just needed to call
  ///get_env()
  {
    simulation s;//!OCLINT
    assert (s.get_env().get_target_value() < 123456789);
  }


    ///A simulation has a member of type population
    ///The population has a vector of individuals of size 1 by default
    {
      simulation s;
      assert(s.get_pop().get_ind_vec().size() == 1u);
    }
    ///A simulation has a member of type environment
    ///The environemtn has a target value of value 0 by default
    {
      simulation s;
      assert(s.get_env().get_target_value() == 0);
    }

  ///A simulation can be initialized by a
  /// target for the environment
  /// and the number of individuals in
  /// the populaation
    {
        double init_target_value = 1.23456;
        int init_pop_size = 123456;
        simulation s{init_target_value,init_pop_size};
        assert(s.get_env().get_target_value() == init_target_value
               && s.get_pop().get_ind_vec().size() == static_cast<unsigned int>(init_pop_size));
    }


}
