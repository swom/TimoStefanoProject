#include <cassert>
#include <string>
#include <vector>

#include "parser.h"

template<class S, class O>
void run_simulation(all_params params, obs_param obs_pars)
{
    S s{params};
    O o{obs_pars};
    exec(s, o);

    std::cout << "Saving" << std::endl;
    save_json(o,
              convert_arc_to_string(params.i_p.net_par.net_arc)+ "_" +
              std::to_string(params.s_p.seed) + "_" +
              params.i_p.net_par.str_func +
              "_A_" + std::to_string(params.e_p.targetA) +
              "_B_" + std::to_string(params.e_p.targetB) +
              "_e_ch_" + params.e_p.env_change_type_string +
              ".json");
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
    obs_param obs_pars = convert_obs_args(results);

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

    if(params.e_p.env_change_type == env_change_type::two_optima)
    {
        using env_t = environment<env_change_type::two_optima>;
        using sim_t = simulation<env_t>;
        using obs_t = observer<sim_t>;
        run_simulation<sim_t, obs_t>(params, obs_pars);
    }
    else if(params.e_p.env_change_type == env_change_type::drift)
    {
        using env_t = environment<env_change_type::drift>;
        using sim_t = simulation<env_t>;
        using obs_t = observer<sim_t>;
        run_simulation<sim_t, obs_t>(params, obs_pars);
    }
    else if(params.e_p.env_change_type == env_change_type::noise)
    {
        using env_t = environment<env_change_type::noise>;
        using sim_t = simulation<env_t>;
        using obs_t = observer<sim_t>;
        run_simulation<sim_t, obs_t>(params, obs_pars);
    }

    return 0;
}
