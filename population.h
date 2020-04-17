#ifndef POPULATION_H
#define POPULATION_H

#include "individual.h"

#include <vector>

class population
{
public:
    population(int init_nr_indiv = 1);

    ///Get vector of individuals
    const std::vector<individual>& get_ind_vec() const noexcept{return m_vec_indiv;}

private:

std::vector<individual> m_vec_indiv;

};

void test_population() noexcept;

#endif // POPULATION_H
