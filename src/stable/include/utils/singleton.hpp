/** 
 * @file singleton.h
 * @author Litvinova Alina 
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_UTILS_SINGLETON_H__
#define __WAPSTART_SWORDFISH_UTILS_SINGLETON_H__
//-------------------------------------------------------------------------------------------------
#include <stdexcept>
#include <boost/lexical_cast.hpp>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace utils {
    namespace privacy {
      inline std::string _object_deleted_msg(void *address) {
        return std::string("object at ") + 
               boost::lexical_cast<std::string>(address) + 
               " has already deleted!";
      }
      //-------------------------------------------------------------------------------------------
    } // namespace privacy
    //---------------------------------------------------------------------------------------------
    /**
     * Simple template for singleton idiom.
     */
    template<class T, class Mutex> class SingletonHolder {
    public:
      typedef T          base_type; 
      typedef Mutex      mutex_type;
      typedef base_type *base_pointer;
      /**
       *
       */
      static base_type &instance() {
        if(!pinst_) create();
        return *pinst_;
      }
    private:
      static base_pointer pinst_;
      static mutex_type   mutex_;
      static bool         deleted_;
      /**
       *
       */
      static void create() {
        {
          typename mutex_type::scoped_lock guard(mutex_);
          if(!pinst_) {
            if(deleted_) 
              throw std::runtime_error(privacy::_object_deleted_msg(pinst_));
            pinst_ = new base_type();
            std::atexit(clean);
          }
        }
      }
      /**
       *
       */
      static void clean() {
        delete pinst_;
        pinst_   = false;
        deleted_ = true;
      }
    };
    //---------------------------------------------------------------------------------------------
    template<class T, class Mutex> 
    typename SingletonHolder<T, Mutex>::base_pointer
      SingletonHolder<T, Mutex>::pinst_ = NULL; 
    //---------------------------------------------------------------------------------------------
    template<class T, class Mutex>
    typename SingletonHolder<T, Mutex>::mutex_type 
      SingletonHolder<T, Mutex>::mutex_;
    //---------------------------------------------------------------------------------------------
    template<class T, class Mutex>
      bool SingletonHolder<T, Mutex>::deleted_ = false;
    //---------------------------------------------------------------------------------------------
  } // namespace utils
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_UTILS_SINGLETON_H__
//-------------------------------------------------------------------------------------------------


