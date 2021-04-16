#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <random>

class network
{
public:
    network(std::vector<int> nodes_per_layer);

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

///Claculates mean of a vector of doubles
double calc_mean(const std::vector<double> &numbers);

///Calculates stdev of vector of doubles
double calc_stdev(const std::vector<double>& numbers);

network change_all_weights(network n, double new_weight);

///Mutates a network n times with given mutation
/// rate and step and returns vector all mutated weights
/// of network in all times it was mutated
std::vector<double> register_n_mutations(network n, double mut_rate, double mut_step, std::minstd_rand& rng, int repeats);

std::vector<double> response (const network& n);

network mutate (network n, const double& mut_rate, const double& mut_step, std::minstd_rand &rng);

void test_network();


#endif // NETWORK_H
