#include "utilities.h"
#include<algorithm>
#include<random>
#include<cmath>
#include<numeric>

bool are_equal_with_tolerance(double lhs, double rhs)
{
    return lhs - rhs > -0.0001 && lhs - rhs < 0.0001;
}

bool are_equal_with_more_tolerance(double lhs, double rhs)
{
    return lhs - rhs > -0.005 && lhs - rhs < 0.005;
}

bool are_equal_with_high_tolerance(double lhs, double rhs)
{
    return lhs - rhs > -0.05 && lhs - rhs < 0.05;
}

bool are_not_equal_with_tolerance(double lhs, double rhs)
{
    return !are_equal_with_tolerance(lhs, rhs);
}

double calc_mean(const std::vector<double>& numbers){
    return std::accumulate(numbers.begin(),
                           numbers.end(), 0.0)/numbers.size();
}

double calc_stdev(const std::vector<double>& numbers)
{
    double accum = 0.0;
    auto mean = calc_mean(numbers);
    std::for_each (std::begin(numbers),
                   std::end(numbers),
                   [&](const double weight) {
        accum += (weight - mean) * (weight - mean);});

    return sqrt(accum / (numbers.size()-1));
}

const std::string convert_arc_to_string(const std::vector<int>& v)
{
    std::stringstream ss;
    for(size_t i = 0; i < v.size(); ++i)
    {
        if(i != 0)
            ss << "-";
        ss << v[i];
    }
    return ss.str();
}


bool has_same_stdev_and_mean(const std::vector<double>& values,
                                      double mean,
                                      double var
                                      )
{
    auto calculated_mean = calc_mean(values);
    auto calculated_stdev = calc_stdev(values);
    auto means_are_equal = are_equal_with_high_tolerance(mean, calculated_mean);
    auto stdevs_are_equal = are_equal_with_high_tolerance(var, calculated_stdev);

    return means_are_equal && stdevs_are_equal && values.size();
}

bool are_equal_with_tolerance(int lhs, int rhs)
{
    return lhs - rhs > -10 && lhs - rhs < 10;

}

bool are_equal_with_more_tolerance(int lhs, int rhs)
{
    return lhs - rhs > -100 && lhs - rhs < 100;

}
