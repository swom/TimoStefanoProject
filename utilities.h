#ifndef UTILITIES_H
#define UTILITIES_H
#include<vector>
#include<sstream>
#include<random>

#include"rndutils.hpp"

bool are_equal_with_tolerance(double lhs, double rhs);

bool are_not_equal_with_tolerance(double lhs, double rhs);


bool are_equal_with_tolerance(int lhs, int rhs);

bool are_equal_with_more_tolerance(int lhs, int rhs);


///Claculates mean of a vector of doubles
double calc_mean(const std::vector<double> &numbers);

///Calculates stdev of vector of doubles
double calc_stdev(const std::vector<double>& numbers);

///Converts a net_arc vec of int to a string
const std::string convert_arc_to_string(const std::vector<int>& v);

///Checks if a vector of integers behaves like a nirmal distribution of given mean and variance
bool has_same_stdev_and_mean(const std::vector<double> &values,
                                      double mean,
                                      double var);
#endif // UTILITIES_H
