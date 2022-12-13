#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <vector>

#include "env_change_type.h"
#include "json.hpp"
#include "utilities.h"

struct env_param
{
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(env_param,
                                   targetA,
                                   targetB)
double targetA;
double targetB;
};

template<env_change_type T = env_change_type::two_optima>
class environment
{
public:

    environment(double target_valueA, double target_valueB):
      m_ref_target_values{target_valueA,target_valueB},
      m_current_target_value {target_valueA}
    {}

    environment(env_param e_p):
      m_ref_target_values{e_p.targetA,e_p.targetB},
      m_current_target_value {e_p.targetA}
    {}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(environment,
                                   m_ref_target_values,
                                   m_current_target_value);
    ///Returns the target value of the environment
    double get_current_optimum() const noexcept {return m_current_target_value;}

    ///Returns the 2 possible target values
    const auto& get_ref_target_values() const noexcept {return m_ref_target_values;}

    ///Sets the target value to an arbitrary number, ised for TESTS
    void for_test_set_arbitrary_target(double new_target) noexcept {m_current_target_value = new_target;}

    ///Changes the current environmental optimum
    void switch_target(){
        //Check which target value is the current one and switch it over to the other

        if (m_current_target_value ==  m_ref_target_values.first)
          {
           m_current_target_value = m_ref_target_values.second;
          }
        else
          {
            m_current_target_value = m_ref_target_values.first;
          }
      }

private:

    ///The target value of the environment
    std::pair<double, double> m_ref_target_values;

    double m_current_target_value;
};

///checks if 2 environments are equal
template<env_change_type T>
bool operator== (const environment<T> &lhs, const environment<T> &rhs)
{
  bool ref_t_values = lhs.get_ref_target_values() == rhs.get_ref_target_values();
  bool current_t_value = are_equal_with_tolerance(lhs.get_current_optimum(), rhs.get_current_optimum());

  return ref_t_values && current_t_value;
}

///get the target value A
template<env_change_type T>
double get_target_valueA(const environment<T>& e)
{
  return e.get_ref_target_values().first;
}

///get the target value B
template<env_change_type T>
double get_target_valueB(const environment<T>& e)
{
  return e.get_ref_target_values().second;
}

void test_environment() noexcept;


#endif // ENVIRONMENT_H

