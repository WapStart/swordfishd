/**
 * @file hashmap.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_HASHMAP__H__
#define __WAPSTART_SWORDFISH_HASHMAP__H__
//-------------------------------------------------------------------------------------------------
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/unordered_map.hpp"
//-------------------------------------------------------------------------------------------------
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
//-------------------------------------------------------------------------------------------------

namespace wapstart {
  class Hashmap {
  public:
    typedef Hashmap class_type;
    typedef boost::posix_time::ptime time_type;
    typedef boost::posix_time::time_duration ttl_type;
    typedef boost::interprocess::interprocess_upgradable_mutex mutex_type;
    typedef std::string key_type;
    /**
     * string - value
     * val_type - when item added to storage
     */ 
    typedef std::string val_type;
    typedef std::pair<val_type, time_type>  hashmap_val_type;
    typedef std::pair<key_type, hashmap_val_type> item_type;
    typedef boost::unordered_map<key_type, hashmap_val_type> hashmap_type;   
    /**
     *
     */
    Hashmap(const ttl_type&/* TODO configuration */);
    /**
     * return count of deleted items
     */
    uint expirate(); // deleted += uint
    bool get(const key_type&, val_type&); // ++gets_
    bool add(const key_type&, const val_type);

    /**
     * for stats
     */ 
    uint   get_storage_size(); 
    size_t get_storage_size_b();
    uint   get_deleted(); // set deleted_ = 0
    uint   get_gets();    // set gets_    = 0
    uint   get_updates(); // set updates_ = 0
  private:
    //-----------------------------------Mutexes-------------------------------------------
    //-------------------------------Read_scoped_lock--------------------------------------
    class read_scoped_lock
    {
      private:
        mutex_type& rwlock;
      public:
        read_scoped_lock(mutex_type& lock)
          :rwlock(lock)
        { 
          rwlock.lock_sharable();
          //printf("reader locked\n"); 
        }   
        ~read_scoped_lock()
        {   
          rwlock.unlock_sharable();
          //printf("reader unlocked\n");
        } 
    };  
    //-------------------------------Write_scoped_lock-------------------------------------
    class write_scoped_lock
    {
      private:
        mutex_type& rwlock;
      public:
        write_scoped_lock(mutex_type& lock)
          :rwlock(lock)
        {                              
          rwlock.lock_upgradable();
          //printf("writer locked\n");
        }   
        ~write_scoped_lock()
        {   
          rwlock.unlock_upgradable();
          //printf("writer unlocked\n");
        }   
    };
    //---------------------------------end of Mutexes--------------------------------------
    
    Hashmap(const class_type &);
    void operator =(const class_type &); 
    /**
     * Hash data
     */  
    ttl_type      ttl_;
    hashmap_type  map_;
    size_t        curr_storage_size_;
    /**
     * Synchronization
     */ 
    mutex_type mutex_;
    /**
     * Stats data
     */ 
    uint deleted_;
    uint gets_;
    uint updates_;
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_STATS__H__
//-------------------------------------------------------------------------------------------------

