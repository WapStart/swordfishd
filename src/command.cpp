/**
 * @file server.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include "command.hpp"
#include <stdio.h>
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace privacy {
    const char _spacebar = ' ';
    //---------------------------------------------------------------------------------------------
  } // namespace privacy
  //-----------------------------------------------------------------------------------------------
  Command::Command(std::istream &stream)
  {
    parse_command(stream);
  }
  //-----------------------------------------------------------------------------------------------
  void Command::parse_command(std::istream &stream)
  {
    std::string ttt;
    if(std::getline(stream, name_, privacy::_spacebar)) parse_args(stream);
    else name_ = _unknown_command;
    printf("command name %s\n", name_.c_str());
  }
  //-----------------------------------------------------------------------------------------------
  void Command::parse_args(std::istream &stream)
  {
    std::string arg;
    while(std::getline(stream, arg, privacy::_spacebar))
      args_.push_back(arg);
    if (args_.size())
      args_.at(args_.size()-1).erase(args_.at(args_.size()-1).length() - 2, 2); 
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

