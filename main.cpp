#include <cassert>
#include <string>
#include <vector>

#include "parser.h"

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
    auto results = create_parser().parse(argc,argv);
    all_params params{
        convert_env_args(results),
                convert_ind_args(results),
                convert_pop_args(results),
                convert_sim_args(results)
    };
#ifndef NDEBUG
    if(results["test"].as<bool>())
    {
        test();
        // We've already tested, so the program is done
        std::cout << "done testing" << std::endl;
        return 0;
    }
#else
    // In release mode, all asserts are removed from the code
    assert(1 == 2);
#endif

    simulation s{params};
    observer o{convert_obs_args(results)};
    exec(s, o);

    std::cout << "Saving" << std::endl;
    save_json(o,
              convert_arc_to_string(params.i_p.net_par.net_arc)+ "_" +
              std::to_string(params.s_p.seed) + "_" +
              params.i_p.net_par.str_func +
              ".json");

    return 0;
}
