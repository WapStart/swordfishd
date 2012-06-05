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
#include "boost/multi_index_container.hpp"
#include "boost/multi_index/hashed_index.hpp"
#include "boost/multi_index/ordered_index.hpp"
#include "boost/multi_index/member.hpp"
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

	typedef std::string key_type;
    typedef std::string item_t;
    typedef boost::weak_ptr<item_t> set_item_type;
    typedef boost::shared_ptr<item_t> map_item_type;

    typedef struct key_type_struct {
      key_type key_;
      mutable time_type ttl_;
      map_item_type value_;

      key_type_struct(const key_type &key, const map_item_type &value)
        : key_(key), value_(value)
      {
        update_ttl();
      }

      void update_ttl() const
      {
        ttl_ = boost::date_time::second_clock<time_type>::local_time();
      }
    } KeyType;

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
          return (*pox == *poy);
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
            __custom_hash__(*po, res);
          else
            res = hash(*po);
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

    struct hashmap_ttl_tag {};
    struct hashmap_key_tag {};

    typedef boost::multi_index::multi_index_container<
        KeyType,
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<hashmap_ttl_tag>,
                boost::multi_index::member<KeyType,time_type,&KeyType::ttl_>
            >,
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<hashmap_key_tag>,
                boost::multi_index::member<KeyType,std::string,&KeyType::key_>,
                normalized_hash
            >
        >
    > hashmap_type;

    typedef hashmap_type::index<hashmap_ttl_tag>::type hashmap_set_by_ttl;
    typedef hashmap_type::index<hashmap_key_tag>::type hashmap_set_by_key;

  public:
    typedef DHashmap class_type;
    typedef boost::interprocess::interprocess_upgradable_mutex mutex_type;

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

    size_t storage_size;

    typedef void (*__custom_hash_type)(const std::string &value, size_t &hash);
    typedef void (*__normalize_key_type)(const std::string &key,   std::string &normalized);
    //static __custom_hash_type __custom_hash__;
    //static __normalize_key_type __normalize_key__;

    void               *lib_handle_;
    //void       (*__custom_hash__)    (const std::string &value, size_t &hash);
    static __custom_hash_type __custom_hash__;
    static __normalize_key_type __normalize_key__;

    void inc_storage_size(size_t size);
    void dec_storage_size(size_t size);
  };

  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_STATS__H__
//-------------------------------------------------------------------------------------------------

