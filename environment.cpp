#include "environment.h"
#include <cassert>

environment::environment(double init_target_value) :
  m_target_value{init_target_value}
{

}

void test_environment() noexcept
{
  //an environment has a m_target_value member
  //By default 0
  {
    environment e;
    assert(e.get_target_value() < 0.0000000001 && e.get_target_value() > -0.0000000001);

    double init_target_value = 0.42;
    environment e1(init_target_value);
    assert(e1.get_target_value() - init_target_value <  0.0000001 &&
           e1.get_target_value() - init_target_value > -0.0000001);
  }

}
