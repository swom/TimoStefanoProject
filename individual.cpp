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
                node_spectrum[std::distance(node->begin(), weight)] = weight_spectrum;
            }
            layer_spectrum[std::distance(layer->begin(), node)] = node_spectrum;
        }
        network_weights_spectrum[std::distance(mutable_net.get_net_weights().begin(), layer)] = layer_spectrum;
    }
    return network_weights_spectrum;
}

namespace spectrum{

net_w_spectrum calc_mutational_spectrum_weights_in_bins(const individual& ind,
                                             double mut_step,
                                             int n_mutations,
                                             std::mt19937_64& rng,
                                             int n_bins,
                                             const value_range& range)
{
    auto mutable_net = ind.get_net();
    std::normal_distribution<double> mut_dist(0,mut_step);

    net_w_spectrum network_weights_spectrum(mutable_net.get_net_weights().size());
    layer_w_spectrum layer_spectrum;
    node_w_spectrum node_spectrum;
    histogram h{{},n_bins, range};

    for(auto layer_it = mutable_net.get_net_weights().begin(); layer_it != mutable_net.get_net_weights().end(); layer_it++)
    {
        layer_spectrum.resize(layer_it->size());
        for(auto node_it = layer_it->begin(); node_it != layer_it->end(); node_it++)
        {
            node_spectrum.resize(node_it->size());
            for(auto weight = node_it->begin(); weight != node_it->end(); weight++)
            {
                auto original_weight = *weight;
                for(int i = 0; i != n_mutations; i++)
                {
                    *weight +=  mut_dist(rng);
                    auto response_value = response(mutable_net, ind.get_input_values())[0];
                    h.add_observation(response_value); // if observation exceeds range of histogram it is counted in first bin of histogram"
                    *weight = original_weight;
                }
                if(!all_observations_counted(h, n_mutations))
                {
                    std::cerr << "not all observation counted";
                }

                auto index_weight = std::distance(node_it->begin(), weight);
                node_spectrum[index_weight] = h;
                h.reset_count();
            }
            auto index_node = std::distance(layer_it->begin(), node_it);
            layer_spectrum[index_node] = node_spectrum;
        }
        auto index_layer = std::distance( mutable_net.get_net_weights().begin(), layer_it);
        network_weights_spectrum[index_layer] = layer_spectrum;
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

    for(auto layer_it = mutable_net.get_reference_to_biases().begin(); layer_it != mutable_net.get_reference_to_biases().end(); layer_it++)
    {
        layer_spectrum.resize(layer_it->size());
        for(auto  node_bias = layer_it->begin(); node_bias != layer_it->end(); node_bias++)
        {
            auto original_bias = *node_bias;
            for(int i = 0; i != n_mutations; i++)
            {
                *node_bias +=  mut_dist(rng);
                node_bias_spectrum[i] = response(mutable_net, ind.get_input_values());
                *node_bias = original_bias;
            }
            layer_spectrum[std::distance(layer_it->begin(), node_bias)] = node_bias_spectrum;
        }
        network_bias_spectrum[std::distance(mutable_net.get_reference_to_biases().begin(), layer_it)] = layer_spectrum;
    }
    return network_bias_spectrum;
}


net_b_spectrum calc_mutational_spectrum_biases_bins(const individual& ind,
                                                double mut_step,
                                                int n_mutations,
                                                std::mt19937_64& rng,
                                                int n_bins,
                                                const value_range& range)
{
    auto mutable_net = ind.get_net();
    std::normal_distribution<double> mut_dist(0,mut_step);
    net_b_spectrum network_bias_spectrum(mutable_net.get_biases().size());
    layer_b_spectrum layer_spectrum;
    histogram h{{},n_bins, range};

    for(auto layer_it = mutable_net.get_reference_to_biases().begin(); layer_it != mutable_net.get_reference_to_biases().end(); layer_it++)
    {
        layer_spectrum.resize(layer_it->size());
        for(auto  node_bias = layer_it->begin(); node_bias != layer_it->end(); node_bias++)
        {
            auto original_bias = *node_bias;
            for(int i = 0; i != n_mutations; i++)
            {
                *node_bias +=  mut_dist(rng);
                h.add_observation(response(mutable_net, ind.get_input_values())[0]);
                *node_bias = original_bias;
            }
            if(!all_observations_counted(h, n_mutations))
            {
                throw std::runtime_error{"not all observation counted"};
            }
            layer_spectrum[std::distance(layer_it->begin(), node_bias)] = h;
            h.reset_count();
        }
        network_bias_spectrum[std::distance(mutable_net.get_reference_to_biases().begin(), layer_it)] = layer_spectrum;
    }
    return network_bias_spectrum;
}

network_spectrum calculate_mutational_spectrum( const individual& ind,
                                                double mut_step,
                                                int n_mutations,
                                                std::mt19937_64& rng,
                                                int n_bins,
                                                const value_range& range)
{

    auto spectrum_weights = calc_mutational_spectrum_weights_in_bins(ind,
                                                                     mut_step,
                                                                     n_mutations,
                                                                     rng,
                                                                     n_bins,
                                                                     range);
    auto spectrum_biases = calc_mutational_spectrum_biases_bins(ind,
                                                                mut_step,
                                                                n_mutations,
                                                                rng,
                                                                n_bins,
                                                                range);

    return network_spectrum{spectrum_weights, spectrum_biases};

}

//end of namespace
}
//


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

