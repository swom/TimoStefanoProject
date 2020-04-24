#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"
#include "population.h"

#include <vector>

class simulation
{
public:
  simulation(double init_target_value = 0);

  ///Returns the environment of the simulation
  const environment& get_env() const noexcept {return m_environment;}
  const population& get_pop() const noexcept {return m_population;}

  private:

   environment m_environment;
   population m_population;

};





void test_simulation() noexcept;

#endif // SIMULATION_H
