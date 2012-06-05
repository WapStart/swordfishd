/**
* @file queue.cpp
* @author  Litvinova Alina
*/
//-------------------------------------------------------------------------------------------------
#include "queue.hpp"
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
#include <algorithm>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  Queue::Queue()
  {
  
  }

  uint Queue::push(const data_type& data)
  {
    boost::mutex::scoped_lock lock(mutex_);
    if (std::find(queue_.begin(), queue_.end(), data) == queue_.end())
    {
      queue_.push_back(data);
      __LOG_DEBUG << "[Queue::push] add item:" << data;
    }
    else
      __LOG_DEBUG << "[Queue::push] already in queue item: " << data;
    uint size = queue_.size();
    lock.unlock();
    cv_.notify_one(); 
    return size;
  }

  bool Queue::empty()
  {
    boost::mutex::scoped_lock lock(mutex_);
    return queue_.empty();
  }

  void Queue::wait_and_pop(data_type& data)
  {
    boost::mutex::scoped_lock lock(mutex_);
    /*while(queue_.empty())
    {
      cv_.wait(lock);
    }*/
    if (queue_.empty())
    {
      __LOG_DEBUG << "[Queue::wait_and_pop] queue empty";
      data.clear();
      return;
    }
    data = queue_.front();
    queue_.pop_front();
  }

  uint Queue::size()
  {
    boost::mutex::scoped_lock lock(mutex_);
    return queue_.size();
  }

} // namespace wapstart 
