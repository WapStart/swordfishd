/** 
 * @file base_exception.hpp
 * @autor Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_BASE_EXCEPTION_H__
#define __WAPSTART_SWORDFISH_BASE_EXCEPTION_H__
//-------------------------------------------------------------------------------------------------
#include <boost/exception/all.hpp>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class base_exception: public virtual boost::exception, public virtual std::exception {};
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_BASE_EXCEPTION_H__
//-------------------------------------------------------------------------------------------------

