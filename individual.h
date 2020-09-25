#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "network.h"

class individual
{
public:
  individual(int age = 0);

  ///Returns the age of the individual
  int get_age() const noexcept {return m_age;}

private:

  ///The age of the individual
  int m_age;



};

void test_individual();
#endif // INDIVIDUAL_H
