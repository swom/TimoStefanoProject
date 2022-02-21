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


std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> calc_mutational_spectrum_weights(const individual& ind,
                                                                                                         double mut_step,
                                                                                                         int n_mutations,
                                                                                                         std::mt19937_64& rng)
{
    auto mutable_net = ind.get_net();
    std::normal_distribution<double> mut_dist(0,mut_step);
    std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> network_weights_spectrum(mutable_net.get_net_weights().size());
    std::vector<std::vector<std::vector<std::vector<double>>>> layer_spectrum;
    std::vector<std::vector<std::vector<double>>> node_spectrum;
    std::vector<std::vector<double>> weight_spectrum(n_mutations);
    for(auto layer = mutable_net.get_net_weights().begin(); layer != mutable_net.get_net_weights().end(); layer++)
    {
        layer_spectrum.resize(layer->size());
        for(auto  node = layer->begin(); node != layer->end(); node++)
        {
            node_spectrum.resize(node->size());
            for(auto  weight = node->begin(); weight != node->end(); weight++)
            {
                auto original_weight = *weight;
                for(int i = 0; i != n_mutations; i++)
                {
                    *weight +=  mut_dist(rng);
                    weight_spectrum[i] = response(mutable_net, ind.get_input_values());
                    *weight = original_weight;
                }
                node_spectrum[std::distance(weight, node->begin())] = weight_spectrum;
            }
            layer_spectrum[std::distance(node, layer->begin())] = node_spectrum;
        }
        network_weights_spectrum[std::distance(layer, mutable_net.get_net_weights().begin())] = layer_spectrum;
    }
    return network_weights_spectrum;
}

std::vector<std::vector<std::vector<std::vector<double>>>> calc_mutational_spectrum_biases(const individual& ind,
                                                                                           double mut_step,
                                                                                           int n_mutations,
                                                                                           std::mt19937_64& rng)
{
    auto mutable_net = ind.get_net();
    std::normal_distribution<double> mut_dist(0,mut_step);
    std::vector<std::vector<std::vector<std::vector<double>>>> network_bias_spectrum(mutable_net.get_biases().size());
    std::vector<std::vector<std::vector<double>>> layer_spectrum;
    std::vector<std::vector<double>> node_bias_spectrum(n_mutations);

    for(auto layer = mutable_net.get_reference_to_biases().begin(); layer != mutable_net.get_reference_to_biases().end(); layer++)
    {
        layer_spectrum.resize(layer->size());
        for(auto  node_bias = layer->begin(); node_bias != layer->end(); node_bias++)
        {
            auto original_bias = *node_bias;
            for(int i = 0; i != n_mutations; i++)
            {
                *node_bias +=  mut_dist(rng);
                node_bias_spectrum[i] = response(mutable_net, ind.get_input_values());
                *node_bias = original_bias;
            }
            layer_spectrum[std::distance(node_bias, layer->begin())] = node_bias_spectrum;
        }
        network_bias_spectrum[std::distance(layer, mutable_net.get_reference_to_biases().begin())] = layer_spectrum;
    }
    return network_bias_spectrum;
}

network_spectrum calculate_mutational_spectrum( const individual& ind,
                                                double mut_step,
                                                int n_mutations,
                                                std::mt19937_64& rng)
{
    assert(response(ind).size());
    assert(mut_step);
    assert(n_mutations);

    auto spectrum_weights = calc_mutational_spectrum_weights(ind,
                                                             mut_step,
                                                             n_mutations,
                                                             rng);
    auto spectrum_biases = calc_mutational_spectrum_biases(ind,
                                                           mut_step,
                                                           n_mutations,
                                                           rng);

    return network_spectrum{spectrum_weights, spectrum_biases};

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
        assert( response(i) == response(i.get_net(),i.get_input_values(), &idenity));
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
        std::mt19937_64 rng;
        net_param n_p;
        n_p.function = idenity;
        n_p.net_arc = {1,1};
        individual i{{n_p}};

        auto ind_output = response(i);
        assert(first_output_always_returns_target_value(i.get_net(), 0));


        int n_mutations_per_locus = 1000;
        double mut_step = 0.1;

        auto i_before = i;
        network_spectrum network_spectrum = calculate_mutational_spectrum(i,
                                                                          mut_step,
                                                                          n_mutations_per_locus,
                                                                          rng);

        ///to make sure individual is not modified throughout the process
        assert(i_before == i);

        auto extracted_output_weights =  extract_first_outputs_weights(network_spectrum);

        assert(has_same_stdev_and_mean(extracted_output_weights, *ind_output.begin(), mut_step));

        auto extracted_output_biases =  extract_first_outputs_biases(network_spectrum);

        assert(has_same_stdev_and_mean(extracted_output_biases, *ind_output.begin(), mut_step));
    }

    #define FIX_BINS
#ifdef FIX_BINS
    ///It is possible to "bin" the values of a vector of doubles
    /// to obtain a vector of <value_ranges, observation_count> pairs
    {
        int bin_number = 3;
        rndutils::xorshift128 rng;
        double min = - 1;
        double max = 1;
        std::uniform_real_distribution dist(min,max);
        size_t size = 1000;
        std::vector<double> values(size);

        for(auto& value : values)
        {
            value = dist(rng);
        }

        histogram binned_values(values, bin_number, value_range{min, max});

        assert(static_cast<int>(binned_values.hist().size()) == bin_number);
        assert(all_bins_have_same_n_obs_with_tolerance(binned_values));
    }
#endif

}
#endif
