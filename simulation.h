#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"
#include "param.h"
#include "population.h"

#include <vector>

class simulation
{
public:

  simulation(int init_pop_size = 1,
             int seed = 0,
             int t_change_interval = 10
          );


  const population& get_pop() const noexcept {return m_population;}
  std::minstd_rand& get_rng() noexcept {return m_rng;}
  const environment& get_env() const noexcept {return m_environment;}
  const std::bernoulli_distribution& get_t_change_env_distr() const noexcept {return m_t_change_env_distr;}
  std::bernoulli_distribution& get_t_change_env_distr() noexcept {return m_t_change_env_distr;}

  private:
   environment m_environment;
   population m_population;
   std::minstd_rand m_rng;
   std::bernoulli_distribution m_t_change_env_distr;

};





void test_simulation() noexcept;

#endif // SIMULATION_H
