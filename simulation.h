#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"
#include "population.h"

#include <vector>

class simulation
{
public:
  simulation(double init_target_value = 0, int init_pop_size = 1, int seed = 0);

  ///Returns the environment of the simulation
  const environment& get_env() const noexcept {return m_environment;}
  const population& get_pop() const noexcept {return m_population;}
  std::minstd_rand& get_rng() noexcept {return m_rng;}

  private:

   environment m_environment;
   population m_population;
   std::minstd_rand m_rng;

};





void test_simulation() noexcept;

#endif // SIMULATION_H
