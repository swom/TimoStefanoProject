#include "individual.h"

#include <cassert>

individual::individual(std::vector<int> net_arch, int age) :
  m_age{age},
  m_network{net_arch}
{

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
}
