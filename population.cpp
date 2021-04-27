#include "population.h"

#include <cassert>

population::population(int init_nr_indiv,
                       double mut_rate,
                       double mut_step,
                       std::vector<int> net_arch
                       ):
  m_vec_indiv(static_cast<unsigned int>(init_nr_indiv),individual{net_arch}),
  m_vec_new_indiv(static_cast<unsigned int>(init_nr_indiv)),
  m_mut_rate{mut_rate},
  m_mut_step{mut_step}
{

}

std::vector<double> adjust_distances(std::vector<double> distances)
{
  for(double& dist : distances)
    {
      dist += 0.0000000000000001;
    }
  return distances;
}

bool all_nets_equals_to(const population& p, const network& n)
{
  return std::all_of(p.get_inds().begin(), p.get_inds().end(),
                     [n](const individual& i)
  {return i.get_net() == n;});
}

std::vector<double> calc_dist_from_target(const std::vector<individual>& inds, double env_value)
{
  std::vector<double> distance_from_target;

  for(auto& ind : inds)
    {
      distance_from_target.push_back(calc_sqr_distance(ind, env_value));
    }

  return distance_from_target;
}

population calc_fitness(population p, const double& env_value,const double &sel_str)
{
  std::vector<double> distance_from_target = calc_dist_from_target(p.get_inds(), env_value);

  double min_distance = *std::min_element(distance_from_target.begin(),distance_from_target.end());

  auto fitness_vector = rescale_dist_to_fit(distance_from_target,
                                            min_distance,
                                            sel_str);

  set_fitness_inds(p, fitness_vector);

  return p;
}

rndutils::mutable_discrete_distribution<>  create_mut_dist_fit(population& p)
{
  rndutils::mutable_discrete_distribution<> mut_dist;
  mut_dist.mutate_transform(p.get_inds().begin(),
                            p.get_inds().end(),
                            [](const individual& i) {return i.get_fitness();});
  return  mut_dist;
}

std::vector<double> create_rescaled_fitness_vec(std::vector<double> distance_from_target,
                                                double min_distance,
                                                double selection_strength)
{
  std::vector<double> fitness_inds;
  for(size_t i = 0; i != distance_from_target.size(); i++)
    {

      auto scaled_dist = min_distance / distance_from_target[i];

      auto ind_fit = pow(1 - (1 -  scaled_dist), selection_strength);

      fitness_inds.push_back(ind_fit);
    }
  return fitness_inds;
}

void change_nth_ind_net(population& p, size_t ind_index, network n)
{
  get_nth_ind_net(p, ind_index) = n;
}

void check_and_correct_dist(std::vector<double>& distance_from_target, double& min_distance)
{
  if(min_distance == 0)
    {
      distance_from_target =  adjust_distances(distance_from_target);

      min_distance = *std::min_element(distance_from_target.begin(),
                                       distance_from_target.end());
    }
}

void select_new_pop(population& p,
                    const rndutils::mutable_discrete_distribution<>& mut_dist,
                    std::minstd_rand& rng)
{
  for( size_t i = 0; i != p.get_inds().size(); i++)
    {
      p.get_new_inds()[i] = mutate(p.get_inds()[mut_dist(rng)],
          p.get_mut_rate(),
          p.get_mut_step(),
          rng);
    }
}

void swap_new_with_old_pop(population& p)
{
  p.get_inds().swap(p.get_new_inds());
}

const individual& get_nth_ind(const population& p, size_t ind_index)
{
  return p.get_inds()[ind_index];
}

individual& get_nth_ind(population& p, size_t ind_index)
{
  return p.get_inds()[ind_index];
}

double get_nth_ind_fitness(const population& p, const size_t& ind_index)
{
  return p.get_inds()[ind_index].get_fitness();
}

const network& get_nth_ind_net(const population& p, size_t ind_index)
{
  return get_nth_ind(p, ind_index).get_net();
}

