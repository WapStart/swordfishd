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
    /**
     * Construct command objectt from input stream
     */
    Command(const std::string &cmd_line);
    /**
     * @return Name of the current command
     */ 
    const std::string &name() const;
    /**
     * @return Number of command arguments 
     */
    size_type argc() const;
    /**
     *
     */
    const std::string &operator [](size_type x) const;
    /**
     *
     */
    const std::string &at(size_type x) const;
    /**
     *
     */
    arg_iterator arg_begin() const;
    /**
     *
     */
    arg_iterator arg_end() const;

  private:
    args_type   args_; /**< List of arguments */
    /**
     *
     */
    void parse_command(const std::string &cmd_line);
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_COMMAND__H__
//-------------------------------------------------------------------------------------------------

