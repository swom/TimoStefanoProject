#ifndef UTILITIES_H
#define UTILITIES_H
#include<vector>


bool are_equal_with_tolerance(double lhs, double rhs);

bool are_not_equal_with_tolerance(double lhs, double rhs);

///Claculates mean of a vector of doubles
double calc_mean(const std::vector<double> &numbers);

///Calculates stdev of vector of doubles
double calc_stdev(const std::vector<double>& numbers);

#endif // UTILITIES_H
