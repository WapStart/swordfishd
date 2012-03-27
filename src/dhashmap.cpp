/**
 * @file hdashmap.cpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include "dhashmap.hpp"
#include "logger.hpp"
#include "filler.h"

#include <dlfcn.h> 
//-------------------------------------------------------------------------------------------------
namespace wapstart {
//-------------------------------------------------------------------------------------------------
  DHashmap::__custom_hash_type DHashmap::__custom_hash__      = NULL;
  DHashmap::__normalize_key_type DHashmap::__normalize_key__  = NULL;
  //----------------------------------------------------------------------------------------------- 
  //__custom_hash_type __normalize_key__ = NULL;
  //__normalize_key_type __normalize_key__ = NULL;
  //DHashmap::__custom_hash__ = NULL;
  //DHashmap::__normalize_key = NULL;
  
  DHashmap::DHashmap(const ttl_type& ttl)
    : ttl_(ttl), deleted_(0), gets_(0), updates_(0),
      lib_handle_(0), configured_(false)
      //__custom_hash(0), __normalize_key(0) 
  {
    
  }
//-------------------------------------------------------------------------------------------------
  bool DHashmap::configure_func(const std::string& libpath)
  {
    if (libpath.empty())
    {
      __LOG_INFO << "Configured storage with default hash and empty notmalize funcs";
      return configured_;
    }
    // загрузить библиотеку и инициализировать указатель на функцию
    
    if (!lib_handle_)
      lib_handle_ = dlopen(libpath.c_str(), RTLD_LAZY);
    if (!lib_handle_) 
    {
      __LOG_CRIT << "[DHashmap::Configure_func] Cannot load lib " << libpath << ". " << dlerror(); 
      exit(1);
    }

    if (!__custom_hash__)
      __custom_hash__  = (__custom_hash_type)(dlsym(lib_handle_, "custom_hash"));
    
    if (!__normalize_key__)
      __normalize_key__ = (__normalize_key_type)(dlsym(lib_handle_, "normalize_key"));
    
    char * error;
    if ((error = dlerror()) != NULL || !__normalize_key__ || !__custom_hash__)  
    {
      __LOG_CRIT << "[DHashmap::configure_func] Cannot load custom  func. " << error;
      exit(1);
    }
    __LOG_DEBUG << "Filler function loaded";
    configured_ = true;
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
  bool DHashmap::get(const key_type& _key, val_type& val, key_type& normalized_key)
  {
    key_type key;
    if (__normalize_key__)
      __normalize_key__(_key, key);
    else
      key = _key;
    
    normalized_key = key;
    
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
      
      if ((set_it == values_.end()))
      {
        __LOG_CRIT << "[DHashmap::get] value ref only in keys_!";
        abort();
      }

      if (boost::date_time::second_clock<time_type>::local_time()
            < ( it->second->ttl_ + ttl_)
      )
      {
        //?!
        it->second->update_ttl();
        __LOG_DEBUG << "[DHashmap::get] key " << key << " value " << val;
        __LOG_DEBUG << "[DHashmap::get] refresh ttl key " << key << " value " << val;

        return true;
      } else {
        __LOG_DEBUG << "[DHashmap::get] key " << key << " value " << val;
        __LOG_DEBUG << "[DHashmap::get] update key " << key;

        {
          write_scoped_lock lock(mutex_);
          ++updates_;
        }

        return false;
      }
    }
    __LOG_DEBUG << "[DHashmap::get] missing get key " << key;
    //expirate();
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
        __LOG_DEBUG << "[DHashmap::add] Added key for exists value";
      }
      else
      {
        //delete from set
        values_.erase(found);
      }
    }
    else
    {
      //значит, надо поискать в ключах, если такой ключик есть,
      // то обновить только значение и ттл
      hashmap_type::iterator it = keys_.find(key);
      if (it != keys_.end())
      {
        __LOG_DEBUG << "[DHashmap::add] Update value and ttl for key " << key;
        new_item->value_ = val;
        new_item->update_ttl();
        values_.insert(new_item);
        it->second = new_item;
      }
      else
      {
        // иначе надо создать строку в куче, добавить ее в множество, затем в мап
        new_item->update_ttl();
        values_.insert(new_item);
        keys_.insert(item_type(key, new_item));
        __LOG_DEBUG << "[DHashmap::add] Add new key for exits value";
      }
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
    return keys_.size() + values_.size();
  }
//-------------------------------------------------------------------------------------------------
  uint DHashmap::get_keys_size()
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

  uint DHashmap::get_updates()
  {
    write_scoped_lock lock(mutex_);
    uint ret = updates_;
    updates_ = 0;
    return ret;
  }
//-------------------------------------------------------------------------------------------------
}// namespace wapstart
