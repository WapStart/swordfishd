/** 
 * @file abstract_filler.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_ABSTRACT_FILLER_H__
#define __WAPSTART_SWORDFISH_ABSTRACT_FILLER_H__
//-------------------------------------------------------------------------------------------------
#include "storage.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
//-------------------------------------------------------------------------------------------------
namespace wapstart {

  /**
   *
   */
  class AbstractFiller {
  public:
    typedef AbstractFiller              class_type;
    typedef Storage                     storage_type;
    typedef boost::mutex                mutex_type;
    typedef bool (*get_vals_type)(const std::vector<std::string>&, std::vector<std::string>&);
    AbstractFiller(storage_type *storage,
                   const std::string  &libpath = "/home/alina/fill_func/libfiller.so",
                   const std::string  &func_name = "get_values")
      : storage_(storage),
        is_alive_(true),
        configured_(false){};
    ~AbstractFiller();
    /**
     * @param storage.
     */
    void operator()();
    void Shutdowh(class_type& self);
    bool Configure(const std::string  &libpath = "/home/alina/sf_repo/trunk/misc/fill_func/libfiller.so",
                   const std::string  &func_name = "get_values");
#ifndef __UNIT_TESTING_ON
  private:
#endif // __UNIT_TESTING_ON
  private:
    AbstractFiller(const class_type &);
    AbstractFiller();
    void operator =(const class_type &);
    
    Storage::val_type Get_val(const Storage::key_type& key);    //= 0;

    //bool (*get_vals)(const std::vector<std::string>& keys, std::vector<std::string>& vals);
    get_vals_type get_vals; 
    std::string (*get_val)(std::string key);
    bool is_alive();
  
    storage_type *storage_;
    mutex_type          state_mutex_;
    bool                is_alive_;
    bool                configured_;
    void               *lib_handle_;
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_ABSTRACT_FILLER_H__
//-------------------------------------------------------------------------------------------------

