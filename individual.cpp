#include "individual.h"

#include <algorithm>
#include <cassert>

individual::individual(std::vector<int> net_arch, int age) :
  m_age{age},
  ///!!!!Attention!!!! input values are for now a fixed amount
  m_input_values(net_arch[0], 1.0),
  m_network{net_arch}
{

}

bool operator== (const individual& lhs, const individual& rhs)
{
  bool fitness = are_equal_with_tolerance(lhs.get_fitness(), rhs.get_fitness());
  bool network = lhs.get_net() == rhs.get_net();
  bool inputs = lhs.get_input_values() == rhs.get_input_values();

  return fitness && network && inputs;
}

double calc_sqr_distance(const individual& i, double env_value)
{
   return (response(i)[0] - env_value) * (response(i)[0] - env_value);
}

individual mutate(individual i, double mut_rate, double mut_step, std::minstd_rand& rng)
{
  i.get_net() = mutate(i.get_net(), mut_rate, mut_step, rng);
  return i;
}

std::vector<double> response(const individual& ind)
{
  return response(ind.get_net(),ind.get_input_values());
}

void test_individual()
{
  //An individual has a member variable called m_age
  //By default initialized to 0
  {
    individual i;
    assert(i.get_age() == 0);

    int age = 5;
    individual i2{{1},age};
    assert(i2.get_age() == age);
  }

  ///An indiivdual is initialized with a network architecture,
  /// by default 1,2,1
  {
    std::vector<int> net_arch{1,2,1};
    individual i{net_arch};
    assert(i.get_net() == network{net_arch});
  }

  ///Individuals have a vector of fixed input values, always equal to 1, for their network
  {
    int n_input = 456;
    std::vector<int> net_arch{n_input};
    individual i{net_arch};
    assert(i.get_input_values().size() == static_cast<size_t>(n_input));
    for(const auto& value : i.get_input_values())
      {
        assert(are_equal_with_tolerance(value, 1.0));
      }
  }

  ///When an individual responds to environment it uses its input values as input
  {
    individual i;
    assert( response(i) == response(i.get_net(),i.get_input_values()));
  }

  //#define FIX_ISSUE_36
  #ifdef  FIX_ISSUE_36
  {
    std::vector<int> net_arc{1,23,45,678,9};
    int age = 123456789;
    ind_param i_p{net_arc, age};
    individual i{i_p};
    assert(i.get_net() == network{net_arc});
    assert(are_equal_with_tolerance(i.get_age(), age));
  }
  #endif


}
