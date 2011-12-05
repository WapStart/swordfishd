/**
 * @file logger.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <boost/log/utility/init/common_attributes.hpp>
#include <boost/log/utility/init/to_file.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/formatters/stream.hpp>
#include <boost/log/formatters/attr.hpp>
#include <boost/log/formatters/date_time.hpp>
#include <boost/log/formatters/message.hpp>
#include <boost/log/utility/empty_deleter.hpp>
#include <boost/log/filters.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------------------------------------
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace privacy {
    void logger_init() 
    {
      boost::log::add_common_attributes<char>();
    }
    //---------------------------------------------------------------------------------------------
  } // namespace privacy
  //------------------------------------------------------------------------------------------------
  void set_log_severity_level(LogLevel::type type)
  {
    using namespace boost::log;
    
    boost::shared_ptr<core> c = core::get();

    c->set_filter
    (
      filters::attr<LogLevel::type >("Severity") <= type
    );
  }
  //-----------------------------------------------------------------------------------------------
  void file_logger_init(const std::string &path_base,
                        size_t             rot_size,
                        size_t             rot_frequency)
  {
    using namespace boost::log; 
      
    const std::string name_pattern = path_base + ".%N";
      
    init_log_to_file
    (
      boost::log::keywords::file_name = name_pattern,       
      // Ротируем каждые rot_size байтов
      keywords::rotation_size = 1024 * 1024 * rot_size,
      // Или каждые rot_frequency часов
      keywords::time_based_rotation =sinks::file::rotation_at_time_interval(
        boost::posix_time::time_duration(rot_frequency, 0, 0, 0)),
      keywords::auto_flush = true,
      keywords::format = 
        (
          formatters::stream << "[" 
                             << formatters::date_time<boost::posix_time::ptime>("TimeStamp")
                             << " "
                             << formatters::attr<LogLevel::type>("Severity")
                             << "] "
                             << formatters::message<char>()
        )
    );
  }
  //-----------------------------------------------------------------------------------------------
  void syslog_logger_init()
  {
    using namespace boost::log;

    boost::shared_ptr<core> c = core::get();
        
    boost::shared_ptr<sinks::syslog_backend> backend(
      new sinks::syslog_backend
        (
        keywords::facility = sinks::syslog::user,
        keywords::use_impl = sinks::syslog::native
        )
    );

    backend->set_severity_mapper(sinks::syslog::direct_severity_mapping<int>("Severity"));

    typedef sinks::synchronous_sink<sinks::syslog_backend> sink_t;
    boost::shared_ptr<sink_t> sink = boost::make_shared<sink_t>(backend);
    sink->locked_backend()->set_formatter(
          formatters::stream << "[" 
                             << formatters::attr<LogLevel::type>("Severity")
                             << "] "
                             << formatters::message<char>()
    ); 
    
    c->add_sink(sink);
  }
  //-----------------------------------------------------------------------------------------------
  void stdout_logger_init()
  { 
    using namespace boost::log;
    
    boost::shared_ptr<core> c = core::get();

    boost::shared_ptr<sinks::text_ostream_backend> backend =
      boost::make_shared<sinks::text_ostream_backend>();
    
    backend->add_stream(boost::shared_ptr<std::ostream>(&std::cout, empty_deleter()));

    // Enable auto-flushing after each log record written
    backend->auto_flush(true);

    typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
    boost::shared_ptr<sink_t> sink = boost::make_shared<sink_t>(backend);
    sink->locked_backend()->set_formatter(
      formatters::stream << "[" 
                         << formatters::date_time<boost::posix_time::ptime>("TimeStamp")
                         << " "
                         << formatters::attr<LogLevel::type>("Severity")
                         << "] "
                         << formatters::message<char>()
    );
    
    c->add_sink(sink);
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

