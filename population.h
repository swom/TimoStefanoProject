#ifndef POPULATION_H
#define POPULATION_H

#include "individual.h"
#include "rndutils.hpp"
#include <vector>

class population
{
public:
  population(int init_nr_indiv = 1,
             double mut_rate = 0.01,
             double mut_step = 0.1
             , std::vector<int> net_arch = {1,2,1});

  ///Get const ref to vector of individuals
  const std::vector<individual>& get_inds() const noexcept{return m_vec_indiv;}

  ///Get ref to vector of individuals
  std::vector<individual>& get_inds() noexcept{return m_vec_indiv;}

  ///Get vector of individuals
  const std::vector<individual>& get_new_inds() const noexcept{return m_vec_new_indiv;}

  ///Return mutation rate
  double get_mut_rate() const noexcept {return m_mut_rate;}

  ///Return mutation step
  double get_mut_step() const noexcept {return m_mut_step;}

private:

  std::vector<individual> m_vec_indiv;
  std::vector<individual> m_vec_new_indiv;
  double m_mut_rate;
  double m_mut_step;

};

///Calculates the fitness of inds in pop given a target env_value
population calc_fitness(population p, double env_value);

const individual& get_nth_ind(const population& p, size_t ind_index);
individual& get_nth_ind(population& p, size_t ind_index);

///Returns the fitness of the nth individual
double get_nth_ind_fitness(const population& p, const size_t& ind_index);

const network& get_nth_ind_net(const population& p, size_t ind_index);
network& get_nth_ind_net( population& p, size_t ind_index);

///Sets the fitness of the nth ind in the population
void set_nth_ind_fitness (population& p, size_t ind_index, double fitness);

void test_population() noexcept;

#endif // POPULATION_H
