#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"
#include "population.h"

#include <vector>

class simulation
{
public:
  simulation(double target_valueA = 0,
             double target_valueB = 1,
             int init_pop_size = 1,
             int seed = 0,
             int t_change_interval = 10
          );


  const population& get_pop() const noexcept {return m_population;}
  std::minstd_rand& get_rng() noexcept {return m_rng;}
  const environment& get_envA() const noexcept {return m_environmentA;}
  const environment& get_envB() const noexcept {return m_environmentB;}
  const std::geometric_distribution<int> get_t_change_env_distr() noexcept {return m_t_change_env_distr;}

  private:
   environment m_environmentA;
   environment m_environmentB;
   population m_population;
   std::minstd_rand m_rng;
   std::geometric_distribution<int> m_t_change_env_distr;

};





void test_simulation() noexcept;

#endif // SIMULATION_H
