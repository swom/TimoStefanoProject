#include "network.h"

#include <cassert>
#include <cmath>
#include <numeric>

network::network(std::vector<int> nodes_per_intermediate_layer)
{

    for (size_t i = 0; i != nodes_per_intermediate_layer.size()-1; i++ ){
        int number_connections = nodes_per_intermediate_layer[i] * nodes_per_intermediate_layer[i + 1];
        m_network_weights.push_back(std::vector <double>(number_connections, 0));
    }
}


std::vector<double> response (const network& n, const std::vector<double>& input)
{
    std::vector<double>previous_layer_value = input;

    assert(input.size() == n.get_net_weights()[0].size());

    for(size_t i = 0; i != n.get_net_weights().size(); i++)
    {
        auto temp_vector = std::vector<double>{};

        for(size_t j = 0; j != n.get_net_weights()[i].size(); j++)
        {
          double node_value = std::inner_product(previous_layer_value.begin(),
                                                 previous_layer_value.end(),
                                                 n.get_net_weights()[i].begin(),
                                                 0.0);
          temp_vector.push_back(node_value);
        }

        previous_layer_value = temp_vector;
    }
    ;
    auto output = previous_layer_value;
    return output;
}

void test_network()
{
    ///A network can be initialized
    /// by a vector of int that is going to
    /// define the number of layers(size of the vector)
    /// and nodes in those layers(values of the vector)
    {
        std::vector<int> testvec{1, 2, 3, 1} ;
        network n {testvec} ;

        //The number of rows of the weight matrix
        //should be the number elements of the vector
        //privded to the constructor
        // minus 1 (the output layer does not have forward connections)
        assert(n.get_net_weights().size() == testvec.size() -1);

        for(size_t  i = 0; i != n.get_net_weights().size(); i++)
        {
            assert (static_cast<int>(n.get_net_weights()[i].size()) == testvec[i]*testvec[i+1]);
        }

    }

    ///The function resposne returns the output of the network
    {
        auto very_simple_nodes = std::vector<int>{1,1};
        auto input = std::vector<double>{1};
        auto expected_output = std::vector<double>{0};
        network n{very_simple_nodes};
        auto output = response(n,input);
        assert(output == expected_output);
    }
}
