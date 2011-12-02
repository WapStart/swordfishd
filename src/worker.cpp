/**
 * @file worker.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <boost/algorithm/string.hpp>
//-------------------------------------------------------------------------------------------------
#include "worker.hpp"
#include "command.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  Worker::Worker(service_type &service,
                 Storage      &storage): service_(service), 
                                         storage_(storage),
                                         socket_(service)
  {
  }
  //-------------------------------------------------------------------------------------------------
  Worker::pointer_type Worker::create(service_type &service,
                                      Storage      &storage)
  {
    return pointer_type(new Worker(service, storage));
  }
  //-------------------------------------------------------------------------------------------------
  void Worker::_do()
  {
    try{
      boost::asio::streambuf request;
      
      while(true) {  
        boost::asio::read_until(socket_, request, "\r\n");
    
        std::istream command_stream(&request);
      
        std::string command_line;
        while(std::getline(command_stream, command_line)) {
          boost::trim_if(command_line, boost::is_any_of("\r\n")); 
          
          Command cmd(command_line);

          std::string message;
          storage_._do(cmd, message);
          
          if (message.empty()) message = "END\r\n";
          
          boost::asio::write(socket_, boost::asio::buffer(message), boost::asio::transfer_all()); 
        }
      }
    }
    catch(...)
    {
      //printf("[Worker::_do] Exception\n");
    }
    shared_from_this().reset();
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

