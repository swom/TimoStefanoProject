#include "population.h"

#include <cassert>

population::population(int init_nr_indiv, double mut_rate, double mut_step):
    m_vec_indiv(static_cast<unsigned int>(init_nr_indiv)),
    m_mut_rate{mut_rate},
    m_mut_step{mut_step}
{

}




void test_population() noexcept
{
    {
        int nelement = 10;
        population name{10};
        assert (static_cast<int>(name.get_ind_vec().size()) == nelement );
    }
    //A population has a member variable called m_mut_step
    //And m_mut_rate
    //By default initialized to 0.01 (mut_rate)
    // and 0.1 (mut_step)
    {
        population p;
        assert(p.get_mut_rate() == 0.01);
        assert(p.get_mut_step() == 0.1);

        auto mut_rate = 5.0;
        population p2{0,mut_rate};
        assert(p2.get_mut_rate() == mut_rate);

        auto mut_step = 5.0;
        population p3{0,0,mut_step};
        assert(p3.get_mut_step() == mut_step);
    }
}
