#include "network.h"

#include <cassert>
#include <cmath>
#include <numeric>

network::network(std::vector<int> nodes_per_layer):
    m_input_size{nodes_per_layer[0]}
{
    for (size_t i = 1; i != nodes_per_layer.size(); i++ )
    {
        std::vector<std::vector<double>>temp_layer_vector;
        size_t n_nodes_prev_layer = nodes_per_layer[i-1];
        for(int j = 0; j != nodes_per_layer[i]; j++)
        {
          std::vector<double> temp_weights(n_nodes_prev_layer, 0);
          temp_layer_vector.push_back(temp_weights);
        }

        //A vector of the size of the number of connections is pushed back in the weight matrix
        m_network_weights.push_back(temp_layer_vector);
    }
}


std::vector<double> response(const network& n, const std::vector<double>& input)
{
    std::vector<double>previous_layer_value = input;
    assert(input.size() == n.get_input_size());

    for(size_t i = 0; i != n.get_net_weights_const().size(); i++)
    {
        auto temp_vector = std::vector<double>{};

        for(size_t j = 0; j != n.get_net_weights_const()[i].size(); j++)
        {
          double node_value = std::inner_product(previous_layer_value.begin(),
                                                 previous_layer_value.end(),
                                                 n.get_net_weights_const()[i][j].begin(),
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
        // minus 1 (the input layer does not have connections to previous layers)
        assert(n.get_net_weights().size() == testvec.size() -1);


        //The size of the input is stored even though is not saved in n_connection_weights
        //
        for(size_t  i = 0; i != n.get_net_weights().size(); i++)
        {
            assert (static_cast<int>(n.get_net_weights()[i].size()) == testvec[i + 1]);
        }

    }

    ///The function resposne returns the output of the network
    {
        auto very_simple_nodes = std::vector<int>{1,2,1};
        auto input = std::vector<double>{1};
        auto expected_output = std::vector<double>{0};
        network n{very_simple_nodes};
        auto output = response(n,input);
        assert(output == expected_output);

        //Let-s change the weights of the network to something else than 0(e.g 1)
        for(auto& layer : n.get_net_weights())
            for(auto& node : layer)
                for(auto& weight : node)
                {
                    weight = 1;
            }
        expected_output = std::vector<double>{2};
        output = response(n,input);
        assert(output == expected_output);

    }





}
