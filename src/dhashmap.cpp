/**
 * @file hdashmap.cpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include "dhashmap.hpp"
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
//-------------------------------------------------------------------------------------------------

  DHashmap::DHashmap(const ttl_type& ttl)
    : ttl_(ttl), deleted_(0), gets_(0) 
  {
    
  }
//-------------------------------------------------------------------------------------------------

  uint DHashmap::expirate()
  {
    write_scoped_lock lock(mutex_);
    deleted_ = 0;
    hashmap_type::iterator it = keys_.begin();
    while (it != keys_.end())
    {
      time_type max_time = it->second->ttl_ + ttl_;
      if ( max_time < boost::date_time::second_clock<time_type>::local_time() )
      {
        it = keys_.erase(it);
        ++deleted_;
      }
      else
        ++it;
    }
    set_type::iterator set_it = values_.begin();
    while (set_it != values_.end())
    {
      if (!set_it->lock())
      {
        set_it = values_.erase(set_it);
      }
      else
        ++set_it;
    }
    __LOG_DEBUG << "[DHashmap::expirate()] Deleted " << deleted_ << " items";
    return deleted_;
  }
//-------------------------------------------------------------------------------------------------
  bool DHashmap::get(const key_type& key, val_type& val)
  {
    {
      write_scoped_lock lock(mutex_);
      ++gets_;
    }
    read_scoped_lock lock(mutex_);
    hashmap_type::iterator it = keys_.find(key);
    if (it != keys_.end())
    {
      val = it->second->value_;
    
      //?!
      set_item_type po = it->second; 
      set_type::iterator set_it = values_.find(po);//(set_item_type(*(it->second)));
      
      if (set_it == values_.end())
      {
        __LOG_CRIT << "[DHashmap::get] value ref only in keys_!";
        abort();
      }
      //?!
      it->second->update_ttl();
      __LOG_DEBUG << "[DHashmap::get] key " << key << " value " << val;
      __LOG_DEBUG << "[DHashmap::add] refresh ttl key " << key << " value " << val;

      return true;
    }
    __LOG_DEBUG << "[Hashmap::get] missing get key " << key;
    return false;
  }
//-------------------------------------------------------------------------------------------------

  bool DHashmap::add(const key_type& key, const val_type& val)
  {
    write_scoped_lock lock(mutex_);
    
    set_type::iterator set_it;
    boost::shared_ptr<item_t> new_item(new item_t(val)); 
    set_type::iterator in_set  = values_.find(new_item);
    if (in_set != values_.end())
    {
      // значит такое уже есть в памяти, создать надо пару ключ-ссылка
      if(map_item_type found = in_set->lock())
      {
        //все ок - объект есть, проапдейтить время, добавить ссылку в мапе
        found->update_ttl();
        keys_.insert(item_type(key, found));
      }
      else
      {
        //delete from set
        values_.erase(found);
      }
    }
    else
    {
      // значит надо создать строку в куче, добавить ее в множество, затем в мап
      new_item->update_ttl();
      values_.insert(new_item);
      keys_.insert(item_type(key, new_item));
    }
    /*set_it = val_res.first;
    if (!val_res.second)
      set_it->update_ttl();
    
    std::pair<hashmap_type::iterator, bool> res 
      = keys_.insert(item_type(key, set_it->val_ptr_));
    if (!val_res.second)
    {
      __LOG_DEBUG << "key already in storage"; 
    }
    __LOG_DEBUG << "[DHashmap::add] new item key " << key << " value " << val; 
    */
    return true;
  }
//-------------------------------------------------------------------------------------------------

  uint DHashmap::get_storage_size()
  {
    read_scoped_lock lock(mutex_);
    return keys_.size(); 
  }
//-------------------------------------------------------------------------------------------------
  uint DHashmap::get_values_size()
  {
    read_scoped_lock lock(mutex_);
    return values_.size();
  }
//-------------------------------------------------------------------------------------------------

  uint DHashmap::get_deleted()
  {
    read_scoped_lock lock(mutex_);
    uint ret = deleted_;
    deleted_ = 0;
    return ret;
  }
//-------------------------------------------------------------------------------------------------

  uint DHashmap::get_gets()
  {
    write_scoped_lock lock(mutex_);
    uint ret = gets_;
    gets_ = 0;
    return ret;
  }
//-------------------------------------------------------------------------------------------------
}// namespace wapstart
