/**
 * @file storage.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "storage.hpp"
#include "boost/lexical_cast.hpp"
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  Storage::Storage(uint ttl, uint max_storage_size, uint max_queue_size)
    : storage_(storage_type::ttl_type(boost::posix_time::seconds(ttl))),
      max_storage_size_(max_storage_size), max_queue_size_(max_queue_size)
      //max_queue_size_(max_queue_size)
  {
  }
  //-----------------------------------------------------------------------------------------------
  void Storage::_do(const cmd_type  &command, result_type& result) 
  {
    /*if (command.arg_begin() != command.arg_end())
      printf("%s : [", command.name().c_str());
    for(Command::arg_iterator x = command.arg_begin(); x != command.arg_end(); ++x) 
      if((x + 1) != command.arg_end()) 
        printf("%s, ", x->c_str());
      else
        printf("%s]\n", x->c_str());
    */
    if (command.name() == "stats")
      get_stats(result);
    else if (command.name() == "get")
      get_val(command, result);
    else if (command.name() == "quit")
      return;
  }
//-------------------------------------------------------------------------------------------------

  void Storage::add_item(const key_type& key, const val_type& val)
  {
    while ( (storage_.get_storage_size_b() + key.length() + val.length())  >= max_storage_size_)
    {
      __LOG_WARN << "[Storage::add_item] storage is full";
      expirate();
      refresh_stats();
      sleep(1);
    }
    storage_.add(key, val); 
    refresh_stats();
  }
//-------------------------------------------------------------------------------------------------

  void Storage::reset_stats()
  {
    __LOG_DEBUG << "[Storage::reset_stats]";
    stats_.set_start_time();
    stats_.set_storage_size(storage_.get_storage_size());
    stats_.set_deleted(0);
    stats_.set_queue_size(queue_.size());
    stats_.set_gets(0);   
  }
//-------------------------------------------------------------------------------------------------

  size_t Storage::queue_size()
  {
    return queue_.size(); 
  }
//-------------------------------------------------------------------------------------------------

  void Storage::pop_key(key_type& key)
  {
    queue_.wait_and_pop(key);  
  }
//-------------------------------------------------------------------------------------------------

  void Storage::push_key(const key_type& key)
  {
    if (queue_.size_b() + key.length() <= max_queue_size_)
      stats_.set_queue_size(queue_.push(key));
    else
      __LOG_WARN << "[Storage::push_key] queue is full";
  }
//-------------------------------------------------------------------------------------------------

  void Storage::expirate()
  {
    stats_.set_deleted(storage_.expirate());
  }
//-------------------------------------------------------------------------------------------------

  void Storage::refresh_stats()
  {
    __LOG_DEBUG << "[Storage::refresh_stats]"; 
    stats_.set_storage_size(storage_.get_storage_size());
    stats_.set_deleted(storage_.get_deleted());
    stats_.set_queue_size(queue_.size());
    stats_.set_gets(storage_.get_gets());   
  }
//-------------------------------------------------------------------------------------------------

  void Storage::get_stats(result_type& res)
  {
    stats_.get(res);  
  }
//-------------------------------------------------------------------------------------------------

  bool Storage::get_val(const Command& cmd, result_type& res)
  {
    res = "";
    for(Command::arg_iterator x = cmd.arg_begin(); x != cmd.arg_end(); ++x) {
      std::string value;
      bool result = storage_.get(*x, value);
      if (!value.empty())
        res_append(*x, value, res);
      if(!result)
       push_key(*x); 
    }
    refresh_stats();
    res += "END\r\n";
    //printf("res: %s", res.c_str());
    return true;
  }
//-----------------------------------------------------------------------------------------------
  
  void Storage::res_append(const arg_type& arg, const val_type& value, result_type& result)
  {
    result.append("VALUE ");
    result.append(arg);
    result.append(" 0 "); // хардкод нулевого флага - пожелание заказчика
    result.append(boost::lexical_cast<result_type>(value.length()));
    result.append("\r\n");
    result.append(value);
    result.append("\r\n"); 
  }
//-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

