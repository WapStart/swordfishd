#include "abstract_filler.hpp"
#include <dlfcn.h>
/** 
 * @file cfg.cpp
 * @author Litvinova Alina
 */

#include "filler.h"
#include "boost/pointer_cast.hpp"
#include <stdio.h>
//-------------------------------------------------------------------------------------------------

namespace wapstart {
    
  void AbstractFiller::Shutdown()
  {
    boost::mutex::scoped_lock lock(state_mutex_);
    is_alive_ = false; 
  }
//-------------------------------------------------------------------------------------------------
  AbstractFiller::~AbstractFiller()
  { 
    dlclose(lib_handle_);
  }
//-------------------------------------------------------------------------------------------------
  bool AbstractFiller::Configure(const std::string& libpath,
                                 const std::string& funcname)
  {
    if (libpath.empty() || funcname.empty())
      return configured_;

    // загрузить библиотеку и инициализировать указатель на функцию
    lib_handle_ = dlopen(libpath.c_str(), RTLD_LAZY);
    if (!lib_handle_) 
    {
      printf("Error: [AbstractFiller::Configure] Cannot load lib %s, errno:%s\n", libpath.c_str(), dlerror());
      exit(1);
    }
    get_vals = (get_vals_type)(dlsym(lib_handle_, "get_values_from_outside"));
    char * error;
    if ((error = dlerror()) != NULL)  
    {
      printf("Error: [AbstractFiller::Configure] Cannot load filler func. error: %s\n", error);
      exit(1);
    }
    //dlclose(lib_handle);
    configured_ = true;
  }
//-------------------------------------------------------------------------------------------------
  void AbstractFiller::operator()()
  {
    if(!configured_) 
    {
      printf("Error: [AbstractFiller::operator()()] filler not configured\n");
      exit(1);
    }
    std::string key;
    std::vector<std::string>keys, vals;
    size_t k;
    while(is_alive())
    {
      k = storage_->max_storage_size();
      if (k - storage_->storage_size() == 0)
        storage_->expirate();
      while(storage_->queue_size() != 0  && k - storage_->storage_size()  > 0)
      {
        key = "";
        storage_->pop_key(key);
        if (!key.empty())
          keys.push_back(key);
      }
      if (keys.size() > 0)
      {
        get_vals(keys, vals);
        std::vector<std::string>::iterator key_it, val_it;
        key_it = keys.begin();
        val_it = vals.begin();
        while(key_it != keys.end() && val_it != vals.end())
        {
          storage_->add_item(*key_it++, *val_it++);
        }
        keys.clear();
      }
      else sleep(1);
    }
  }

  bool AbstractFiller::is_alive()
  {
    boost::mutex::scoped_lock lock(state_mutex_);
    return is_alive_;  
  }
} // namespace wapstart

