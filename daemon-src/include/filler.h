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
#include <boost/property_tree/ptree.hpp>

extern "C" bool get_values_from_outside(
  const std::vector<std::string> &keys,
  std::vector<std::string> &values,
  boost::property_tree::ptree *config
);

extern "C" void normalize_key(
  const std::string &key/*in*/,
  std::string &normalized_key/*out*/
);

extern "C" void custom_hash(const std::string &value/*in*/, size_t &hash/*out*/);
#endif //__WAPSTART_SWORDFISH_FILLER_H__
