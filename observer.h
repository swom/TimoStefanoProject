#ifndef OBSERVER_H
#define OBSERVER_H
#include "simulation.h"

struct obs_param
{
    int m_n_inds;
    int m_n_mutations;
    int m_n_bins;
    int best_ind_saving_freq;
    int best_ind_spectrum_saving_freq;
};

class observer
{
public:
    observer();
    observer(const obs_param& o_p):
        m_best_ind_saving_freq(o_p.best_ind_saving_freq),
        m_best_ind_spectrum_saving_freq(o_p.best_ind_spectrum_saving_freq),
        m_n_inds{o_p.m_n_inds},
        m_n_mutations(o_p.m_n_mutations),
        m_n_bins(o_p.m_n_bins)
    {}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(observer,
                                   m_avg_fitnesses,
                                   m_var_fitnesses,
                                   m_top_inds,
                                   m_top_inds_spectrum,
                                   m_env_values,
                                   m_params)

    ///Returns const ref to m_env_values
    const std::vector<double>& get_env_values() const noexcept {return m_env_values;}

    ///Saves the avg fitness and current environment value
    void store_avg_fit_and_env(const simulation& s);

    ///Saves the n best individuals in the population where n is a parameter of sim
    void save_best_n_inds(const simulation& s);
    ///Saves the mutational spectrum of the best n individuals in population
    /// where n is a parameter of simulation
    void save_best_n_inds_mut_spectrum(const simulation& s);

    const all_params& get_params() const noexcept {return m_params;};

    void store_par (const simulation& s) {m_params = s.get_params();}

    const int m_best_ind_saving_freq;
    const int m_best_ind_spectrum_saving_freq;
private:

    std::vector<double> m_avg_fitnesses;
    std::vector<double> m_var_fitnesses;
    std::vector<std::vector<ind_data>> m_top_inds;
    std::vector<std::vector<ind_spectrum>> m_top_inds_spectrum;
    std::vector<double> m_env_values;
    all_params m_params = {};
    int m_n_inds = 0;
    const int m_n_mutations;
    const int m_n_bins;
};

bool operator==(const all_params& lhs, const all_params& rhs);

bool operator!=(const all_params& lhs, const all_params& rhs);


///Executes a simulation for n generations
void exec(simulation& s , observer& o);

///Saves the enitre GODDDAM SIMULATIONNNN!!!!!!! WHOO NEEDS MEMORRYYYY
void save_json(const observer &o, const std::string& filename);

void test_observer();

#endif // OBSERVER_H
