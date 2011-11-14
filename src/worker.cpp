/**
 * @file worker.cpp
 * @author  Litvinova Alina
 */
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
    boost::asio::read_until(socket_, request, "\r\n");
    
    std::istream command_stream(&request);
    
    //storage_._do(Command(command_stream));
   
    std::string message = "END\r\n";

    boost::asio::write(socket_, boost::asio::buffer(message), boost::asio::transfer_all()); 

    _do();
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

