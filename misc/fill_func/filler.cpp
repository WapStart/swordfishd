#include "filler.h"
#include <stdio.h>
bool get_values_from_outside(const std::vector<std::string> &keys,std::vector<std::string> &values)
{
  printf("hello from filler!\n");
  std::vector<std::string>::const_iterator it_key = keys.begin();
  values.erase(values.begin(), values.end());
  while (it_key != keys.end())
  {
    values.push_back(*it_key + "_val");
    ++it_key;
  }
  return true;
}
