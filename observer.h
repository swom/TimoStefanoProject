#ifndef OBSERVER_H
#define OBSERVER_H
#include "Stopwatch.hpp"
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

template<class S = simulation<>>
class observer
{
public:
    observer(const obs_param& o_p, const all_params& a_p = {}):
        m_best_ind_saving_freq(o_p.best_ind_saving_freq),
        m_best_ind_spectrum_saving_freq(o_p.best_ind_spectrum_saving_freq),
        m_params{a_p},
        m_o_params{o_p},
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

    ///Checks if it is time to record all the individuals
    bool is_time_to_record_all_inds_nets(const S& s) const noexcept
    {return s.get_time() % m_o_params.all_inds_rec_freq == 0;}

    ///Saves the avg fitness and current environment value
    void store_avg_fit_and_env(const S& s)
    {
        m_avg_fitnesses.push_back(avg_fitness(s));
        m_var_fitnesses.push_back(var_fitness(s));
        m_env_values.push_back(get_current_env_value(s));
    }

    ///Saves the n best individuals in the population where n is a parameter of sim
    void save_best_n_inds(const S &s)
    {
        auto best_inds = get_best_n_inds(s, m_n_inds);
        std::mt19937_64 rng;

        std::vector<ind_data> ind_data_v(best_inds.size());

        for(auto i = best_inds.begin(); i != best_inds.end(); i++)
        {
            ind_data_v[std::distance(best_inds.begin(), i)] = ind_data{*i, s.get_time()};
        }

        m_top_inds.push_back(ind_data_v);
    }

    ///Saves the mutational spectrum of the best n individuals in population
    /// where n is a parameter of simulation
    void save_best_n_inds_mut_spectrum(const S &s)
    {
        auto best_inds = get_best_n_inds(s, m_n_inds);
        std::mt19937_64 rng;

        std::vector<ind_spectrum> ind_spectrum_v(best_inds.size());

        for(auto i = best_inds.begin(); i != best_inds.end(); i++)
        {
            auto index = std::distance(best_inds.begin(), i);

            value_range range = {-1,1};
            ind_spectrum_v[index] = ind_spectrum{*i,
                    response(*i),
                    calculate_mutational_spectrum(*i,
                                                  s.get_params().p_p.mut_step,
                                                  m_n_mutations,
                                                  rng,
                                                  m_n_bins,
                                                  range),
                    s.get_time()
        };

        }

        m_top_inds_spectrum.push_back(ind_spectrum_v);
    }

    ///Returns the parameters used to run the simualtion
    const all_params& get_params() const noexcept {return m_params;};

    ///Stores all data about individuals
    void store_data_about_inds(S &s)
    {
        ///after tick new population is in place,
        /// old populaiton is still stored in new_inds vector
        /// to save data from individuals that just went through the
        /// generation it is necessary to swap back old pop to ind vec
        s.get_pop().get_inds().swap(s.get_pop().get_new_inds());

        if(s.get_time() %m_best_ind_saving_freq  == 0)
        {
            save_best_n_inds(s);
        }
        if(s.get_time() < (400) ||
                s.get_time() > (s.get_n_gen() - 200))
        {
            save_best_n_inds_mut_spectrum(s);
            if(is_time_to_record_all_inds_nets(s))
            {
                store_all_inds_nets(s);
                //store all phenotypes
            }
        }

        ///Swap back
        s.get_pop().get_inds().swap(s.get_pop().get_new_inds());
    }

    ///Stores the parameters used to run the simulation
    void store_par (const S& s) {m_params = s.get_params();}

    ///Stores a vector containing the networks of all individuals
    void store_all_inds_nets(const S& s)
    {
        m_all_inds_nets.push_back({s.get_time(), extract_all_inds_nets(s)});
    }

    const int m_best_ind_saving_freq = 1;
    const int m_best_ind_spectrum_saving_freq = 1;

private:

    std::vector<double> m_avg_fitnesses;
    std::vector<double> m_var_fitnesses;
    std::vector<std::vector<ind_data>> m_top_inds;
    std::vector<std::vector<ind_spectrum>> m_top_inds_spectrum;
    std::vector<double> m_env_values;
    all_params m_params = {};
    obs_param m_o_params;
    std::vector<Ind_Nets> m_all_inds_nets;
    int m_n_inds = 0;
    const int m_n_mutations = 0;
    const int m_n_bins = 0;
};

bool operator==(const all_params& lhs, const all_params& rhs);

bool operator!=(const all_params& lhs, const all_params& rhs);


///Executes a simulation for n generations
template<class S, class O>
void exec(S &s , O &o)
{
    stopwatch::Stopwatch sw;
    o.store_par(s);

    while(s.get_time() < s.get_n_gen())
    {
        tick(s);
        o.store_avg_fit_and_env(s);
        o.store_data_about_inds(s);
        if(s.get_time() % 1000 == 0)
        {
            std::cout << "Cycle " << s.get_time() << ". Elapsed: " << sw.lap<stopwatch::s>() << " seconds." << std::endl;
        }
    }
}

///Saves the enitre GODDDAM SIMULATIONNNN!!!!!!! WHOO NEEDS MEMORRYYYY
template<class O>
void save_json(const O &o, const std::string& filename)
{
    std::ofstream  f(filename);
    nlohmann::json json_out;
    json_out = o;
    f << json_out;
}

void test_observer();

#endif // OBSERVER_H
