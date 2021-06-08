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

    ///Returns the const ref to the node biases
    const std::vector<std::vector<double>>& get_biases() const noexcept{return m_nodes_biases;}

    ///Returns const ref to vector of weights
    const std::vector<std::vector<std::vector<double>>>& get_net_weights() const noexcept{return m_network_weights;}

    ///Returns not constant ref to vector of weights
    std::vector<std::vector<std::vector<double>>>& get_net_weights() noexcept{return m_network_weights;}

    ///Returns the input size
    size_t get_input_size() const noexcept {return static_cast<size_t>(m_input_size);}

    ///Mutates the network
    void mutate(const double& mut_rate, const double& mut_step, std::mt19937_64 &rng);

private:
    ///Vector of of vectors, representing the weights coming into each node
    std::vector<std::vector<std::vector<double>>> m_network_weights;

    ///Vector of vectors containing the nodes biases stored per layer per node
    std::vector<std::vector<double>> m_nodes_biases;

    ///The size of the input vector the network will receive
    int m_input_size;
};

bool operator==(const network& lhs, const network& rhs);

bool operator!=(const network& lhs, const network& rhs);

network change_all_weights(network n, double new_weight);

///Mutates a network n times with given mutation
/// rate and step and returns vector all mutated weights
/// of network in all times it was mutated
std::vector<double> register_n_mutations(network n, double mut_rate, double mut_step, std::mt19937_64 &rng, int repeats);

double sigmoid(double x);
double linear(double x);

std::vector<double> response(const network& n, const std::vector<double>& input, std::function<double(double)> fun = &linear);


void test_network();

#endif // NETWORK_H
