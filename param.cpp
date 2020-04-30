#include "param.h"

#include<cassert>

param::param(int init_pop_size, double target_value ):
    m_init_pop_size{init_pop_size},
    m_target_value{target_value}
{
assert(m_init_pop_size > 0);
assert(m_target_value >= 0);
}

void test_param() noexcept
{
    //Parameters can be intitialized with a given value for
    //starting population size
    //default 1
    {
        int init_pop_size = 42;
        param p(init_pop_size);
        assert(p.get_init_pop_size() == init_pop_size);
    }

    //Parameters can be intitialized with a given value for
    //starting target value
    //It has to be between 0 and 1
    //default 1
    {
        double target_value = 0.142;
        param p(1,target_value);
        assert(p.get_target_value() - target_value < 0.00001 &&
               p.get_target_value() - target_value > -0.00001);
    }
}
