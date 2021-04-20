#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "network.h"

class individual
{
public:
  individual( std::vector<int> net_arch = std::vector<int>{1,2,1}, int age = 0);

  ///Returns the age of the individual
  int get_age() const noexcept {return m_age;}

  ///Returns const ref to network
  const network& get_net() const noexcept {return m_network;}

private:

  ///The age of the individual
  int m_age;

  ///The network of an individual
  network m_network;
};

void test_individual();
#endif // INDIVIDUAL_H
