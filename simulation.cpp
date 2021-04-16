#include "simulation.h"

#include <cassert>
#include <vector>

simulation::simulation(param parameters):
    m_param{parameters},
    m_environment{m_param.get_target_value()},
    m_population{m_param.get_init_pop_size()}
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
      assert(s.get_param().get_init_pop_size() > -100000);
    }

    //A simulation can be initialized with a parameter object
    {
        int init_pop = 42;
        double target_value = 0.42;
        param p(init_pop, target_value);
        simulation s(p);
        assert(s.get_param().get_init_pop_size() == init_pop);
        assert(s.get_param().get_target_value() - target_value < 0.0001 &&
               s.get_param().get_target_value() - target_value > -0.0001);


    }

#ifdef FIX_ISSUE_20
  //A simulation has an absolute counter and you can access it
  {
    simulation s;
    assert(s.get_absolute_time() > 0 | s.get_absolute_time() <= 0);
  }
#endif
}
