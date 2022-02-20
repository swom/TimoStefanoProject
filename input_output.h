#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H
#include <vector>

struct input_output
{
    input_output(){};
    input_output(std::vector<double> input,
                 std::vector<double> output);

    std::vector<double> m_input;
    std::vector<double> m_output;
};
//Extracts the first output of each element of an input_output vector
std::vector<double> extract_first_outputs(const std::vector<input_output>& v);
#endif // INPUT_OUTPUT_H
