/** 
 * @file cfg.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_CFG_H__
#define __WAPSTART_SWORDFISH_CFG_H__
//-------------------------------------------------------------------------------------------------
#include <boost/property_tree/ptree.hpp>
//-------------------------------------------------------------------------------------------------
#include "log_level.hpp"
#include "base_exception.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  /**
   *
   */
  class Config {
  public:
    typedef Config                      class_type;
    typedef LogLevel::type              log_level_type;
    typedef std::basic_istream<char>    stream_type;
    typedef boost::property_tree::ptree ptree_type;

    struct file_not_found: public base_exception {
      typedef boost::errinfo_file_name name;
    };
    
    Config();
    /**
     * @param path Path to a configuration file.
     */
    Config(const std::string &path);
    /**
     * Read a configuration file.
     */ 
    void load(const std::string &path);
    /**
     * Reread the configuration file.
     * @param path Path to a configation file.
     */ 
    void reload();
    /** 
     *
     */
    log_level_type log_level() const;
#ifndef __UNIT_TESTING_ON
  private:
#endif // __UNIT_TESTING_ON
    /**
     *
     */
    void load(stream_type &stream);
  private:
    Config(const class_type &);
    void operator =(const class_type &);
    std::string path_; /**< */
    ptree_type  tree_; /**< */
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_CFG_H__
//-------------------------------------------------------------------------------------------------

