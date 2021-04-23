#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>

class environment
{
public:
    environment(double target_valueA, double target_valueB);

    ///Returns the target value of the environment
    double get_current_target_value() const noexcept {return m_current_target_value;}
    const std::vector<double>& get_ref_target_values() const noexcept {return m_ref_target_values;}
    double get_target_valueA() const noexcept {return m_target_valueA;}
    double get_target_valueB() const noexcept {return m_target_valueB;}

    ///Returns the target value of the environment
    double& get_current_target_value() noexcept {return m_current_target_value;}

private:

    ///The target value of the environment
    double m_current_target_value;
    double m_target_valueA;
    double m_target_valueB;
    std::vector<double> m_ref_target_values;
};

void test_environment() noexcept;


#endif // ENVIRONMENT_H

