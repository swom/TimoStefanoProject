#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "network.h"

class individual
{
public:
  individual( std::vector<int> net_arch = std::vector<int>{1,2,1}, int age = 0);

  ///Returns the age of the individual
  int get_age() const noexcept {return m_age;}

  ///Return const referernce to vector of fixed input values
  const std::vector<double>& get_input_values() const noexcept {return m_input_values;}

  ///Returns const ref to network
  const network& get_net() const noexcept {return m_network;}

  ///Returns ref to network
  network& get_net() noexcept {return m_network;}

private:

  ///The age of the individual
  int m_age;

  ///The vector of fixed input values that will be given to the network
  std::vector<double> m_input_values;

  ///The network of an individual
  network m_network;
};

std::vector<double> response(const individual& ind);

void test_individual();
#endif // INDIVIDUAL_H
