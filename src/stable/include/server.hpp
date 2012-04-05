/**
 * @file server.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_SERVER__H__
#define __WAPSTART_SWORDFISH_SERVER__H__
//-------------------------------------------------------------------------------------------------
#include <boost/asio.hpp>
//-------------------------------------------------------------------------------------------------
#include "worker.hpp"
#include "storage.hpp"
#include "abstract_filler.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class Server {
  public:
    typedef Server                           class_type;
    typedef boost::asio::io_service          service_type;
    typedef boost::asio::ip::tcp::socket     socket_type;
    typedef boost::asio::ip::tcp::endpoint   endpoint_type;
    typedef boost::asio::ip::tcp::acceptor   acceptor_type;
    typedef boost::system::error_code        error_code_type;
    typedef int                              port_type;
    typedef std::size_t                      size_type; 
    /**
     * @param storage Reference to the storage object
     * @param port    Port to listen on
     * @param workers Number of workers in the worker pool 
     */
    Server(service_type &service,
           Storage      &storage,
           port_type     port,
           size_type     workers = 10); 
    /**
     *  
     */
    ~Server();  
    /**
     *
     */
    void run();
    /**
     *
     */
    void stop();
  private:
    Server(const class_type &);
    void operator =(const class_type &);
    service_type        &service_;
    Storage             &storage_;
    acceptor_type        acceptor_;
    size_type            workers_;
    Worker::pointer_type worker_;
    /**
     *
     */
    void on_accept(const error_code_type &error);
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_SERVER__H__
//-------------------------------------------------------------------------------------------------

