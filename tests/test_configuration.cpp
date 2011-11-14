/**
 * @file test_configuration.cpp
 * @autor Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <sstream>
#include <boost/test/unit_test.hpp>
//-------------------------------------------------------------------------------------------------
#include <cfg.hpp>
//-------------------------------------------------------------------------------------------------
const std::string _config_text = 
"[general]\n \
;\n\
;\n\
;\n\
log_level = INFO\n";
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(test_configuration)
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_parse)
{
  using namespace wapstart;
  
  std::stringstream stream;

  stream << _config_text;

  Config config;

  config.load(stream);

  BOOST_REQUIRE(config.log_level() == 6);
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_file_not_found)
{
  using namespace wapstart;
  
  BOOST_REQUIRE_THROW(Config cfg0("/some/missing/path/something.ini"), Config::file_not_found);

  Config cfg1;

  BOOST_REQUIRE_THROW(cfg1.load("/some/missing/path/something.ini"), Config::file_not_found);
  BOOST_REQUIRE_THROW(cfg1.reload(), Config::file_not_found);
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-------------------------------------------------------------------------------------------------
