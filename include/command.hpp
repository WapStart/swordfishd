/**
 * @file command.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_COMMAND__H__
#define __WAPSTART_SWORDFISH_COMMAND__H__
//-------------------------------------------------------------------------------------------------
#include <vector>
#include <boost/algorithm/string.hpp>
//-------------------------------------------------------------------------------------------------
class Storage;
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  const std::string _unknown_command = "unknown_command";
  /**
   *
   */
  class Command {
  public:
    typedef Command                   class_type;
    typedef std::size_t               size_type;
    typedef std::vector<std::string>  args_type;
    typedef args_type::const_iterator arg_iterator;
    typedef std::string               result_type;
    friend class Storage;
    /**
     * Construct command objectt from input stream
     */
    Command(std::istream &stream);
    /**
     * @return Name of the current command
     */ 
    const std::string &name() const { return name_; }
    /**
     * @return Number of command arguments 
     */
    size_type argc() const { return args_.size(); }
    /**
     *
     */
    const std::string &operator [](size_type x) const { return args_[x]; }
    /**
     *
     */
    const std::string &at(size_type x) const { return args_.at(x); }
    /**
     *
     */
    arg_iterator arg_begin() const { return args_.begin(); }
    /**
     *
     */
    arg_iterator arg_end() const { return args_.end(); }

    const result_type& result() { return res_; }
  
  private:
    std::string name_; /**< Name of command */
    args_type   args_; /**< List of arguments */
    mutable result_type res_;
    /**
     *
     */
    void parse_command(std::istream &stream);
    /**
     *
     */
    void parse_args(std::istream &stream);
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_COMMAND__H__
//-------------------------------------------------------------------------------------------------

