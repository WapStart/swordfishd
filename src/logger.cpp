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
#include <fstream>
//-------------------------------------------------------------------------------------------------
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace privacy {
    typedef boost::log::sinks::synchronous_sink
      <boost::log::sinks::syslog_backend> syslog_sink_type;
    typedef boost::log::sinks::synchronous_sink<
      boost::log::sinks::text_ostream_backend> text_stream_sink_type;
    
    boost::shared_ptr<syslog_sink_type> syslog_sink;
    boost::shared_ptr<text_stream_sink_type> text_stream_sink;
    boost::shared_ptr<boost::log::sinks::syslog_backend>       syslog_backend;
    boost::shared_ptr<boost::log::sinks::text_ostream_backend> text_stream_backend;
    //---------------------------------------------------------------------------------------------
    void logger_init() 
    {
      boost::log::add_common_attributes<char>();
    }
    //---------------------------------------------------------------------------------------------
    bool check_file(const std::string &file) 
    {
      FILE *f = fopen(file.c_str(), "a");
      if(f) {
        fclose(f);
        return true;
      }
      return false;
    }
    //---------------------------------------------------------------------------------------------
  } // namespace privacy
  //-----------------------------------------------------------------------------------------------
  void logger_set_severity_level(LogLevel::type type)
  {
    using namespace boost::log;
    
    boost::shared_ptr<core> c = core::get();

    c->reset_filter();
    
    c->set_filter
    (
      filters::attr<LogLevel::type >("Severity") <= type
    );
  }
  //-----------------------------------------------------------------------------------------------
  bool logger_file_sink_init(const std::string &path)
  {
    using namespace boost::log;
    
    if(!privacy::check_file(path)) {
      return false;
    }
    
    boost::shared_ptr<core> c = core::get();

    if(!privacy::text_stream_backend)
      privacy::text_stream_backend.reset(new sinks::text_ostream_backend());
    
    privacy::text_stream_backend->add_stream(
        boost::shared_ptr<std::ostream>(new std::ofstream(path.c_str(), std::ios_base::app)));

    return true;
  }
  //-----------------------------------------------------------------------------------------------
  void logger_syslog_sink_init()
  {
    using namespace boost::log;
        
    privacy::syslog_backend.reset(
      new sinks::syslog_backend
        (
        keywords::facility = sinks::syslog::user,
        keywords::use_impl = sinks::syslog::native
        )
    );

    privacy::syslog_backend->set_severity_mapper(
        sinks::syslog::direct_severity_mapping<int>("Severity"));

  }
  //-----------------------------------------------------------------------------------------------
  void logger_stdout_sink_init()
  { 
    using namespace boost::log;
    
    boost::shared_ptr<core> c = core::get();

    if(!privacy::text_stream_backend)
      privacy::text_stream_backend.reset(new sinks::text_ostream_backend());
    
    privacy::text_stream_backend->add_stream(
        boost::shared_ptr<std::ostream>(&std::cout, empty_deleter()));
  }
  //-----------------------------------------------------------------------------------------------
  void logger_backends_init_start()
  {
    using namespace boost::log;
    
    boost::shared_ptr<core> c = core::get();
    
    if(privacy::text_stream_sink) {
      c->remove_sink(privacy::text_stream_sink);
      privacy::text_stream_sink.reset();
      privacy::text_stream_backend.reset();
    }

    if(privacy::syslog_sink) {
      c->remove_sink(privacy::syslog_sink);
      privacy::syslog_sink.reset();
      privacy::syslog_backend.reset();
    }
  }
  //-----------------------------------------------------------------------------------------------
  void logger_backends_init_commit()
  {
    using namespace boost::log;
    
    boost::shared_ptr<core> c = core::get();
    
    if(privacy::text_stream_backend) {
      privacy::text_stream_backend->auto_flush(true);

      privacy::text_stream_sink.reset(new 
          privacy::text_stream_sink_type(privacy::text_stream_backend));
      
      privacy::text_stream_sink->locked_backend()->set_formatter(
        formatters::stream << "[" 
                           << formatters::date_time<boost::posix_time::ptime>("TimeStamp")
                           << " "
                           << formatters::attr<LogLevel::type>("Severity")
                           << "] "
                           << formatters::message<char>()
      );

      c->add_sink(privacy::text_stream_sink);
    }

    if(privacy::syslog_backend) {
      privacy::syslog_sink.reset(new 
          privacy::syslog_sink_type(privacy::syslog_backend));
    
      privacy::syslog_sink->locked_backend()->set_formatter(
        formatters::stream << "[" 
                           << formatters::date_time<boost::posix_time::ptime>("TimeStamp")
                           << " "
                           << formatters::attr<LogLevel::type>("Severity")
                           << "] "
                           << formatters::message<char>()
        );

      c->add_sink(privacy::syslog_sink);
    }
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

