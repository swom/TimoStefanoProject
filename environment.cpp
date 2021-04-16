#include "environment.h"
#include <cassert>

environment::environment()
{

}

void test_environment() noexcept
{

  //an environment has a m_current_target_value member
  {
    environment e;
    assert(e.get_current_target_value() < 0 || e.get_current_target_value() > 0);
  }

#ifdef FIX_ISSUE_24
  //an env has 2 reference target values;
  {
    environment e;
    assert(e.get_ref_target_values().size() == 2);
  }

  //an env can be initialized with 2 reference target values
  {
    double target_valueA = 0.123456;
    double target_valueB = 0.654321;
    environment e{target_valueA, target_valueB};

    assert(e.get_target_valueA() - target_valueA < 0.0001
           && e.get_target_valueA() - target_valueA > -0.0001);

    assert(e.get_target_valueB() - target_valueB < 0.0001
           && e.get_target_valueB() - target_valueB > -0.0001);
  }
#endif


}
