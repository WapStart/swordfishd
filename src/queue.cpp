/**
* @file queue.cpp
* @author  Litvinova Alina
*/
//-------------------------------------------------------------------------------------------------
#include "queue.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  Queue::Queue()
  {
  
  }

  uint Queue::push(const data_type& data)
  {
    boost::mutex::scoped_lock lock(mutex_);
    queue_.push(data);
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
    while(queue_.empty())
    {
      cv_.wait(lock);
    }
    data = queue_.front();
    queue_.pop();
  }

  uint Queue::size()
  {
    boost::mutex::scoped_lock lock(mutex_);
    return queue_.size();
  }

} // namespace wapstart 
