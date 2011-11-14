/**
 * @file worker.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_WORKER__H__
#define __WAPSTART_SWORDFISH_WORKER__H__
//-------------------------------------------------------------------------------------------------
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
//-------------------------------------------------------------------------------------------------
#include "storage.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class Worker: public boost::enable_shared_from_this<Worker> {
  public:
    typedef Worker                        class_type;
    typedef boost::shared_ptr<class_type> pointer_type;
    typedef boost::asio::io_service       service_type;
    typedef boost::asio::ip::tcp::socket  socket_type;
    /**
     *
     */
    static pointer_type create(service_type &service,
                               Storage      &storage);
    /**
     * @return the socket object, it used in accept operation 
     */
    socket_type &socket() { return socket_; } 
    /**
     * 
     */
    void _do();
  private:
    Worker(const class_type &);
    void operator =(const class_type &);
    /**
     *
     */
    Worker(service_type &service,
           Storage      &storage);
    service_type &service_;
    Storage      &storage_;
    socket_type   socket_;
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_WORKER__H__
//-------------------------------------------------------------------------------------------------

