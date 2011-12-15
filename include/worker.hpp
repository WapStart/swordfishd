/**
 * @file worker.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_WORKER__H__
#define __WAPSTART_SWORDFISH_WORKER__H__
//-------------------------------------------------------------------------------------------------
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
//-------------------------------------------------------------------------------------------------
#include "storage.hpp"
#include "command_tokenizer.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class Worker: public boost::enable_shared_from_this<Worker> {
  public:
    typedef Worker                        class_type;
    typedef boost::shared_ptr<class_type> pointer_type;
    typedef boost::asio::io_service       service_type;
    typedef service_type::strand          strand_type;
    typedef boost::asio::ip::tcp::socket  socket_type;
    typedef boost::array<char, 8192>      array_type;
    typedef std::size_t                   size_type;
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
    void run();

  private:
    Worker(const class_type &);
    void operator =(const class_type &);
    /**
     *
     */
    Worker(service_type &service,
           Storage      &storage);
    /**
     * Reference to the base io_service object.
     */
    service_type    &service_;
    /**
     * Reference to the storage object.
     */
    Storage         &storage_;
    /**
     * Strand to ensure the connection's handlers are not called concurrently.
     */
    strand_type      strand_;
    /**
     * Accepted socket.
     */
    socket_type      socket_;
    /**
     *
     */
    array_type       buffer_;
    /**
     *
     */
    CommandTokenizer tokenizer_;
    /**
     * Handle completion of a read operation.
     */
    void on_read(const boost::system::error_code &error,
                 size_type                        bytes_transfered);
    /**
     * Handle completion of a write operation.
     */
    void on_write(const boost::system::error_code &error);
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_WORKER__H__
//-------------------------------------------------------------------------------------------------

