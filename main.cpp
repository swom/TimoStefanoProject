
#include "simulation.h"

#include <cassert>
#include <string>
#include <vector>

void test() {
  test_simulation();
}


int main(int argc, char ** argv) //!OCLINT tests may be long
{
  const std::vector<std::string> args(argv, argv + argc);
#ifndef NDEBUG
  test();
  if (args.size() > 1 && args[1] == "--test")
    {
      // We've already tested, so the program is done
      return 0;
    }
#else
  // In release mode, all asserts are removed from the code
  assert(1 == 2);
#endif

  simulation s;
  return 0;
}
