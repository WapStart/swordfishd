/** 
 * @file abstract_filler.cpp
 * @author Litvinova Alina
 */

#include "filler.h"
#include "boost/pointer_cast.hpp"
#include <stdio.h>
#include "logger.hpp"
#include "abstract_filler.hpp"
#include <dlfcn.h>

//-------------------------------------------------------------------------------------------------

namespace wapstart {
    
  void AbstractFiller::Shutdown()
  {
    __LOG_DEBUG << "I'm stopping filler thread...";

    boost::mutex::scoped_lock lock(state_mutex_);
    is_alive_ = false; 
  }
//-------------------------------------------------------------------------------------------------
  AbstractFiller::~AbstractFiller()
  { 
    dlclose(lib_handle_);
  }
//-------------------------------------------------------------------------------------------------
  bool AbstractFiller::Configure(
    const std::string& libpath,
    const std::string& funcname,
    size_t             max_fill_size
  )
  {
    if (libpath.empty() || funcname.empty())
      return configured_;

    // загрузить библиотеку и инициализировать указатель на функцию
    lib_handle_ = dlopen(libpath.c_str(), RTLD_LAZY);
    if (!lib_handle_) 
    {
      //printf("Error: [AbstractFiller::Configure] Cannot load lib %s, errno:%s\n", libpath.c_str(), dlerror());
      __LOG_CRIT << "[AbstractFiller::Configure] Cannot load lib " << libpath << ". " << dlerror(); 
      exit(1);
    }
    get_vals = (get_vals_type)(dlsym(lib_handle_, funcname.c_str()));

	is_filler_alive = (__is_filler_alive_type)(dlsym(lib_handle_, "is_filler_alive"));
	
    char * error;
    if ((error = dlerror()) != NULL)
    {
      //printf("Error: [AbstractFiller::Configure] Cannot load filler func. error: %s\n", error);
      __LOG_CRIT << "[AbstractFiller::Configure] Cannot load filler func. " << error;
      exit(1);
    }
    __LOG_DEBUG << "Filler function loaded";
    max_fill_size_ = max_fill_size ? max_fill_size : 10;
    configured_ = true;
  }
//-------------------------------------------------------------------------------------------------
  void AbstractFiller::operator()()
  {
    if(!configured_) 
    {
      __LOG_CRIT << "[AbstractFiller::operator()()] filler not configured";
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
      size_t t = max_fill_size_;
      while(t-- && is_alive() && storage_->queue_size() != 0  && k - storage_->storage_size()  > 0)
      {
        key = "";
        storage_->pop_key(key);
        if (!key.empty())
          keys.push_back(key);
      }
      
      if (keys.size() > 0 && (!is_filler_alive || is_filler_alive(config_)))
      {
        get_vals(keys, vals, config_);
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
    __LOG_DEBUG << "Filler stopped";
  }

  bool AbstractFiller::is_alive()
  {
    boost::mutex::scoped_lock lock(state_mutex_);
    return is_alive_;  
  }
} // namespace wapstart

