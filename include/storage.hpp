/**
 * @file storage.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_STORAGE__H__
#define __WAPSTART_SWORDFISH_STORAGE__H__
//-------------------------------------------------------------------------------------------------
#include "boost/date_time/posix_time/posix_time.hpp"
//-------------------------------------------------------------------------------------------------
#include "hashmap.hpp"
#include "stats.hpp"
#include "queue.hpp"
#include "command.hpp"
//-------------------------------------------------------------------------------------------------
class Command;
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class Storage {
  public:
    typedef Storage class_type;
    typedef boost::posix_time::ptime time_type;
    typedef std::string arg_type;
    typedef std::string result_type;
    typedef std::string key_type;
    typedef std::string val_type;

    typedef Hashmap storage_type;
    typedef Queue   queue_type;
    typedef Stats   stats_type;
    typedef Command cmd_type;
    /**
     *
     */
    Storage(uint ttl = 10, uint max_storage_size = 10, uint max_queue_size = 10);
    /**
     *
     */
    void _do(const cmd_type& command, result_type& result);
    void add_item(const key_type& key, const val_type& val);
    void reset_stats();
    void pop_key(key_type& key);
    void push_key(const key_type& key);
    void expirate();
    void res_append(const arg_type& arg, const val_type& value, result_type& res);
    
    size_t queue_size();
    size_t max_storage_size() {return max_storage_size_;}; 
    size_t storage_size() {return storage_.get_storage_size();};
  private:
    
    Storage(const class_type &);
    void operator =(const class_type &);
    void refresh_stats();

    void get_stats(result_type& res);
    bool get_val(const Command& cmd, result_type& res);


    storage_type storage_;
    queue_type   queue_;
    stats_type   stats_;
    uint         max_storage_size_; // in bytes
    size_t       max_queue_size_;   // in bytes
  }; 
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_STORAGE__H__
//-------------------------------------------------------------------------------------------------