network& get_nth_ind_net( population& p, size_t ind_index)
{
  return get_nth_ind(p, ind_index).get_net();
}

std::vector<double> rescale_dist_to_fit(std::vector<double> distance_from_target,
                                        double min_distance,
                                        double selection_strength)
{
  check_and_correct_dist(distance_from_target, min_distance);

  auto fitness_inds = create_rescaled_fitness_vec(distance_from_target, min_distance, selection_strength);

  return fitness_inds;
}

void reproduce(population& p, std::minstd_rand& rng)
{
  auto mut_dist = create_mut_dist_fit(p);

  select_new_pop(p, mut_dist, rng);

  swap_new_with_old_pop(p);
}

void set_fitness_inds(population& p, const std::vector<double>& fitness_vector)
{
  assert(p.get_inds().size() == fitness_vector.size());

  for(size_t i = 0; i != fitness_vector.size(); i++)
    {
      set_nth_ind_fitness(p, i, fitness_vector[i]);
    }
}

void set_nth_ind_fitness (population& p, size_t ind_index, double fitness)
{
  auto& ind = p.get_inds()[ind_index];
  ind.set_fitness(fitness);
}

void test_population() noexcept
{
  {
    int nelement = 10;
    population pop{10};
    assert (static_cast<int>(pop.get_inds().size()) == nelement );
  }
  //A population has a member variable called m_mut_step
  //And m_mut_rate
  //By default initialized to 0.01 (mut_rate)
  // and 0.1 (mut_step)
  {
    population p;
    assert(are_equal_with_tolerance(p.get_mut_rate(), 0.01));
    assert(are_equal_with_tolerance(p.get_mut_step(), 0.1));

    auto mut_rate = 5.0;
    population p2{0, mut_rate};
    assert(are_equal_with_tolerance(p2.get_mut_rate(), mut_rate));

    auto mut_step = 5.0;
    population p3{0 ,0, mut_step};
    assert(are_equal_with_tolerance(p3.get_mut_step(), mut_step));
  }

  ///Population can be initialized with network architecture for inds
  {
    std::vector<int> net_arch{1,33,3,1};
    population p{1, 0, 0, net_arch};
    assert(get_nth_ind_net(p, 0) == network{net_arch});
  }

  //Population has a buffer_vector for the new_population, with size equal to number of inds
  {
    population p;
    assert(p.get_new_inds().size() == p.get_inds().size());
  }

#define FIX_ISSUE_32
#ifdef FIX_ISSUE_32
  ///Individuals with higher fitness are preferentially selected for the next generation
  {
    int n_inds = 2;
    size_t first_ind = 0;
    size_t second_ind = 1;
    population p{n_inds};
    std::minstd_rand rng;

    //make first ind net recognizable
    auto new_net =  change_all_weights(get_nth_ind_net(p,first_ind), 123456);
    change_nth_ind_net(p, first_ind, new_net);

    set_nth_ind_fitness(p, first_ind, 1);
    set_nth_ind_fitness(p, second_ind, 0);

    reproduce(p, rng);

    assert(all_nets_equals_to(p, new_net));
  }
#endif

//#define FIX_ISSUE_37
#ifdef FIX_ISSUE_37
  {
    std::vector<int> net_arc{1,23,456,789};
    int age = 123456789;

    ind_param i_p{net_arc, age};

    int number_of_inds = 13245679;
    double mut_rate = 0.314;
    double mut_step = 0.1414;

    pop_param p_p{number_of_inds, mut_rate, mut_step};

    population p{p_p, i_p};

    for(const auto& ind : p.get_inds())
      {
        assert(ind.get_net() == network{net_arc});
      }

    assert(are_equal_with_tolerance(p.get_inds().size(), number_of_inds) &
           are_equal_with_tolerance(p.get_mut_rate(), mut_rate) &
           are_equal_with_tolerance(p.get_mut_step(), mut_step));
  }
#endif
}
