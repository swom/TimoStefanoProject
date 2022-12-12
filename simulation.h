#ifndef SIMULATION_H
#define SIMULATION_H
#include <fstream>
#include <vector>

#include "environment.h"
#include "population.h"

struct sim_param
{
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(sim_param,
                                   seed,
                                   change_freq,
                                   selection_strength,
                                   n_generations)
 int seed;
 double change_freq;
 double selection_strength;
 int n_generations;

};

struct all_params
{
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(all_params,
                                   e_p,
                                   i_p,
                                   p_p,
                                   s_p)
 env_param e_p;
 ind_param i_p;
 pop_param p_p;
 sim_param s_p;

};

template<class Env = environment<env_change_type::two_optima>>
class simulation
{
public:

  simulation(double targetA = 0,
             double targetB = 0.1,
             int init_pop_size = 1,
             int seed = 0,
             double t_change_interval = 0.1,
             std::vector<int> net_arch = {1,2,1},
             double sel_str = 2,
             int number_of_generations = 1000
          ):
      m_environment{targetA, targetB},
      m_population{init_pop_size},
      m_n_generations{number_of_generations},
      m_seed{seed},
      m_t_change_env_distr{static_cast<double>(t_change_interval)},
      m_sel_str{sel_str},
      m_change_freq {static_cast<double>(t_change_interval)}
  {
      m_mut_rng.seed(m_seed);
      for(auto& ind : m_population.get_inds())
      {
          ind.get_net() = net_arch;
      }
  }

  simulation(const all_params& params):
      m_environment{params.e_p},
      m_population{params.p_p, params.i_p},
      m_n_generations{params.s_p.n_generations},
      m_seed{params.s_p.seed},
      m_t_change_env_distr{static_cast<double>(params.s_p.change_freq)},
      m_sel_str{params.s_p.selection_strength},
      m_change_freq {static_cast<double>(params.s_p.change_freq)},
      m_params{params}
  {
      m_mut_rng.seed(m_seed);
      m_env_rng.seed(0);
  }

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(simulation,
                                 m_environment,
                                 m_population,
                                 m_time,
                                 m_change_freq,
                                 m_sel_str,
                                 m_seed)

  ///Returns const ref ot population memeber
  const population& get_pop() const noexcept {return m_population;}

  ///Returns const ref ot population memeber
  population& get_pop() noexcept {return m_population;}

  ///Returns ref to rng used for mutations
  std::mt19937_64& get_mut_rng() noexcept {return m_mut_rng;}

  ///Returns ref to rng used for mutations
  std::mt19937_64& get_env_rng() noexcept {return m_env_rng;}

  ///Returns const ref to env_member
  const Env& get_env() const noexcept {return m_environment;}

  ///Returns const ref to env_member
  Env& get_env() noexcept {return m_environment;}

  ///Returns the number of generatiosn for which the simualtion has to run
  const int& get_n_gen() const noexcept {return m_n_generations;}

  ///returns const ref to
  const std::bernoulli_distribution& get_t_change_env_distr() const noexcept {return m_t_change_env_distr;}

  const int& get_time() const noexcept {return m_time;}
  void increase_time() {++m_time;}

  ///Returns the strength of selection
  double get_sel_str() const noexcept {return m_sel_str;}

  ///Returns change frequency
  double get_change_freq() const noexcept {return m_change_freq;}

  ///Returns seed
  int get_seed() const noexcept {return m_seed;}

  const all_params& get_params() const noexcept {return m_params;}

  private:

  Env m_environment;

   population m_population;

   int m_n_generations;

   std::mt19937_64 m_mut_rng;

   std::mt19937_64 m_env_rng;

   int m_seed;

   std::bernoulli_distribution m_t_change_env_distr;

   int m_time = 0;

   double m_sel_str;

   double m_change_freq;

   all_params m_params;

};
///Checks if 2 simulations are equal
template<class E>
bool operator ==(const simulation<E> &lhs, const simulation<E> &rhs)
{
    bool pop = lhs.get_pop() == rhs.get_pop();
    bool env = lhs.get_env() == rhs.get_env();
    bool time = lhs.get_time() == rhs.get_time();
    bool sel_str = are_equal_with_tolerance(lhs.get_sel_str(), rhs.get_sel_str());
    bool change_freq = are_equal_with_tolerance(lhs.get_change_freq(), rhs.get_change_freq());

    return pop && env && time && sel_str && change_freq;
}

///Calculates the avg_fitness of the population
template <class E>
double avg_fitness(const simulation<E> &s)
{
    return avg_fitness(s.get_pop());
}

///Calculates fitness of inds in pop given current env values
template <class E>
void calc_fitness(simulation<E> &s)
{
    s.get_pop() = calc_fitness(s.get_pop(), get_current_env_value(s), s.get_sel_str());
}


