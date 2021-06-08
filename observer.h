#ifndef OBSERVER_H
#define OBSERVER_H
#include "simulation.h"

class observer
{
public:
    observer();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(observer,
                                   m_avg_fitnesses,
                                   m_var_fitnesses,
                                   m_top_100_inds,
                                   m_env_values)

    ///Saves the avg fitness and current environment value
    void store_avg_fit_and_env(const simulation& s);

    ///Saves the 100 best individuals in the population
    void save_best_100_inds(const simulation& s);

private:

    std::vector<double> m_avg_fitnesses;
    std::vector<double> m_var_fitnesses;
    std::vector<std::vector<individual>> m_top_100_inds;
    std::vector<double> m_env_values;
};

///Executes a simulation for n generations
void exec(simulation& s , observer& o, int n_generations);

///Saves the enitre GODDDAM SIMULATIONNNN!!!!!!! WHOO NEEDS MEMORRYYYY
void save_json(const observer &o, const std::string& filename);

#endif // OBSERVER_H
