#include<algorithm>
#include<stdexcept>
#include<iostream>
#include "histogram.h"
#include"utilities.h"
histogram::histogram(const std::vector<double>& values,
                     int bin_number,
                     value_range range_values)

{
    try
    {
        if(*std::max(values.begin(), values.end()) > range_values.m_end ||
                *std::min(values.begin(), values.end()) < range_values.m_start)
        {
            throw std::invalid_argument{"The range provided does not encompass all values"};
        }

        //Create map intervals
        auto step = range_values.calculate_extension() / bin_number;
        auto start_bin_value = range_values.m_start + step;
        for(int i = 0; i != bin_number; i ++)
        {
            m_binned_values_count.insert({start_bin_value + step * i, 0});
        }

        //Count observations
        for(const auto& value : values)
        {
            auto it = m_binned_values_count.lower_bound(value)++;
            it->second ++;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "In Histogram: " << e.what() << std::endl;
    }
}

bool all_bins_have_same_n_obs_with_tolerance(const histogram &h)
{
    if (h.hist().empty())
    {
        throw std::invalid_argument{"When comparing bins of an histogram there are no bins"};
    }
        auto val = h.hist().begin()->second;
        return std::all_of(std::next(h.hist().begin()), h.hist().end(),
                             [val](typename std::map<double, int>::const_reference t){ return are_equal_with_more_tolerance(t.second, val); });

}
