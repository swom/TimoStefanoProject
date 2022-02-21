#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include<vector>
#include<map>
#include"range.h"

class histogram
{
public:
    histogram( const std::vector<double>& values,
               int bin_number,
               value_range range_values);

    ///returns a constant reference to the range_count map
    const std::map<double,int>& hist() const noexcept {return m_binned_values_count;}

private:
    ///The count of observations that fall in a given number of bins
    /// that divide homogeneously a range of values
    std::map<double,int> m_binned_values_count;
};

bool all_bins_have_same_n_obs_with_tolerance(const histogram &h);
#endif // HISTOGRAM_H
