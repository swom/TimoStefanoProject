#include "simulation.h"

#include <cassert>
#include <vector>

simulation::simulation(double init_target_value, int n_ind):
  m_environment{init_target_value},
  m_population{n_ind}
{

}



void test_simulation() noexcept//!OCLINT test may be many
{
  //Checks that simulation does have a environment member
  //The value 123456789 is irrelevant is just needed to call
  //get_env()
  {
    simulation s;//!OCLINT
    assert (s.get_env().get_target_value() < 123456789);
  }


  //A simulation has a member of type population
  //The population has a vector of individuals of size 1 by default
  {
    simulation s;
    assert(s.get_pop().get_ind_vec().size() == 1u);
  }

    //A simulation has a parameter member
    {
      simulation s;
      assert(s.get_param().get_init_pop_size() > 0);
    }
}
