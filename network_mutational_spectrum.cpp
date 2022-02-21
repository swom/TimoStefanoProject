#include"network_mutational_spectrum.h"

std::vector<double> extract_first_outputs_weights(const network_spectrum &v)
{

    return flatten(flatten(flatten(flatten(v.m_outputs_of_mutated_weights))));
}

std::vector<double> extract_first_outputs_biases(const network_spectrum &v)
{
    return flatten(flatten(flatten(v.m_outputs_of_mutated_biases)));
}
