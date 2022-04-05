#ifndef NETWORK_SPECTRUM_H
#define NETWORK_SPECTRUM_H
#include "network.h"

class network_spectrum
{
public:
    network_spectrum(const network& n);
private:

    ///The ouput of the netwrok for each mutatation on each weight
    std::vector<std::vector<std::vector<std::vector<double>>>> m_outputs_of_mutated_weights;

    ///The ouput of the netwrok for each mutatation on each bias
    std::vector<std::vector<std::vector<double>>> m_outputs_of_mutated_biases;
};

#endif // NETWORK_SPECTRUM_H
