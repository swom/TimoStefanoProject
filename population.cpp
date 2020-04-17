#include "population.h"

#include <cassert>

population::population(int init_nr_indiv):

    m_vec_indiv (init_nr_indiv)
{

}




void test_population() noexcept
{
int nelement = 10;
population name{10};
 assert (static_cast<int>(name.get_ind_vec().size()) == nelement );

}
