#ifndef PARAM_H
#define PARAM_H


class param
{
public:
    param(int init_pop_size = 1);

    ///Retruens the number of individuals at the start of the simulation
    int get_init_pop_size() const noexcept {return m_init_pop_size;}
private:
    ///The number of individuals at the start of the simulation
    int m_init_pop_size;
};

void test_param() noexcept;

#endif // PARAM_H
