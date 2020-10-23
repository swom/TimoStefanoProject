#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>

class network
{
public:
    network(std::vector<int> nodes_per_layer);

    ///Returns const ref to vector of weights
    const std::vector<std::vector<std::vector<double>>>& get_net_weights() const noexcept{return m_network_weights;}

    ///Returns the input size
    size_t get_input_size() const noexcept {return static_cast<size_t>(m_input_size);}

private:
    ///Vector of of vectors, representing the weights coming into each node
    std::vector<std::vector<std::vector<double>>> m_network_weights;

    ///The size of the input vector the network will receive
    int m_input_size;
};

std::vector<double> response (const network& n);

void test_network();


#endif // NETWORK_H
