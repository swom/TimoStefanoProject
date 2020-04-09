#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"

class simulation
{
public:
  simulation(double init_target_value = 0);

   environment get_env() const noexcept {return m_environment;}

  private:

   environment m_environment;



};





void test_simulation() noexcept;

#endif // SIMULATION_H
