#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


class environment
{
public:
    environment();

    ///Returns the target value of the environment
    double get_current_target_value() const noexcept {return m_current_target_value;}

private:

    ///The target value of the environment
    double m_current_target_value;
};

void test_environment() noexcept;


#endif // ENVIRONMENT_H

