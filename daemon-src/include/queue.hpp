/**
 * @file queue.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_QUEUE__H__
#define __WAPSTART_SWORDFISH_QUEUE__H__
//-------------------------------------------------------------------------------------------------
//#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <queue>
//-------------------------------------------------------------------------------------------------

namespace wapstart {
  class Queue {
  public:
    typedef Queue class_type;
    typedef boost::/*interprocess::interprocess_upgradable_*/mutex mutex_type;
    typedef std::string data_type;
    typedef std::list<data_type> queue_type;   
    typedef boost::condition_variable cv_type;
    /**
     *
     */
    Queue(/* TODO configuration */);
    /**
     *
     */
    uint push(const data_type& data);
    bool empty();
    void wait_and_pop(data_type& data);
    uint size();
    uint size_b();
  private:
    Queue(const class_type &);
    void operator =(const class_type &); 
    /**
     * Queue data
     */  
    queue_type queue_;
    size_t     curr_size_;
    /**
     * Synchronization
     */ 
    mutex_type mutex_;
    cv_type cv_;
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_STATS__H__
//-------------------------------------------------------------------------------------------------

