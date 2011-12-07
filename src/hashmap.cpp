/**
 * @file hashmap.cpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include "hashmap.hpp"
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
//-------------------------------------------------------------------------------------------------

  Hashmap::Hashmap(const ttl_type& ttl)
    : ttl_(ttl), deleted_(0), gets_(0) 
  {
    
  }
//-------------------------------------------------------------------------------------------------

  uint Hashmap::expirate()
  {
    write_scoped_lock lock(mutex_);
    deleted_ = 0;
    hashmap_type::iterator it = map_.begin();
    while (it != map_.end())
    {
      time_type max_time = it->second.second + ttl_;
      if ( max_time < boost::date_time::second_clock<time_type>::local_time() )
      {
        it = map_.erase(it);
        ++deleted_;
      }
      else
        ++it;
    }
    __LOG_DEBUG << "[Hashmap::expirate()] Deleted " << deleted_ << " items";
    return deleted_;
  }
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
  bool Hashmap::get(const key_type& key, val_type& val)
  {
    {
      write_scoped_lock lock(mutex_);
      ++gets_;
    }
    read_scoped_lock lock(mutex_);
    hashmap_type::iterator it = map_.find(key);
    if (it != map_.end())
    {
      val = it->second.first;
      __LOG_DEBUG << "[Hashmap::get] key " << key << " value " << val;
      it->second.second = boost::date_time::second_clock<time_type>::local_time();
      __LOG_DEBUG << "[Hashmap::add] refresh ttl key " << key << " value " << val;

      return true;
    }
    __LOG_DEBUG << "[Hashmap::get] missing get key " << key;
    return false;
  }
//-------------------------------------------------------------------------------------------------

  bool Hashmap::add(const key_type& key, const val_type val)
  {
    write_scoped_lock lock(mutex_);
    std::pair<hashmap_type::iterator, bool> res 
      = map_.insert(item_type(key, 
          hashmap_val_type(val, boost::date_time::second_clock<time_type>::local_time())));
    if (!res.second)
    {
      // такое уже есть, обновить значение и ttl
      res.first->second.first = val;
      res.first->second.second = boost::date_time::second_clock<time_type>::local_time();
      __LOG_DEBUG << "[Hashmap::add] refresh ttl key " << key << " value " << val;
    }
    else
      __LOG_DEBUG << "[Hashmap::add] new item key " << key << " value " << val; 
    return true;
  }
//-------------------------------------------------------------------------------------------------

  uint Hashmap::get_storage_size()
  {
    read_scoped_lock lock(mutex_);
    return map_.size(); 
  }
//-------------------------------------------------------------------------------------------------

  uint Hashmap::get_deleted()
  {
    read_scoped_lock lock(mutex_);
    uint ret = deleted_;
    deleted_ = 0;
    return ret;
  }
//-------------------------------------------------------------------------------------------------

  uint Hashmap::get_gets()
  {
    write_scoped_lock lock(mutex_);
    uint ret = gets_;
    gets_ = 0;
    return ret;
  }
//-------------------------------------------------------------------------------------------------
}// namespace wapstart
