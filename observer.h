#ifndef OBSERVER_H
#define OBSERVER_H
#include "simulation.h"

struct Ind_Nets
{
    int m_generation;
    std::vector<network> m_nets;
};

struct obs_param
{
    obs_param(int n_inds = 1,
    int n_mutations = 1000,
    int n_bins = 100,
    int best_ind_sav_freq = 1,
    int best_ind_spec_sav_freq = 1,
    int all_inds_record_freq = 1):
        m_n_inds{n_inds},
         m_n_mutations{n_mutations},
         m_n_bins{n_bins},
         best_ind_saving_freq{best_ind_sav_freq},
         best_ind_spectrum_saving_freq{best_ind_spec_sav_freq},
         all_inds_rec_freq{all_inds_record_freq}
    {

    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(obs_param,
                                    m_n_inds,
                                    m_n_mutations,
                                    m_n_bins,
                                    best_ind_saving_freq,
                                    best_ind_spectrum_saving_freq,
                                   all_inds_rec_freq)
    int m_n_inds;
    int m_n_mutations;
    int m_n_bins;
    int best_ind_saving_freq;
    int best_ind_spectrum_saving_freq;
    int all_inds_rec_freq;
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

    ///returns the vector where the networks of all individuals of specific generations are stored
    const std::vector<Ind_Nets>& get_all_inds_nets() const noexcept {return m_all_inds_nets;}

    ///Returns const ref to m_env_values
    const std::vector<double>& get_env_values() const noexcept {return m_env_values;}

    ///Saves the avg fitness and current environment value
    void store_avg_fit_and_env(const simulation& s);

    ///Saves the n best individuals in the population where n is a parameter of sim
    void save_best_n_inds(const simulation& s);
    ///Saves the mutational spectrum of the best n individuals in population
    /// where n is a parameter of simulation
    void save_best_n_inds_mut_spectrum(const simulation& s);

    ///Returns the parameters used to run the simualtion
    const all_params& get_params() const noexcept {return m_params;};

    ///Stores the parameters used to run the simulation
    void store_par (const simulation& s) {m_params = s.get_params();}

    ///Stores a vector containing the networks of all individuals
    void store_all_inds_nets(const simulation& s) {m_all_inds_nets.push_back({s.get_time(), extract_all_inds_nets(s)});}


    const int m_best_ind_saving_freq = 1;
    const int m_best_ind_spectrum_saving_freq = 1;
private:

    std::vector<double> m_avg_fitnesses;
    std::vector<double> m_var_fitnesses;
    std::vector<std::vector<ind_data>> m_top_inds;
    std::vector<std::vector<ind_spectrum>> m_top_inds_spectrum;
    std::vector<double> m_env_values;
    all_params m_params = {};
    std::vector<Ind_Nets> m_all_inds_nets;
    int m_n_inds = 0;
    const int m_n_mutations = 0;
    const int m_n_bins = 0;
};

bool operator==(const all_params& lhs, const all_params& rhs);

bool operator!=(const all_params& lhs, const all_params& rhs);


///Executes a simulation for n generations
void exec(simulation& s , observer& o);

///Saves the enitre GODDDAM SIMULATIONNNN!!!!!!! WHOO NEEDS MEMORRYYYY
void save_json(const observer &o, const std::string& filename);

void test_observer();

#endif // OBSERVER_H
