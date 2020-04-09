#include "simulation.h"

#include <cassert>

simulation::simulation(double init_target_value):
  m_environment{init_target_value}
{

}



void test_simulation() noexcept
{
simulation s;
assert (s.get_env().get_target_value()<1000000);





}
