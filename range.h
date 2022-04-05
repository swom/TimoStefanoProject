#ifndef RANGE_H
#define RANGE_H
#include <vector>
#include "json.hpp"

struct value_range
{
    value_range(double start, double end);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(value_range,
                                   m_start,
                                   m_end);
    double m_start;
    double m_end;
    inline double calculate_extension() const noexcept{return std::abs(m_end - m_start);}
};

#endif // RANGE_H