    ///It is possible to calculate the mutational spectrum
    ///  of an individual's weights and biases
    {
        using namespace::spectrum;

        std::mt19937_64 rng;
        net_param n_p;
        n_p.function = idenity;
        n_p.net_arc = {1,1};
        individual i{{n_p}};
        int n_mutations_per_locus = 1000;
        double mut_step = 0.1;

        auto ind_output = response(i);
        assert(first_output_always_returns_target_value(i.get_net(), 0));



        auto i_before = i;
        auto spectrum_weights = calc_mutational_spectrum_weights(i,
                                                                 mut_step,
                                                                 n_mutations_per_locus,
                                                                 rng);
        assert(i_before == i);
        auto extracted_output_weights =  extract_first_outputs_weights(spectrum_weights);
        assert(has_same_stdev_and_mean(extracted_output_weights, *ind_output.begin(), mut_step));

        auto spectrum_biases = calc_mutational_spectrum_biases(i,
                                                               mut_step,
                                                               n_mutations_per_locus,
                                                               rng);
        assert(i_before == i);
        auto extracted_output_biases =  extract_first_outputs_biases(spectrum_biases);
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

    ///It is possible to record the mutational spectrum of a networks's weight and biases
    ///in the form of histograms (ouput_range - count) for each weight and bias
    {
        using namespace::spectrum;

        std::mt19937_64 rng;
        net_param n_p;
        n_p.function = idenity;
        n_p.net_arc = {1,1};
        individual i{{n_p}};
        int n_mutations_per_locus = 1000;
        double mut_step = 0.1;

        auto ind_output = *response(i).begin();
        assert(first_output_always_returns_target_value(i.get_net(), ind_output));

        ///Bin for a range that is triple the normal range of ouputs for a network
        /// centered around the starting individual of the individual
        int n_bins = 3;
        double very_high_value = 1000000;
        auto range_upper_bound = ind_output + i.get_net().get_activation_function()(very_high_value) * (n_bins - 1);
        auto range_lower_bound = ind_output - range_upper_bound;
        value_range range{range_lower_bound,range_upper_bound};


        auto net_spectrum = calculate_mutational_spectrum(i,
                                                          mut_step,
                                                          n_mutations_per_locus,
                                                          rng,
                                                          n_bins,
                                                          range);
        for(const auto& layer : net_spectrum.m_outputs_of_mutated_weights)
            for(const auto& node :layer)
                for(const auto& histogram : node)
                {
                    assert(all_counts_are_in_middle_bin(histogram));
                }
        for(const auto& layer : net_spectrum.m_outputs_of_mutated_biases)
            for(const auto& histogram :layer)
                {
                    assert(all_counts_are_in_middle_bin(histogram));
                }
    }

}
#endif
