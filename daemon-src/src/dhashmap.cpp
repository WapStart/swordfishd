/**
 * @file hdashmap.cpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include "dhashmap.hpp"
#include "logger.hpp"
#include "filler.h"

#include <dlfcn.h>

#include <iostream>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
//-------------------------------------------------------------------------------------------------
  DHashmap::__custom_hash_type DHashmap::__custom_hash__      = NULL;
  DHashmap::__normalize_key_type DHashmap::__normalize_key__  = NULL;
  //----------------------------------------------------------------------------------------------- 
  
  DHashmap::DHashmap(const ttl_type& ttl)
    : ttl_(ttl), deleted_(0), gets_(0), updates_(0),
      lib_handle_(0), configured_(false), storage_size(0)
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
  uint DHashmap::expirate(size_t expirate_size)
  {
    write_scoped_lock lock(mutex_);

    hashmap_set_by_ttl::iterator it = keys_.get<hashmap_ttl_tag>().begin();
    hashmap_set_by_ttl::iterator end_it = keys_.get<hashmap_ttl_tag>().end();

    size_t deleted_bytes = 0;

    while ((it != end_it) && (deleted_bytes < expirate_size))
    {
        deleted_bytes += it->key_.length();

        if (it->value_.use_count() == 1)
               deleted_bytes += it->value_->length();

        keys_.get<hashmap_ttl_tag>().erase(it);
        ++deleted_;

        ++it;
    }

    dec_storage_size(deleted_bytes);

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

    hashmap_set_by_key::iterator it = keys_.get<hashmap_key_tag>().find(key);
    hashmap_set_by_key::iterator end_it = keys_.get<hashmap_key_tag>().end();

    if (it != end_it)
    {
/*       set_type::iterator set_it = values_.find();//(set_item_type(*(it->second)));
      
      if ((set_it == values_.end()))
      {
        __LOG_CRIT << "[DHashmap::get] value ref only in keys_!";
        abort();
      }
*/
      val = *it->value_;

      if (boost::date_time::second_clock<time_type>::local_time()
            < ( it->ttl_ + ttl_)
      )
      {
        it->update_ttl();
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

    return false;
  }
//-------------------------------------------------------------------------------------------------

  bool DHashmap::add(const key_type& key, const val_type& val)
  {
    write_scoped_lock lock(mutex_);
    
    set_type::iterator set_it;
    map_item_type new_item(new item_t(val));
    set_type::iterator in_set  = values_.find(new_item);

    if (in_set != values_.end())
    {
      // значит такое уже есть в памяти, создать надо пару ключ-ссылка
      if(map_item_type found = in_set->lock())
      {
        //все ок - объект есть
        // проверяем есть ли такой ключ
        hashmap_set_by_key::iterator it = keys_.get<hashmap_key_tag>().find(key);
        hashmap_set_by_key::iterator end_it = keys_.get<hashmap_key_tag>().end();

        if (it != end_it) {
           if (it->value_ != found) {
             if (it->value_.use_count() == 1) {
               dec_storage_size(it->value_->length());
               values_.erase(it->value_);
             }

             keys_.get<hashmap_key_tag>().erase(it);
             keys_.insert(key_type_struct(key, found));

              __LOG_DEBUG << "[DHashmap::add] Update value for existing key";
           } else {
             __LOG_DEBUG << "[DHashmap::add] No changes";
           }
        } else {
          keys_.insert(key_type_struct(key, found));
          inc_storage_size(key.length());

          __LOG_DEBUG << "[DHashmap::add] Added key for exists value";
        }
      }
      else
      {
        //delete from set
        dec_storage_size(found->length());
        values_.erase(found);

        __LOG_DEBUG << "[DHashmap::add] Delete value from set";
      }
    }
    else
    {
      //значит, надо поискать в ключах, если такой ключик есть,
      // то обновить только значение и ттл
      hashmap_set_by_key::iterator it = keys_.get<hashmap_key_tag>().find(key);
      hashmap_set_by_key::iterator end_it = keys_.get<hashmap_key_tag>().end();
      if (it != end_it)
      {
        __LOG_DEBUG << "[DHashmap::add] Update value and ttl for key " << key;
        it->update_ttl();
        values_.insert(new_item);
        inc_storage_size(new_item->length());
      }
      else
      {
        // иначе надо создать строку в куче, добавить ее в множество, затем в мап
        values_.insert(new_item);
        keys_.insert(key_type_struct(key, new_item));
        inc_storage_size(new_item->length() + key.length());
        __LOG_DEBUG << "[DHashmap::add] Add new key for exits value";
      }
    }

    return true;
  }
//-------------------------------------------------------------------------------------------------
  uint DHashmap::get_storage_size()
  {
    read_scoped_lock lock(mutex_);
    return storage_size;
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
  void DHashmap::inc_storage_size(size_t size)
  {
    storage_size += size;
  }
//-------------------------------------------------------------------------------------------------
  void DHashmap::dec_storage_size(size_t size)
  {
    storage_size -= size;
  }
}// namespace wapstart
