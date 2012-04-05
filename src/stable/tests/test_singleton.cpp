/**
 * @file test_singleton.cpp
 * @autor Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <utils/dummy_mutex.hpp>
#include <utils/singleton.hpp>
//-------------------------------------------------------------------------------------------------
#define BOOST_TEST_MODULE test_swordfish
#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE(test_singleton)
//-------------------------------------------------------------------------------------------------
struct WithholdingType {
  int a1_;
  int a2_;
  int a3_;
  WithholdingType(): a1_(1), a2_(2), a3_(3) {;}
};
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(test_singleton_creation)
{
  using namespace wapstart::utils;

  SingletonHolder<WithholdingType, DummyMutex> singleton;

  BOOST_REQUIRE(singleton.instance().a1_ == 1);
  BOOST_REQUIRE(singleton.instance().a2_ == 2);
  BOOST_REQUIRE(singleton.instance().a3_ == 3);
}
//-------------------------------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-------------------------------------------------------------------------------------------------

