#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"
#include "param.h"
#include "population.h"

#include <vector>

class simulation
{
public:
  simulation(double init_target_value = 0, int n_ind = 1);

  ///Returns the environment of the simulation
  const environment& get_env() const noexcept {return m_environment;}
  const population& get_pop() const noexcept {return m_population;}
  const param& get_param() const noexcept {return m_param;}


  private:

   environment m_environment;
   population m_population;
   param m_param;


};





void test_simulation() noexcept;

#endif // SIMULATION_H
