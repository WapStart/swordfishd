#include <stats.hpp>
#include <sstream>
#include <stdio.h>
#include "logger.hpp"
namespace wapstart {
  Stats::Stats()
  {
    reset();
  }
//-------------------------------------------------------------------------------------------------

  bool Stats::get(result_type& result)
  {
    result = "";
    std::stringstream ss;
    read_scoped_lock lock(mutex_);
      
    ss << "STAT " << "uptime "   << boost::date_time::second_clock<time_type>::local_time() - start_time_   << "\r\n";
    ss << "STAT " << "storage_size " << storage_size_ << "\r\n";
    ss << "STAT " << "deleted "       << deleted_      << "\r\n";
    ss << "STAT " << "gets_count "   << gets_         << "\r\n";
    ss << "STAT " << "queue_size "   << queue_size_   << "\r\n";
    ss << "END\r\n";
    
    result = ss.str();
    
    //printf("[Stats::get] result: %s\n", result.c_str());
    __LOG_DEBUG << "[Stats::get] uptime " << boost::date_time::second_clock<time_type>::local_time() - start_time_ 
               << " storage size: " << storage_size_ << " deleted: " << deleted_ << " gets " << gets_ 
               << " queue size: " << queue_size_; 
    return true;
  }

//-------------------------------------------------------------------------------------------------
  bool Stats::set_start_time()
  {
    write_scoped_lock lock(mutex_);
    start_time_ = boost::date_time::second_clock<time_type>::local_time(); 
  }

//-------------------------------------------------------------------------------------------------
  bool Stats::set_storage_size(uint size)
  {
    write_scoped_lock lock(mutex_); 
    storage_size_ = size; 
  }

//-------------------------------------------------------------------------------------------------
  bool Stats::set_deleted(uint count)
  {
    write_scoped_lock lock(mutex_); 
    deleted_ += count;
  }

//-------------------------------------------------------------------------------------------------
  bool Stats::set_queue_size(uint size)
  {
    write_scoped_lock lock(mutex_); 
    queue_size_ = size;
  }

//-------------------------------------------------------------------------------------------------
  bool Stats::set_gets(uint count)
  {
    write_scoped_lock lock(mutex_); 
    gets_ += count;
  }

//-------------------------------------------------------------------------------------------------
  bool Stats::reset()
  {
    write_scoped_lock lock(mutex_); 
    start_time_   = boost::date_time::second_clock<time_type>::local_time();
    storage_size_ = 0;
    deleted_      = 0;
    queue_size_   = 0;
    gets_         = 0;
  }
}// namespace wapstart