///Changes all the weights of a given individual to a given value
template <class E>
void change_all_weights_nth_ind(simulation<E> &s, size_t ind_index, double new_weight)
{
    auto new_net = change_all_weights(get_nth_ind_net(s, ind_index), new_weight);
    change_nth_ind_net(s, ind_index, new_net);
}

///Changes the curent environmental optimum
template<class E>
void change_current_target_value(simulation<E>& s, double new_target_value)
{
    s.get_env().set_current_target_value(new_target_value);
}

///Gets the minimum fitness among the inds present in the simulation
template<class E>
double find_min_fitness(const simulation<E>&s)
{
    auto inds = s.get_pop().get_inds();

    auto min_ind =
            std::min_element(inds.begin(), inds.end(), [](const individual& lhs, const individual& rhs){
        return lhs.get_fitness() < rhs.get_fitness();});


    return min_ind->get_fitness();
}

///Gets the nth individual in the population vector
template<class E>
const individual& get_nth_ind(const simulation<E>& s, size_t ind_index)
{
    return get_nth_ind(s.get_pop(), ind_index);
}

///Changes the network of the nth individual for a given network
template <class E>
void change_nth_ind_net(simulation<E> &s, size_t ind_index, const network& n)
{
    change_nth_ind_net(s.get_pop(), ind_index, n) ;
}

///Returns a vector of networks of all the individuals in the population
template <class E>
const std::vector<network> extract_all_inds_nets(const simulation<E> &s)
{
    std::vector<network> nets(s.get_pop().get_inds().size(), s.get_pop().get_inds().at(0).get_net());
    std::transform( s.get_pop().get_inds().begin(),
                    s.get_pop().get_inds().end(),
                    nets.begin(),[](auto& ind){return ind.get_net();});
    return nets;
}

///Gets the best n individuals in a pop
template <class E>
std::vector<individual> get_best_n_inds(const simulation<E> &s, int n)
{
    return get_best_n_inds(s.get_pop(), n);
}

///Returns the current target value of the environment in the simualtion
template <class E>
double get_current_env_value(const simulation<E> &s)
{
    return s.get_env().get_current_target_value();
}

///Returns the current target value of the environment in the simualtion
template <class E>
double get_current_env_value(simulation<E> &s)
{
    return s.get_env().get_current_target_value();
}

///Returns the individuals in the simualtion
template <class E>
const std::vector<individual>& get_inds(const simulation<E> &s)
{
    return s.get_pop().get_inds();
}

///Returns the fitness of the nth ind in pop
template <class E>
double get_nth_ind_fitness(const simulation<E> &s, const size_t ind_index)
{
    return get_nth_ind_fitness(s.get_pop(), ind_index);
}

///Returns const or non-onst ref to the network of the nth individual in the
/// popoulation member of a simulation
template <class E>
const network& get_nth_ind_net(const simulation<E> &s, size_t ind_index)
{
    return get_nth_ind_net(s.get_pop(), ind_index);
}

///Gets the network of the nth individual in the population vector
template <class E>
network& get_nth_ind_net(simulation<E> &s, size_t ind_index)
{
    return get_nth_ind_net(s.get_pop(), ind_index);
}

///Checks if the environment is about to change
template<class E>
bool is_environment_changing (simulation<E>&s)
{
    std::bernoulli_distribution distro = s.get_t_change_env_distr();
    return distro (s.get_env_rng());
}

///Tries to load a simulation gien the name of a .json file
template <class E>
simulation<E> load_json(
        const std::string& filename
        )
{
    std::ifstream f(filename);
    nlohmann::json json_in;
    simulation s;
    f >> json_in;
    return s = json_in;
}


///Reproduction: individuals are copied wiht mutation to the next
/// generation based on their perfromance ->
/// i.e. how close they  match the current optimum
template<class E>
void reproduce(simulation<E>& s)
{
    reproduce(s.get_pop(), s.get_mut_rng());
}

///Calculates fitness and selects a new population based on fitness
template <class E>
void select_inds(simulation<E> &s)
{
    calc_fitness(s);
    reproduce(s);
}

///Ticks time one generation into the future
template <class E>
void tick(simulation<E> &s)
{
    s.increase_time();

    if(is_environment_changing(s)){

        switch_target(s.get_env());
    }

    select_inds(s);

}

///Saves the enitre GODDDAM SIMULATIONNNN!!!!!!! WHOO NEEDS MEMORRYYYY
template <class E>
void save_json(const simulation<E> &s, const std::string& filename)
{
    std::ofstream  f(filename);
    nlohmann::json json_out;
    json_out = s;
    f << json_out;
}

///Calculates the standard devaition of the population fitness
template <class E>
double var_fitness(const simulation<E> &s)
{
    return var_fitness(s.get_pop());
}

void test_simulation() noexcept;

#endif // SIMULATION_H
