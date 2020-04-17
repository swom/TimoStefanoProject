#include "simulation.h"

#include <cassert>

simulation::simulation(double init_target_value):
  m_environment{init_target_value}
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

#ifdef FIX_ISSUE_13
  //A simulation has a member of type population
  //The population has a vector of individuals of size 1 by default
  {
    simulation s;
    assert(s.get_pop().size() == 1u);
  }
#endif
}
