#ifndef IND_DATA_H
#define IND_DATA_H
#include"individual.h"

using namespace spectrum;
struct ind_data
{
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ind_data,
                                   ind,
                                   gen);
individual ind;
int gen;
};

struct ind_spectrum
{
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ind_spectrum,
                                   ind,
                                   ind_output,
                                   mutational_spectrum,
                                   gen);
individual ind;
std::vector<double> ind_output;
network_spectrum mutational_spectrum;
int gen;
};

#endif // IND_DATA_H
