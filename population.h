#ifndef POPULATION_H
#define POPULATION_H

#include "individual.h"

#include <vector>

class population
{
public:
    population(int init_nr_indiv = 1, double mut_rate = 0.01, double mut_step = 0.1);

    ///Get vector of individuals
    const std::vector<individual>& get_ind_vec() const noexcept{return m_vec_indiv;}

    ///Return mutation rate
    double get_mut_rate() const noexcept {return m_mut_rate;}

    ///Return mutation step
    double get_mut_step() const noexcept {return m_mut_step;}

private:

std::vector<individual> m_vec_indiv;
double m_mut_rate;
double m_mut_step;

};

void test_population() noexcept;

#endif // POPULATION_H
