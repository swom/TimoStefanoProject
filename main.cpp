#include <cassert>
#include <string>
#include <vector>

#include "parser.h"

#include "Stopwatch.hpp"

///Executes a simulation for n generations
template<class S, class O>
void exec(S &s , O &o)
{
    using namespace stopwatch;
    Stopwatch sw;
    o.store_par(s);

    while(s.get_time() < s.get_n_gen())
    {
        tick(s);
        o.store_avg_fit_and_env(s);
        o.store_data_about_inds(s);
        if(s.get_time() % 1000 == 0)
        {
            std::cout << "Cycle " << s.get_time() << ". Elapsed: " << sw.lap<stopwatch::s>() << " seconds." << std::endl;
        }
    }
}


#ifndef NDEBUG
void test() {
    std::cout << "testing environment:";
    test_environment();
    std::cout << " environment tested" << std::endl;
    std::cout << "testing individual:";
    test_individual();
    std::cout << " individual tested" << std::endl;
    std::cout << "testing network:";
    test_network();
    std::cout << " network tested" << std::endl;
    std::cout << "testing observer:";
    test_observer();
    std::cout << " observer tested" << std::endl;
    std::cout << "testing parser:";
    test_parser();
    std::cout << " parser tested" << std::endl;
    std::cout << "testing population:";
    test_population();
    std::cout << " population tested" << std::endl;
    std::cout << "testing simulation:";
    test_simulation();
    std::cout << " simulation tested" << std::endl;
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
              "_A_" + std::to_string(params.e_p.targetA) +
              "_B_" + std::to_string(params.e_p.targetB) +
              ".json");

    return 0;
}
