/** 
 * @file filler.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_FILLER_H__
#define __WAPSTART_SWORDFISH_FILLER_H__
//-------------------------------------------------------------------------------------------------
#include <vector> 
#include <string>
extern "C" bool get_values_from_outside(const std::vector<std::string> &keys,
                                   std::vector<std::string> &values);

#endif //__WAPSTART_SWORDFISH_FILLER_H__
