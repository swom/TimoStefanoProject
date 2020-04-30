#include "param.h"

#include<cassert>

param::param(int init_pop_size):
    m_init_pop_size{init_pop_size}
{

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
}
