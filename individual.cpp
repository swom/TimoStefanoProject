#include "individual.h"

#include <algorithm>
#include <cassert>

individual::individual(std::vector<int> net_arch, int age) :
    m_age{age},
    ///!!!!Attention!!!! input values are for now a fixed amount
    m_input_values(net_arch[0], 1.0),
    m_network{net_arch}
{

}

individual::individual(ind_param i_p) :
    ///!!!!Attention!!!! input values are for now a fixed amount
    m_input_values(i_p.net_par.net_arc[0], 1.0),
    m_network{i_p.net_par}
{

}





bool operator== (const individual& lhs, const individual& rhs)
{
    bool fitness = are_equal_with_tolerance(lhs.get_fitness(), rhs.get_fitness());
    bool network = lhs.get_net() == rhs.get_net();
    bool inputs = lhs.get_input_values() == rhs.get_input_values();

    return fitness && network && inputs;
}

double calc_sqr_distance(const individual& i, double env_value)
{
    return (response(i)[0] - env_value) * (response(i)[0] - env_value);
}

void individual::mutate(double mut_rate, double mut_step, std::mt19937_64& rng)
{
    m_network.mutate(mut_rate, mut_step, rng);
}

std::vector<double> response(const individual& ind)
{
    return response(ind.get_net(),ind.get_input_values());
}

std::vector<int> calculate_mutational_spectrum(const individual& ind,
                                               double mut_rate,
                                               double mut_step,
                                               int n_mutations,
                                               int bins)
{
    //Stub
    assert(response(ind).size());
    assert(mut_rate);
    assert(mut_step);
    assert(n_mutations);
    assert(bins);
    return std::vector<int>{};
    //
}

#ifndef NDEBUG
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

    ///Individuals have a vector of fixed input values, always equal to 1, for their network
    {
        int n_input = 456;
        std::vector<int> net_arch{n_input};
        individual i{net_arch};
        assert(i.get_input_values().size() == static_cast<size_t>(n_input));
        for(const auto& value : i.get_input_values())
        {
            assert(are_equal_with_tolerance(value, 1.0));
        }
    }

    ///When an individual responds to environment it uses its input values as input
    {
        individual i;
        assert( response(i) == response(i.get_net(),i.get_input_values(), &linear));
    }
    //#define FIX_ISSUE_36


    {
        net_param net_par;
        ind_param i_p{net_par};
        individual i{i_p};
        assert(i.get_net() == network{net_par});
    }

    ///It is possible to calculate the mutational spectrum of an individual
    {
        net_param n_p;
        n_p.function = linear;
        n_p.net_arc = {1,1};
        individual i{{n_p}};
        auto ind_output = response(i);
        assert(behaves_like_identity_function(i.get_net()));


        int n_output_bins = 3;
        int n_mutations_per_locus = 1000;
        double mut_rate = 1;
        double mut_step = 0.1;

        auto i_before = i;
        auto output_distribution = calculate_mutational_spectrum(i,
                                                                 mut_rate,
                                                                 mut_step,
                                                                 n_mutations_per_locus,
                                                                 n_output_bins);
        assert(i_before == i);
        assert(behaves_like_normal_distribution(*ind_output.begin(), mut_step, output_distribution));
    }

}
#endif
