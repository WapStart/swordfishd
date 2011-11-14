/**
 * @file stats.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_STATS__H__
#define __WAPSTART_SWORDFISH_STATS__H__
//-------------------------------------------------------------------------------------------------
#include "boost/date_time/posix_time/posix_time.hpp"
//-------------------------------------------------------------------------------------------------
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
//-------------------------------------------------------------------------------------------------

namespace wapstart {
  class Stats {
  public:
    typedef Stats class_type;
    typedef boost::posix_time::ptime time_type;
    typedef boost::interprocess::interprocess_upgradable_mutex mutex_type;
    typedef std::string arg_type;
    typedef std::vector<arg_type> args_type;   
    typedef std::string result_type;
    /**
     *
     */
    Stats(/* TODO configuration */);
    /**
     *
     */
    bool get(const args_type& args, result_type& result);
    bool set_start_time();
    bool set_storage_size(uint size);
    bool set_deleted(uint count); // deleted_ += count
    bool set_queue_size(uint size);
    bool set_gets(uint count); // gets_ += count

    bool reset();
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
    
    Stats(const class_type &);
    void operator =(const class_type &); 
    /**
     * Stats data
     */  
    time_type start_time_;
    uint      storage_size_;
    uint      deleted_;
    uint      queue_size_;
    uint      gets_;

    /**
     * Synchronization
     */ 
    mutex_type mutex_;
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_STATS__H__
//-------------------------------------------------------------------------------------------------

