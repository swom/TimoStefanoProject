#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


class environment
{
public:
    environment(double init_target_value = 0);

    ///Returns the target value of the environment
    double get_target_value() const noexcept {return m_target_value;}

private:

    ///The target value of the environment
    double m_target_value;
};

<<<<<<< HEAD
void test_environment() noexcept;
=======
void   test_environment() noexcept;
>>>>>>> stefano

#endif // ENVIRONMENT_H
