#include "simulation.h"

#include <cassert>
#include <vector>
#include <fstream>


simulation::simulation(double targetA, double targetB,
                       int init_pop_size,
                       int seed,
                       int t_change_interval,
                       std::vector<int> net_arch,
                       double sel_str):
  m_environment{targetA, targetB},
  m_population{init_pop_size},
  m_rng{seed},
  m_t_change_env_distr{1.0/static_cast<double>(t_change_interval)},
  m_sel_str{sel_str},
  m_change_freq {1.0/static_cast<double>(t_change_interval)}
{
  for(auto& ind : m_population.get_inds())
    {
      ind.get_net() = net_arch;
    }
}

bool operator ==(const simulation& lhs, const simulation& rhs)
{
  bool pop = lhs.get_pop() == rhs.get_pop();
  bool env = lhs.get_env() == rhs.get_env();
  bool time = lhs.get_time() == rhs.get_time();
  bool sel_str = are_equal_with_tolerance(lhs.get_sel_str(), rhs.get_sel_str());
  bool change_freq = are_equal_with_tolerance(lhs.get_change_freq(), rhs.get_change_freq());

  return pop && env && time && sel_str && change_freq;
}

void calc_fitness(simulation& s)
{
  s.get_pop() = calc_fitness(s.get_pop(), get_current_env_value(s), s.get_sel_str());
}


void change_all_weights_nth_ind(simulation& s, size_t ind_index, double new_weight)
{
  auto new_net = change_all_weights(get_nth_ind_net(s, ind_index), new_weight);
  change_nth_ind_net(s, ind_index, new_net);
}

void change_current_target_value(simulation& s, double new_target_value)
{
  s.get_env().set_current_target_value(new_target_value);
}

void change_nth_ind_net(simulation& s, size_t ind_index, const network& n)
{
  change_nth_ind_net(s.get_pop(), ind_index, n) ;
}

double get_current_env_value(const simulation&s)
{
  return s.get_env().get_current_target_value();
}

double get_current_env_value(simulation&s)
{
  return s.get_env().get_current_target_value();
}

const std::vector<individual>& get_inds(const simulation&s)
{
  return s.get_pop().get_inds();
}

const individual& get_nth_ind(const simulation& s, size_t ind_index)
{
  return get_nth_ind(s.get_pop(), ind_index);
}

double get_nth_ind_fitness(const simulation& s, const size_t ind_index)
{
  return get_nth_ind_fitness(s.get_pop(), ind_index);
}

const network& get_nth_ind_net(const simulation& s, size_t ind_index)
{
  return get_nth_ind_net(s.get_pop(), ind_index);
}

network& get_nth_ind_net( simulation& s, size_t ind_index)
{
  return get_nth_ind_net(s.get_pop(), ind_index);
}

double find_min_fitness(const simulation&s)
{
  auto inds = s.get_pop().get_inds();

  auto min_ind =
      std::min_element(inds.begin(), inds.end(), [](const individual& lhs, const individual& rhs){
      return lhs.get_fitness() < rhs.get_fitness();});


  return min_ind->get_fitness();
}

bool is_environment_changing (simulation &s) {

std::bernoulli_distribution distro = s.get_t_change_env_distr();
return distro (s.get_rng());

}

simulation load_json(
        const std::string& filename
        )
{
    std::ifstream f(filename);
    nlohmann::json json_in;
    simulation s;
    f >> json_in;
    return s = json_in;
}

void reproduce(simulation& s)
{
  reproduce(s.get_pop(), s.get_rng());

}

void tick(simulation &s)
{
    s.increase_time();


   if(is_environment_changing(s)){

    switch_target(s.get_env());
   }

   select_inds(s);

}

void save_json(const simulation& s, const std::string& filename)
{
  std::ofstream  f(filename);
  nlohmann::json json_out;
  json_out = s;
  f << json_out;
}

void select_inds(simulation& s)
{
  calc_fitness(s);
  reproduce(s);
}

