/**
 * @file storage.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "storage.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  Storage::Storage(/* TODO Configuration */)
    : storage_(storage_type::ttl_type(0,0,10))
  {
  }
  //-----------------------------------------------------------------------------------------------
  void Storage::_do(const cmd_type  &command) 
  {
    if (command.arg_begin() != command.arg_end())
      printf("%s : [", command.name().c_str());
    for(Command::arg_iterator x = command.arg_begin(); x != command.arg_end(); ++x) {
      if((x + 1) != command.arg_end()) 
        printf("%s, ", x->c_str());
      else {
        printf("%s ]\n", x->c_str());
      }
    }
    if (command.name() == "stats")
      get_stats(command.args_, command.res_);
    else if (command.name() == "get")
      get_val(command.args_, command.res_);
    else if (command.name() == "quit")
      return;
  }
//-------------------------------------------------------------------------------------------------

  void Storage::add_item(const key_type& key, const val_type& val)
  {
    storage_.add(key, val); 
  }
//-------------------------------------------------------------------------------------------------

  void Storage::reset_stats()
  {
    stats_.set_start_time();
    stats_.set_storage_size(storage_.get_storage_size());
    stats_.set_deleted(0);
    stats_.set_queue_size(queue_.size());
    stats_.set_gets(0);   
  }
//-------------------------------------------------------------------------------------------------

  void Storage::pop_key(key_type& key)
  {
    queue_.wait_and_pop(key);  
  }
//-------------------------------------------------------------------------------------------------

  void Storage::push_key(const key_type& key)
  {
    stats_.set_queue_size(queue_.push(key));
  }
//-------------------------------------------------------------------------------------------------

  void Storage::expirate()
  {
    stats_.set_deleted(storage_.expirate());
  }
//-------------------------------------------------------------------------------------------------

  void Storage::refresh_stats()
  {
    stats_.set_storage_size(storage_.get_storage_size());
    stats_.set_deleted(storage_.get_deleted());
    stats_.set_queue_size(queue_.size());
    stats_.set_gets(storage_.get_gets());   
  }
//-------------------------------------------------------------------------------------------------

  void Storage::get_stats(const Stats::args_type& args, result_type& res)
  {
    stats_.get(args, res);  
  }
//-------------------------------------------------------------------------------------------------

  bool Storage::get_val(const Command::args_type& args, result_type& res)
  {
    printf("[Storage::get_val] process command (empty)\n");
    res = "";
    /*for(Command::arg_iterator x = args.begin(); x != argsend(); ++x) {
      std::string value;
      if ( storage_.get(*x, value) )
        get_res_add(*x, value, res);
      else
       queue_.push(*x); 
    }*/
    res += "END\r\n";
    return true;
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

