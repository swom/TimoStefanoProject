#include "input_output.h"
#include <stdexcept>

input_output::input_output(std::vector<double> input, std::vector<double> output):
    m_input(input),
    m_output(output)
{
    if(!input.size() || !output.size())
    {
        throw std::invalid_argument{"output or input have size 0"};
    }
}

std::vector<double> extract_first_outputs(const std::vector<input_output> &v)
{
  std::vector<double> outputs(v.size());
  for(size_t  i = 0; i != v.size(); i++)
  {
      outputs[i] = v[i].m_output[0];
  }
  return outputs;
}
