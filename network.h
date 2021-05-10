#ifndef NETWORK_H
#define NETWORK_H
#include "utilities.h"
#include <iostream>
#include <vector>
#include <random>
#include "json.hpp"
class network
{
public:
    network(std::vector<int> nodes_per_layer);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(network,
                                   m_input_size,
                                   m_network_weights);

    ///Returns const ref to vector of weights
    const std::vector<std::vector<std::vector<double>>>& get_net_weights() const noexcept{return m_network_weights;}

    ///Returns not constant ref to vector of weights
    std::vector<std::vector<std::vector<double>>>& get_net_weights() noexcept{return m_network_weights;}

    ///Returns the input size
    size_t get_input_size() const noexcept {return static_cast<size_t>(m_input_size);}



private:
    ///Vector of of vectors, representing the weights coming into each node
    std::vector<std::vector<std::vector<double>>> m_network_weights;

    ///The size of the input vector the network will receive
    int m_input_size;
};

bool operator==(const network& lhs, const network& rhs);

bool operator!=(const network& lhs, const network& rhs);

network change_all_weights(network n, double new_weight);

///Mutates a network n times with given mutation
/// rate and step and returns vector all mutated weights
/// of network in all times it was mutated
std::vector<double> register_n_mutations(network n, double mut_rate, double mut_step, std::minstd_rand& rng, int repeats);

std::vector<double> response(const network& n, const std::vector<double> &input);

network mutate (network n, const double& mut_rate, const double& mut_step, std::minstd_rand &rng);

void test_network();


#endif // NETWORK_H
