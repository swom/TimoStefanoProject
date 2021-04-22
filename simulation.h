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
             int t_change_interval = 10,
             std::vector<int> net_arch = {1,2,1}
          );


  ///Returns const ref ot population memeber
  const population& get_pop() const noexcept {return m_population;}

  ///Returns const ref ot population memeber
  population& get_pop() noexcept {return m_population;}

  ///Returns ref to rng
  std::minstd_rand& get_rng() noexcept {return m_rng;}

  ///Returns const ref to env_member
  const environment& get_env() const noexcept {return m_environment;}

  ///Returns const ref to env_member
  environment& get_env() noexcept {return m_environment;}

  ///returns const ref to
  const std::bernoulli_distribution& get_t_change_env_distr() const noexcept {return m_t_change_env_distr;}
  std::bernoulli_distribution& get_t_change_env_distr() noexcept {return m_t_change_env_distr;}

  private:
   environment m_environment;
   population m_population;
   std::minstd_rand m_rng;
   std::bernoulli_distribution m_t_change_env_distr;

};

///Calculates fitness of inds in pop given current env values
void calc_fitness(simulation& s);

///Changes all the weights of a given individual to a given value
void change_all_weights_nth_ind(simulation& s, size_t ind_index, double new_weight);

///Changes the network of the nth individual for a given network
void change_nth_ind_net(simulation& s, size_t ind_index, const network& n);

///Returns the current target value of the environment in the simualtion
double get_current_env_value(const simulation&s);

///Returns the current target value of the environment in the simualtion
double& get_current_env_value(simulation&s);

///Returns the individuals in the simualtion
const std::vector<individual>& get_inds(const simulation&s);

///Returns the fitness of the nth ind in pop
double get_nth_ind_fitness(const simulation& s, const size_t ind_index);

///Returns const or non-onst ref to the network of the nth individual in the
/// popoulation member of a simulation
const network& get_nth_ind_net(const simulation& s, size_t ind_index);
network& get_nth_ind_net( simulation& s, size_t ind_index);



void test_simulation() noexcept;

#endif // SIMULATION_H
