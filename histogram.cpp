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
        //Create map intervals
        auto step = range_values.calculate_extension() / bin_number;
        auto start_bin_value = range_values.m_start + step;
        for(int i = 0; i != bin_number; i ++)
        {
            m_histogram.insert({start_bin_value + step * i, 0});
        }

        if(values.size())
        {
            if(*std::max(values.begin(), values.end()) > range_values.m_end ||
                    *std::min(values.begin(), values.end()) < range_values.m_start)
            {
                throw std::invalid_argument{"The range provided does not encompass all values"};
            }

            //Count observations
            for(const auto& value : values)
            {
                m_histogram.lower_bound(value)->second ++;
            }
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

bool all_counts_are_in_middle_bin(const histogram &h)
{
    auto map = h.hist();
    auto middle_it = map.begin();
    std::advance(middle_it, map.size()/2);
    map.erase(middle_it);
    return std::all_of(map.begin(), map.end(), []
                       (const auto& map_pair){return map_pair.second == 0;});
}
