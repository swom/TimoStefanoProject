#ifndef PARAM_H
#define PARAM_H


class param
{
public:
    param(int init_pop_size = 1, double target_value = 0);

    ///Retruens the number of individuals at the start of the simulation
    int get_init_pop_size() const noexcept {return m_init_pop_size;}

    ///Returns the environment target_value
    double get_target_value() const noexcept {return m_target_value;}

private:

    ///The number of individuals at the start of the simulation
    int m_init_pop_size;

    ///The target value that is gonna give the best fitness
    ///to individuals that match it
    double m_target_value;

};

void test_param() noexcept;

#endif // PARAM_H
