/**
 * @file test_command_tokenizer.cpp
 * @autor Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <iostream>
#include <boost/test/unit_test.hpp>
//-------------------------------------------------------------------------------------------------
#include <command_tokenizer.hpp>
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(test_command_tokenizer)
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_1)
{
  const std::string cmd = "get 1 2 3";
  
  using namespace wapstart;
  
  CommandTokenizer ct;

  ct.append(cmd);

  size_t counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it)
    ++counter;

  BOOST_REQUIRE(!counter);
  BOOST_REQUIRE(ct.remainder() == cmd);
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_2)
{
  const std::string cmd = "get 1 2 3\r\nget 4 5 6\r\nget 7 8 9";
  
  using namespace wapstart;
  
  CommandTokenizer ct;

  ct.append(cmd);

  size_t counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    switch(counter) {
      case 0: BOOST_REQUIRE(*it == "get 1 2 3"); break;
      case 1: BOOST_REQUIRE(*it == "get 4 5 6"); break;
    }
    ++counter;
  }

  BOOST_REQUIRE(counter == 2);
  BOOST_REQUIRE(ct.remainder() == "get 7 8 9");
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_3)
{
  const std::string cmd = "get 1 2 3\r\n";
  
  using namespace wapstart;
  
  CommandTokenizer ct;

  ct.append(cmd);

  size_t counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    BOOST_REQUIRE(*it == "get 1 2 3");
    ++counter;
  }
  BOOST_REQUIRE(counter == 1);
  BOOST_REQUIRE(ct.remainder() == "");
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_4)
{
  const std::string cmd = "get 1 2 3\r\nget 4 5 6\r\nget 7 8 9\r\n";
  
  using namespace wapstart;
  
  CommandTokenizer ct;

  ct.append(cmd);

  size_t counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    switch(counter) {
      case 0: BOOST_REQUIRE(*it == "get 1 2 3"); break;
      case 1: BOOST_REQUIRE(*it == "get 4 5 6"); break;
      case 2: BOOST_REQUIRE(*it == "get 7 8 9"); break;
    }
    ++counter;
  }

  BOOST_REQUIRE(counter == 3);
  BOOST_REQUIRE(ct.remainder() == "");
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_5)
{
  const std::string cmd1 = "get 1 2 3\r\nget 4 5 6\r\nget 7 8 9";
  const std::string cmd2 = "\r\nget 10 11 12\r\nget 13";
  const std::string cmd3 = " 14 15\r\nget 16 17 18";
  const std::string cmd4 = "\r\n";
  
  using namespace wapstart;
  
  CommandTokenizer ct;

  ct.append(cmd1);

  size_t counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    switch(counter) {
      case 0: BOOST_REQUIRE(*it == "get 1 2 3"); break;
      case 1: BOOST_REQUIRE(*it == "get 4 5 6"); break;
    }
    ++counter;
  }

  BOOST_REQUIRE(counter == 2);
  BOOST_REQUIRE(ct.remainder() == "get 7 8 9");
  
  ct.append(cmd2);
  
  counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    switch(counter) {
      case 0: BOOST_REQUIRE(*it == "get 7 8 9"); break;
      case 1: BOOST_REQUIRE(*it == "get 10 11 12"); break;
    }
    ++counter;
  }
  
  BOOST_REQUIRE(counter == 2);
  BOOST_REQUIRE(ct.remainder() == "get 13");
  
  ct.append(cmd3);
  
  counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    BOOST_REQUIRE(*it == "get 13 14 15");
    ++counter;
  }
  
  BOOST_REQUIRE(counter == 1);
  BOOST_REQUIRE(ct.remainder() == "get 16 17 18");
  
  ct.append(cmd4);
  
  counter = 0;
  for(CommandTokenizer::iterator it = ct.begin(); it != ct.end(); ++it) {
    BOOST_REQUIRE(*it == "get 16 17 18");
    ++counter;
  }
  
  BOOST_REQUIRE(counter == 1);
  BOOST_REQUIRE(ct.remainder() == "");
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-------------------------------------------------------------------------------------------------
