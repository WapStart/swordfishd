#include <stats.hpp>
#include <sstream>
#include <stdio.h>
namespace wapstart {
  Stats::Stats()
  {
    reset();
  }
//-------------------------------------------------------------------------------------------------

  bool Stats::get(const args_type& args, result_type& result)
  {
    result = "";
    std::stringstream ss;
    read_scoped_lock lock(mutex_);
    if (args.size() == 0)
    {
      ss << "STAT " << "start_time "   << start_time_   << "\r\n";
      ss << "STAT " << "storage_size " << storage_size_ << "\r\n";
      ss << "STAT " << "deleted "       << deleted_      << "\r\n";
      ss << "STAT " << "gets_count "   << gets_         << "\r\n";
      ss << "STAT " << "queue_size "   << queue_size_   << "\r\n";
    }
    else
    {
       //обработать STAT с параметрами
    }
    ss << "END\r\n";
    result = ss.str();
    //printf("[Stats::get] result: %s\n", result.c_str());
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
