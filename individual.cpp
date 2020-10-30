#include "individual.h"

#include <cassert>

individual::individual(int age) :
  m_age{age}
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
       individual i2{age};
       assert(i2.get_age() == age);
     }

}
