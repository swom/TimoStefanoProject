#include "network.h"

#include <algorithm>
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

bool operator==(const network& lhs, const network& rhs)
{
  return lhs.get_input_size() == rhs.get_input_size() &&
  lhs.get_net_weights() == rhs.get_net_weights();
}

bool operator!=(const network& lhs, const network& rhs)
{
  return !(lhs == rhs);
}

network change_all_weights(network n, double new_weight)
{
  for(auto& layer : n.get_net_weights())
    for(auto& node : layer)
      for(auto& weight : node)
        {
          weight = new_weight;
        }
  return n;
}

std::vector<double> register_n_mutations(network n, double mut_rate, double mut_step, std::minstd_rand& rng, int repeats)
{
  std::vector<double> networks_weights;
  for(int i = 0; i != repeats; i++)
    {

      auto n_new = mutate(n, mut_rate, mut_step, rng);

      for(auto& layer : n_new.get_net_weights())
        for(auto& node : layer)
          for(auto& weight : node)
            {
              if(weight < -0.0001 || weight > 0.0001)
                networks_weights.push_back(weight);
            }
    }
  return  networks_weights;
}



std::vector<double> response(const network& n, const std::vector<double>& input)
{
  std::vector<double>previous_layer_value = input;
  assert(input.size() == n.get_input_size());

  for(size_t layer = 0; layer != n.get_net_weights().size(); layer++)
    {
      auto current_layer_values = std::vector<double>{};

      for(size_t node = 0; node != n.get_net_weights()[layer].size(); node++)
        {
          double node_value = std::inner_product(previous_layer_value.begin(),
                                                 previous_layer_value.end(),
                                                 n.get_net_weights()[layer][node].begin(),
                                                 0.0);
          current_layer_values.push_back(node_value);
        }

      previous_layer_value = current_layer_values;
    }
  ;
  auto output = previous_layer_value;
  return output;
}


network mutate (network n,
                const double& mut_rate,
                const double& mut_step,
                std::minstd_rand& rng)
{

  std::bernoulli_distribution mut_p{mut_rate};
  std::normal_distribution<double> mut_st{0,mut_step};

  for(auto& layer : n.get_net_weights())
    for(auto& node : layer)
      for(auto& weight : node)
        {
          if(mut_p(rng))
            {weight += mut_st(rng);}
        }

  return n;
}




#ifndef NDEBUG
void test_network() //!OCLINT
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
    //provided to the constructor
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

    //For simple net with weights == 0
    auto expected_output = std::vector<double>{0};
    network n{very_simple_nodes};
    auto output = response(n,input);
    assert(output == expected_output);

    //Let's change the weights of the network to something else than 0(e.g 1)
    double new_weight_value = 1.0;
    n = change_all_weights(n,new_weight_value);
    expected_output = std::vector<double>{2};
    output = response(n,input);
    assert(output == expected_output);

    //Testing a more complex arhitecture
    std::vector<int> not_too_simple_nodes{1,3,3,1};
    network not_too_simple{not_too_simple_nodes};
    not_too_simple = change_all_weights(not_too_simple, new_weight_value);
    expected_output = {1 * 3 * 3};
    output = response(not_too_simple, input);
    assert(output == expected_output);

  }

  ///Network weights mutate following a normal distribution
  {
    double mut_rate = 0.01;
    double mut_step = 0.1;
    std::minstd_rand rng;

    auto expected_mean_value  = 0;
    auto expected_stdev = mut_step;

    int repeats = 100000;

    auto very_simple_nodes = std::vector<int>{1,2,1};
    network n{very_simple_nodes};

    std::vector<double> networks_weights = register_n_mutations(n, mut_rate, mut_step, rng, repeats);

    auto mean = calc_mean(networks_weights);
    double stdev = calc_stdev(networks_weights);

    assert(mean - expected_mean_value < 0.01 && mean - expected_mean_value > -0.01);
    assert(stdev - expected_stdev < 0.001 && stdev - expected_stdev > -0.001);

  }
}
#endif
