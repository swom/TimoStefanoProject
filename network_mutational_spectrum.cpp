#include"network_mutational_spectrum.h"
namespace spectrum {

std::vector<double> extract_first_outputs_weights(const std::vector<std::vector<std::vector<std::vector<std::vector<double>>>>> &spectrum_weights)
{

    return flatten(flatten(flatten(flatten(spectrum_weights))));
}

std::vector<double> extract_first_outputs_biases(const std::vector<std::vector<std::vector<std::vector<double>>>> &spectrum_biases)
{
    return flatten(flatten(flatten(spectrum_biases)));
}
}
