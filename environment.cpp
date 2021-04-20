#include "environment.h"
#include "utilities.h"
#include <cassert>

environment::environment(double target_valueA, double target_valueB):

m_target_valueA{target_valueA},
m_target_valueB{target_valueB},
m_ref_target_values{target_valueA,target_valueB}



{




}

void test_environment() noexcept
{

  //an environment has a m_current_target_value member
  {
        double target_valueA = 0.123456;
        double target_valueB = 0.654321;
        environment e{target_valueA, target_valueB};
    assert(e.get_current_target_value() < 0 || e.get_current_target_value() > 0);
  }


  //an env has 2 reference target values;
  {
        double target_valueA = 0.123456;
        double target_valueB = 0.654321;
    environment e{target_valueA, target_valueB};
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


#ifdef FIX_ISSUE_29
  //Current target value is initialized to the first of the 2 target values
  {
    double targetA = 0.123456;
    double targetB = 0.654321;
    environment e{targetA,targetB};
    assert(are_equal_with_tolerance(e.get_current_target_value(), targetA));
    assert(are_not_equal_with_tolerance(e.get_current_target_value(), targetB));
  }
#endif

#ifdef FIX_ISSUE_25
  //An environment can switch target values
  {
    double targetA = 0.123456;
    double targetB = 0.654321;
    environment e{targetA,targetB};
    assert(are_equal_with_tolerance(e.get_current_target_value(), targetA));
    switc_target(e);
    assert(are_equal_with_tolerance(e.get_current_target_value(), targetB));
    switc_target(e);
    assert(are_equal_with_tolerance(e.get_current_target_value(), targetA));
  }
#endif
}
