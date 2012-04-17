/** 
 * @file dummy_mutex.h
 * @author Litvinova Alina 
 */ 
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_DUMMY_MUTEX_H__
#define __WAPSTART_SWORDFISH_DUMMY_MUTEX_H__
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace utils {
    class DummyMutex {
    public:
      struct ScopedLock {
        ScopedLock(const DummyMutex &) {;}
      };
      typedef ScopedLock scoped_lock;
    };
    //---------------------------------------------------------------------------------------------
  } // namespace utils
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_DUMMY_MUTEX_H__
//-------------------------------------------------------------------------------------------------

