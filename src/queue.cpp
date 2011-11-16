/**
* @file queue.cpp
* @author  Litvinova Alina
*/
//-------------------------------------------------------------------------------------------------
#include "queue.hpp"
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
      queue_.push_back(data);
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
    queue_.pop_front();
  }

  uint Queue::size()
  {
    boost::mutex::scoped_lock lock(mutex_);
    return queue_.size();
  }

} // namespace wapstart 
