#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include<vector>
#include<map>
#include"range.h"

class histogram
{
public:
    histogram(){};
    histogram( const std::vector<double>& values,
               int bin_number,
               value_range range_values);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(histogram,
                                   m_histogram);
    ///returns a constant reference to the range_count map
    const std::map<double,int>& hist() const noexcept {return m_histogram;}
    ///Add one observation in the correct key using lower_bound
    void add_observation(double value){
        m_histogram.lower_bound(value)->second++;
    }
    ///Reset all observations count to 0
    void reset_count() noexcept{std::for_each(m_histogram.begin(),
                                                     m_histogram.end(),
                                                     [](auto& key_value){return key_value.second = 0;});}


private:
    ///The count of observations that fall in a given number of bins
    /// that divide homogeneously a range of values
    std::map<double,int> m_histogram;
};

bool all_bins_have_same_n_obs_with_tolerance(const histogram &h);
bool all_counts_are_in_middle_bin(const histogram &h);
bool all_observations_counted(const histogram &h, int n_mutations);
#endif // HISTOGRAM_H
