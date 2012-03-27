/**
 * @file dhashmap.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_DHASHMAP__H__
#define __WAPSTART_SWORDFISH_DHASHMAP__H__
//-------------------------------------------------------------------------------------------------
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
//-------------------------------------------------------------------------------------------------
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
//-------------------------------------------------------------------------------------------------
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
//-------------------------------------------------------------------------------------------------
#include <logger.hpp>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class DHashmap {
  
  public:
  
    typedef std::string val_type;
    typedef boost::posix_time::ptime time_type;
    typedef boost::posix_time::time_duration ttl_type;

    struct item
    {
      val_type  value_;
      mutable time_type   ttl_;
      item(const val_type& value):value_(value){;};
      bool operator==(const item& rhs){return value_ == rhs.value_;};
      void update_ttl() const 
      {
        ttl_ = boost::date_time::second_clock<time_type>::local_time();
      }
    };
    typedef struct item item_t;
    typedef boost::weak_ptr<item_t> set_item_type;
    typedef boost::shared_ptr<item_t> map_item_type;
    typedef std::string key_type; 
    /*struct set_item
    {
      //mutable time_type ttl_;
      //boost::shared_ptr<val_type> val_ptr_;
      mutable boost::shared_ptr<item_type> val_ptr_;

      set_item(boost::shared_ptr<item_type> const& ptr)
      {
        val_ptr_ = ptr;
        val_ptr_->ttl_(boost::date_time::second_clock<time_type>::local_time()){;};
      
      set_item(const val_type& val)
        : val_ptr_(new val_type(val)),
          ttl_(boost::date_time::second_clock<time_type>::local_time()){;};
      void update_ttl() const
      {
        ttl_ = boost::date_time::second_clock<time_type>::local_time();
      };
    };

    typedef set_item set_item_type;
    */

    struct dhm_equal_to // double hash map
      : std::binary_function<set_item_type, set_item_type, bool>
    {
      bool operator()( set_item_type const& x,
                       set_item_type const& y) const
      {
        map_item_type pox = x.lock();
        map_item_type poy = y.lock();
        if (pox && poy) 
        {     
          return (pox->value_ == poy->value_); // Вызываем метод объекта
        } 
        else 
        {
          std::cout << "Ошибка, объект уже удалили" << std::endl;
        } 
        return false;;
      }
    };

    struct dhm_hash
      : std::unary_function<set_item_type, std::size_t>
    {
      std::size_t operator()(set_item_type const& x) const
      {
        boost::hash<val_type> hash;
        std::size_t res = 0;
        if (map_item_type po = x.lock())
        {
          if (__custom_hash__)
            __custom_hash__(po->value_, res);
          else
            res = hash(po->value_);
        }
        else
        {
          std::cout << "dhm_hash: fatal error in operator()"<< std::endl;
          abort();
        }
        return res;
      }
    };
 
   struct normalized_hash
      : std::unary_function<key_type, std::size_t>
    {
      std::size_t operator()(key_type const& x) const
      {
        boost::hash<val_type> hash;
        std::size_t res = 0;
        key_type normalized;
        
        if (__normalize_key__)
            __normalize_key__(x, normalized);
        else
        {
          normalized = x;
          __LOG_DEBUG << "[normalized_hash] standard hash for key";
        }
        __LOG_DEBUG << "[normalized_hash] Key: " << normalized; 
        res = hash(normalized);
        
        return res;
      }
    };
 
  
    typedef boost::unordered_set<set_item_type, dhm_hash, dhm_equal_to> set_type; 
  public:
    typedef DHashmap class_type;
    typedef boost::interprocess::interprocess_upgradable_mutex mutex_type;
    //typedef std::string key_type;
    /**
     * string - value
     * val_type - when item added to storage
     */ 
    //typedef std::pair<val_type, time_type>  hashmap_val_type;
    //typedef boost::shared_ptr<val_type> link_type;
    typedef std::pair<key_type, /*link_type*/ map_item_type> item_type;
    typedef boost::unordered_map<key_type, /*link_type*/map_item_type, normalized_hash> hashmap_type;   
    /**
     *
     */
    DHashmap(const ttl_type&);
    /**
     * return count of deleted items
     */
    uint expirate(size_t expirate_size); // deleted += uint
    bool get(const key_type&, val_type&, key_type& normalized_key); // ++gets_
    bool add(const key_type&, const val_type&);

    /**
     * for stats
     */ 
    uint get_storage_size();
    uint get_keys_size();
    uint get_values_size(); 
    uint get_deleted(); // set deleted_ = 0
    uint get_gets();    // set gets_    = 0
    uint get_updates(); // set updates_ = 0
  
    bool configure_func(const std::string&);
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
    
    DHashmap(const class_type &);
    void operator =(const class_type &); 
    /**
     * Hash data
     */  
    ttl_type ttl_;
    hashmap_type  keys_;
    set_type      values_;
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

    bool configured_;

    typedef void (*__custom_hash_type)(const std::string &value, size_t &hash);
    typedef void (*__normalize_key_type)(const std::string &key,   std::string &normalized);
    //static __custom_hash_type __custom_hash__;
    //static __normalize_key_type __normalize_key__;

    void               *lib_handle_;
    //void       (*__custom_hash__)    (const std::string &value, size_t &hash);
    static __custom_hash_type __custom_hash__;
    static __normalize_key_type __normalize_key__;;
  };

  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_STATS__H__
//-------------------------------------------------------------------------------------------------

