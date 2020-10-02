#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>

class network
{
public:
    network(std::vector<int> nodes_per_layer);

    ///Returns const ref to vector of weights
    const std::vector<std::vector<double>>& get_net_weights() const noexcept{return m_network_weights;}

private:

    std::vector<std::vector<double>> m_network_weights;
};

std::vector<double> response (const network& n);

void test_network();


#endif // NETWORK_H
