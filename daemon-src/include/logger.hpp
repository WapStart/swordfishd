/** 
 * @file logger.hpp
 * @author Alina Litvinova
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_LOGGER__H__
#define __WAPSTART_SWORDFISH_LOGGER__H__
//-------------------------------------------------------------------------------------------------
#include <ostream>
#include <boost/log/detail/config.hpp>
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

  BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(logger, swordfish_logger)
  {
    privacy::logger_init();
    return swordfish_logger();
  }
  //-----------------------------------------------------------------------------------------------
  void logger_set_severity_level(LogLevel::type level);
  //-----------------------------------------------------------------------------------------------
  /**
   * Инициализирует файловый логгер.
   * @param path Путь к файлу журнала.
   */
  bool logger_file_sink_init(const std::string &path);
  //-----------------------------------------------------------------------------------------------
  /**
   *
   */
  void logger_syslog_sink_init();
  //-----------------------------------------------------------------------------------------------
  /**
   *
   */
  void logger_stdout_sink_init();
  //-----------------------------------------------------------------------------------------------
  /**
   *  
   */
  void logger_backends_init_start();
  //-----------------------------------------------------------------------------------------------
  /**
   *
   */
  void logger_backends_init_commit();
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

