#include <cassert>
#include "environment.h"

#ifndef NDEBUG
void test_environment() noexcept
{

  //an environment has a m_current_target_value member
  {
    double target_valueA = 0.123456;
    double target_valueB = 0.654321;
    environment e{target_valueA, target_valueB};
    assert(e.get_current_optimum() < 0 || e.get_current_optimum() > 0);
  }


  //an env has 2 reference target values;
  {
    double target_valueA = 0.123456;
    double target_valueB = 0.654321;
    environment e{target_valueA, target_valueB};
    assert(e.get_ref_target_values().first == target_valueA);
    assert(e.get_ref_target_values().second == target_valueB);
  }

  //an env can be initialized with 2 reference target values
  {
    double target_valueA = 0.123456;
    double target_valueB = 0.654321;
    environment e{target_valueA, target_valueB};

    assert(get_target_valueA(e) - target_valueA < 0.0001
           && get_target_valueA(e) - target_valueA > -0.0001);

    assert(get_target_valueB(e) - target_valueB < 0.0001
           && get_target_valueB(e) - target_valueB > -0.0001);
  }



  //Current target value is initialized to the first of the 2 target values
  {
    double targetA = 0.123456;
    double targetB = 0.654321;
    environment e{targetA,targetB};
    assert(are_equal_with_tolerance(e.get_current_optimum(), targetA));
    assert(are_not_equal_with_tolerance(e.get_current_optimum(), targetB));
  }


  //ISSUE_25
  //An environment can switch target values
  {
    double targetA = 0.123456;
    double targetB = 0.654321;
    environment e{targetA,targetB};
    assert(are_equal_with_tolerance(e.get_current_optimum(), targetA));
    e.switch_target();
    assert(are_equal_with_tolerance(e.get_current_optimum(), targetB));
    e.switch_target();
    assert(are_equal_with_tolerance(e.get_current_optimum(), targetA));
  }


  //#define FIX_ISSUE_35

  {
    double targetA = 123456;
    double targetB = 46589;

    env_param e_p{targetA, targetB};
    environment e{e_p};
    assert(are_equal_with_tolerance(get_target_valueA(e), targetA));
    assert(are_equal_with_tolerance(get_target_valueB(e), targetB));

  }

    ///An environment cna have different types of change:
    /// 1. an environment by default can change between 2 optima
    {
        env_param ep;
        environment<env_change_type::two_optima> e(ep);
        auto first_optimum = e.get_current_optimum();
        e.switch_target();
        auto second_optimum = e.get_current_optimum();
        e.switch_target();
        auto third_optimum = e.get_current_optimum();
        e.switch_target();
        auto fourth_optimum = e.get_current_optimum();

        assert(first_optimum == third_optimum);
        assert(second_optimum == fourth_optimum);

    }

    ///An environment cna have different types of change:
    /// 1. an environment by default can change between 2 optima
}
#endif
