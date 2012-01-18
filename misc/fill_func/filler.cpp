#include "filler.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
bool get_values_from_outside(const std::vector<std::string> &keys,std::vector<std::string> &values)
{
  std::vector<std::string>::const_iterator it_key = keys.begin();
  values.erase(values.begin(), values.end());
  while (it_key != keys.end())
  {
    std::string line, val;

    //std::ifstream myfile ("example.txt");
    // if (myfile.is_open())
    // {
    //   while ( myfile.good() )
    //   {
    //     getline (myfile,line);
    //     val += "_boo";
         //cout << line << endl;
    //   }
    //   myfile.close();
     
    //}

  //   else std::cout << "Unable to open file";
     if (rand() % 2)
       values.push_back(*it_key + "_boo");
     else
       values.push_back("repeat");
     ++it_key;
  }
  return true;
}
