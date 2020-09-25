#include "network.h"
#include <cassert>

network::network(std::vector<int> nodes_per_intermediate_layer)
{
    m_network_weights.push_back(std::vector<double>{1});
    for (size_t i = 0; i != nodes_per_intermediate_layer.size(); i++)
    {
        std::vector <double> temp (nodes_per_intermediate_layer[i],0);
        m_network_weights.push_back(temp);
    }
    m_network_weights.push_back(std::vector<double>{1});

}

void test_network()
{
    ///A network can be initialized
    /// by a vector of int that is going to
    /// define the number of intermediate layers(size of the vector)
    /// and nodes in those layers(values of the vector)
    /// A network will always have a single input node and a single output node
    /// The value of the input will stay fixed at 1
    {
        std::vector<int> testvec{1, 2, 3} ;
        network n {testvec} ;
        //input has only one node
        assert(n.get_net_weights()[0].size() == 1u);
        //value of input is 1
        assert(n.get_net_weights()[0][0] == 1);

        //output has only one node
        assert(n.get_net_weights().back().size() == 1u);


        assert ( n.get_net_weights().size() == testvec.size());
        for( size_t i = 1; i != testvec.size() - 1; i++ )
        {
            assert(n.get_net_weights()[i].size() == static_cast<size_t>(testvec[i]));
        }
    }

    ///The function resposne returns the output of the network
    {

    }

}
