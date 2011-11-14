/**
 * @file log_level.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_LOG_LEVEL__H__
#define __WAPSTART_SWORDFISH_LOG_LEVEL__H__
//-------------------------------------------------------------------------------------------------
#define __CAST_NAME_TO_LEVEL(X, SHORT_NAME, LONG_NAME) \
  if(X == #SHORT_NAME || X == #LONG_NAME) return SHORT_NAME; 
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  struct LogLevel {
    enum type {
      EMERG = 0,
      ALERT,
      CRIT,
      ERROR,
      WARN,
      NOTICE,
      INFO,
      DEBUG
    };
    /**
     *
     */
    static type text2level(const std::string &text) {
      __CAST_NAME_TO_LEVEL(text, EMERG, EMERGENCY)
      __CAST_NAME_TO_LEVEL(text, ALERT, ALERT)
      __CAST_NAME_TO_LEVEL(text, CRIT, CRITICAL)
      __CAST_NAME_TO_LEVEL(text, ERROR, ERROR)
      __CAST_NAME_TO_LEVEL(text, WARN, WARNING)
      __CAST_NAME_TO_LEVEL(text, NOTICE, NOTICE)
      __CAST_NAME_TO_LEVEL(text, INFO, INFORMATIONAL)
      return DEBUG;
    }
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_LOG_LEVEL__H__
//-------------------------------------------------------------------------------------------------

