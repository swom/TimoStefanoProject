#include "population.h"

#include <cassert>

population::population(std::vector<int> net_arch,
                       int init_nr_indiv,
                       double mut_rate,
                       double mut_step
                       ):
  m_vec_indiv(static_cast<unsigned int>(init_nr_indiv),individual{net_arch}),
  m_vec_new_indiv(static_cast<unsigned int>(init_nr_indiv)),
  m_mut_rate{mut_rate},
  m_mut_step{mut_step}
{

}

const individual& get_nth_ind(const population& p, size_t ind_index)
{
  return p.get_inds()[ind_index];
}

individual& get_nth_ind(population& p, size_t ind_index)
{
  return p.get_inds()[ind_index];
}

const network& get_nth_ind_net(const population& p, size_t ind_index)
{
  return get_nth_ind(p, ind_index).get_net();
}

network& get_nth_ind_net( population& p, size_t ind_index)
{
  return get_nth_ind(p, ind_index).get_net();
}


void test_population() noexcept
{
  {
    int nelement = 10;
    population pop{{1}, 10};
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
    population p2{{1}, 0, mut_rate};
    assert(are_equal_with_tolerance(p2.get_mut_rate(), mut_rate));

    auto mut_step = 5.0;
    population p3{{1}, 0,0,mut_step};
    assert(are_equal_with_tolerance(p3.get_mut_step(), mut_step));
  }

  ///Population can be initialized with network architecture for inds
  {
    std::vector<int> net_arch{1,33,3,1};
    population p{net_arch, 1, 0, 0};
    assert(get_nth_ind_net(p, 0) == network{net_arch});
  }

  //Population has a buffer_vector for the new_population, with size equal to number of inds
  {
    population p;
    assert(p.get_new_inds().size() == p.get_inds().size());
  }
}