void test_simulation() noexcept//!OCLINT test may be many
{

  ///A simulation has a member of type population
  ///The population has a vector of individuals of size 1 by default
  {
    simulation s;
    assert(s.get_pop().get_inds().size() == 1u);
  }


#ifdef FIX_ISSUE_27
  ///A simulation can be initialized by
  /// target values for the environment
  /// and the number of individuals in
  /// the populaation
  {
    double target_valueA = 1.23456;
    double target_valueB = 6.12345;
    int init_pop_size = 123456;
    simulation s{target_valueA, target_valueB ,init_pop_size};
    assert(s.get_env().get_target_valueA() == target_valueA
           && s.get_env().get_target_valueB() == target_valueB
           && s.get_pop().get_ind_vec().size() == static_cast<unsigned int>(init_pop_size));
  }
#endif
  ////A simulation should have a random engine, intialized with a seed that is fed to simulation

  {
    double targetA = 1.0;
    double targetB = 0;
    int pop_size = 1;
    int seed = 123456789;
    simulation s{targetA,
          targetB,
          pop_size,
          seed};
    std::minstd_rand copy_rng(seed);
    assert ( s.get_rng()() == copy_rng());

  }

  ///A simulation should have a t_change_env_distr bernoulli distribution
  /// that determines the interval of environmental change
  /// Default initialization value is 10
  {
    double targetA = 1.0;
    double targetB = 0;
    int pop_size = 1;
    int seed = 123456789;
    int t_change_interval = 20;

    simulation s { targetA, targetB,
          pop_size, seed, t_change_interval};
    std::bernoulli_distribution mockdistrotchange(1.0 / static_cast<double>(t_change_interval));
    assert (s.get_t_change_env_distr() == mockdistrotchange);

  }



  //A simulation has an absolute counter and you can access it
  {
    simulation s;
    assert((s.get_time() > 0) | (s.get_time() <= 0));
  }



  //Every tick simulation timer increases by one
  {
    simulation s;
    auto init_timer_value = s.get_time();
    tick(s);
    assert(s.get_time() == init_timer_value + 1);

    simulation s2;
    init_timer_value = s2.get_time();
    int repeats = 123;
    for(int i = 0; i != repeats; i++)
      {
        tick(s2);
      }
    assert(s2.get_time() == init_timer_value + repeats);
  }


  ///Simulation can be initialized with network architecture for inds in pop
  {
    std::vector<int> net_arch{1,33,2,1};
    simulation s{0,0,1,0,0, net_arch};

    assert(get_nth_ind_net(s, 0) == network{net_arch});
  }

#define FIX_ISSUE_30
#ifdef FIX_ISSUE_30
  ///individuals in a pop are selected based on how closely they match the current_env_target_value
  {
    int pop_size = 2;
    simulation s{0,0, pop_size};

    //change ind 0 net
    size_t changed_ind = 0;
    auto new_net = change_all_weights(get_nth_ind_net(s,changed_ind), 1);
    change_nth_ind_net(s, changed_ind, new_net);

    //change target value to match output of ind 0 net
    change_current_target_value(s, response(get_nth_ind(s, 0))[0]);

    assert(response(get_nth_ind(s, 0))[0] == get_current_env_value(s));
    assert(response(get_nth_ind(s, 1))[0] != get_current_env_value(s));

    select_inds(s);

    //all inds should now have the network that matches the target values
    for(const auto& ind :get_inds(s))
      {
        assert(ind.get_net() == new_net);
      }
  }
#endif

  ///Fitness of individuals is calculated based on how close they are to the current target value
  {
    int pop_size = 2;
    simulation s{0,0,pop_size};

    size_t first_ind = 0;
    size_t second_ind = 1;
    change_all_weights_nth_ind(s, first_ind, 1);
    change_all_weights_nth_ind(s, second_ind, 0.99);


    //change target value to match output of ind 0 net
    change_current_target_value(s, response(get_nth_ind(s, 0))[0]);

    calc_fitness(s);

    ///ind 0 response should match exactly the target value therefore it will have fitness 1 (max)
    auto first_ind_fit =  get_nth_ind_fitness(s,0) ;
    assert(are_equal_with_tolerance( first_ind_fit, 1));

    ///ind 1 response is 0, therefore its fitness would be the lowest in all the population
    auto first_response = response(get_nth_ind(s, 0))[0];
    auto second_response = response(get_nth_ind(s, 1))[0];
    assert(!are_equal_with_tolerance(first_response, second_response));

    auto second_ind_fit =  get_nth_ind_fitness(s,1) ;
    auto min_fit = find_min_fitness(s);

    assert(are_equal_with_tolerance(min_fit,second_ind_fit));

  }

  //#define FIX_ISSUE_34
  {
    simulation s;
    int repeats = 10000;
    int n_switches = 0;
    for(int i = 0; i != repeats; i++)
      {
        if(is_environment_changing(s))
          {
            n_switches++;
          }
      }
    assert(n_switches - s.get_change_freq() * repeats < 10 &&
           n_switches - s.get_change_freq() * repeats > -10);
  }


  //#define FIX_ISSUE_38
#ifdef FIX_ISSUE_38
  {

    //env_par
    double targetA = 123456;
    double targetB = 46589;

    env_param e_p{targetA, targetB};
    environment e{e_p};

    //ind_par
    std::vector<int> net_arc{1,23,456,789};
    int age = 123456789;

    ind_param i_p{net_arc, age};

    //pop_par
    int number_of_inds = 13245679;
    double mut_rate = 0.314;
    double mut_step = 0.1414;

    pop_param p_p{number_of_inds, mut_rate, mut_step};

    //sim_par
    int seed = 10126789;
    double change_freq = 123789;
    double selection_strength = 0.321546;

    sim_par s_p{seed, change_freq, selection_strength};
    simulation s{e_p, i_p, p_p, s_p};

    //test env
    assert(are_equal_with_tolerance(e.get_target_valueA(), targetA));
    assert(are_equal_with_tolerance(e.get_target_valueB(), targetB));

    //test pop and ind
    for(const auto& ind : get_inds(s))
      {
        assert(ind.get_net() == network{net_arc});
      }

    assert(are_equal_with_tolerance(p.get_inds().size(), number_of_inds) &
           are_equal_with_tolerance(p.get_mut_rate(), mut_rate) &
           are_equal_with_tolerance(p.get_mut_step(), mut_step));

    //test sim
    assert(are_equal_with_tolerance(s.get_change_freq(), change_freq) &
           are_equal_with_tolerance(s.get_sel_str(), selection_strength) &
           s.get_seed() == seed);
  }
#endif

    //#define FIX_ISSUE_39
  {
    simulation s;
    int repeats =  10000;
    auto previous_env_value = get_current_env_value(s);

    int number_of_env_change = 0;

    for( int i = 0; i != repeats; i++)
      {
        tick(s);
        if(previous_env_value != get_current_env_value(s))
          {
            previous_env_value = get_current_env_value(s);
            number_of_env_change++;
          }
      }
    assert( number_of_env_change - repeats * s.get_change_freq() < 10 &&
            number_of_env_change - repeats * s.get_change_freq() > -10);

  }


#define FIX_ISSUE_40
#ifdef FIX_ISSUE_40
  {
    //create a non-default simulaiton
    simulation s{13, 32, 2, 132, 548, {1,2,3,4,5,6}, 3.14};
    auto name = "sim_save_test";
    save_json(s, name);
    auto loaded_s = load_json(name);
    assert(s == loaded_s);
  }
#endif

}
