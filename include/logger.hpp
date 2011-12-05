/** 
 * @file logger.hpp
 * @author Alina Litvinova
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_LOGGER__H__
#define __WAPSTART_SWORDFISH_LOGGER__H__
//-------------------------------------------------------------------------------------------------
#include <ostream>
#include <boost/log/detail/prologue.hpp>
#include <boost/log/keywords/severity.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
//-------------------------------------------------------------------------------------------------
#include "log_level.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace privacy {
    void logger_init();
    //---------------------------------------------------------------------------------------------
  } // namespace privacy
  //-----------------------------------------------------------------------------------------------
  typedef boost::log::sources::severity_logger_mt<LogLevel::type> swordfish_logger;

  BOOST_LOG_DECLARE_GLOBAL_LOGGER_INIT(logger, swordfish_logger)
  {
    privacy::logger_init();
    return swordfish_logger();
  }
  //-----------------------------------------------------------------------------------------------
  void set_log_severity_level(LogLevel::type level);
  //-----------------------------------------------------------------------------------------------
  /**
   * Инициализирует файловый логгер.
   * @param path_base  
   * @param rot_size Частота ротирования в часах.
   * @param rot_frequency Размер ротирования в мегабайтах. 
   */
  void file_logger_init(const std::string &path_base,
                        size_t             rot_size,
                        size_t             rot_frequency);
  //-----------------------------------------------------------------------------------------------
  /**
   *
   */
  void syslog_logger_init();
  //----------------------------------------------------------------------------------------------
  /**
   *
   */
  void stdout_logger_init();
  //-----------------------------------------------------------------------------------------------
#define __LOG(LEVEL) BOOST_LOG_SEV(wapstart::logger::get(), LEVEL)
#define __LOG_EMERG  __LOG(wapstart::LogLevel::EMERG)
#define __LOG_ALERT  __LOG(wapstart::LogLevel::ALERT)
#define __LOG_CRIT   __LOG(wapstart::LogLevel::CRIT)
#define __LOG_ERROR  __LOG(wapstart::LogLevel::ERROR)
#define __LOG_WARN   __LOG(wapstart::LogLevel::WARN)
#define __LOG_NOTICE __LOG(wapstart::LogLevel::NOTICE)
#define __LOG_INFO   __LOG(wapstart::LogLevel::INFO)
#define __LOG_DEBUG  __LOG(wapstart::LogLevel::DEBUG)
//-------------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_LOGGER__H__
//-------------------------------------------------------------------------------------------------

