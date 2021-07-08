#include "observer.h"
#include "parser.h"
#include <cassert>
#include <string>
#include <vector>


#ifndef NDEBUG
void test() {
  test_environment();
  test_individual();
  test_network();
  test_observer();
  test_parser();
  test_population();
  test_simulation();
}
#endif

int main(int argc, char ** argv) //!OCLINT tests may be long
{
  const std::vector<std::string> args(argv, argv + argc);
#ifndef NDEBUG
  if (args.size() > 1 && args[1] == "--test")
    {
      test();
      // We've already tested, so the program is done
      return 0;
    }
#else
  // In release mode, all asserts are removed from the code
  assert(1 == 2);
#endif

  env_param e_p{};
  e_p.targetA = parse_targetA(args);
  e_p.targetB = parse_targetB(args);

  net_param n_p;
  n_p.function = parse_act_func(args);
  n_p.net_arc = parse_net_arc(args);

  ind_param i_p{n_p, 0};

  pop_param p_p{};
  p_p.mut_rate = parse_mut_rate(args);
  p_p.mut_step = parse_mut_step(args);
  p_p.number_of_inds = parse_pop_size(args);

  simulation s {0.5, 0, 1000, 0, 0.1, {1,10,2,1}, 2};
  observer o;
  exec(s, o, 10);

  save_json(o, "test_observer.json");

  return 0;
}
