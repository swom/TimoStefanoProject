#ifndef SIMULATION_H
#define SIMULATION_H

#include "environment.h"
#include "param.h"
#include "population.h"

#include <vector>

class simulation
{
public:
    simulation(param parameters = param());

    ///Returns the environment of the simulation
    const environment& get_env() const noexcept {return m_environment;}
    const population& get_pop() const noexcept {return m_population;}
    const param& get_param() const noexcept {return m_param;}


private:

    param m_param;
    environment m_environment;
    population m_population;


};





void test_simulation() noexcept;

#endif // SIMULATION_H
