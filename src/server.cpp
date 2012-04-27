/**
 * @file server.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
//-------------------------------------------------------------------------------------------------
#include "server.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  Server::Server(service_type &service,
                 Storage      &storage,
                 port_type     port, 
                 size_type     workers): service_(service),
                                         storage_(storage),
                                         workers_(workers),
                                         worker_(Worker::create(service_, storage)),
                                         acceptor_(service_, endpoint_type(
                                           boost::asio::ip::tcp::v4(), port))
  {
    acceptor_.async_accept(worker_->socket(), boost::bind(
      &Server::on_accept, this, boost::asio::placeholders::error)); 
  }
  //-----------------------------------------------------------------------------------------------
  Server::~Server()
  {
  }
  //-----------------------------------------------------------------------------------------------
  void Server::on_accept(const error_code_type &error) 
  {
    if(!error) {
      worker_->run();
      worker_ = Worker::create(service_, storage_);
    } else {
	//look like error
	//do nothing
    }
      acceptor_.async_accept(worker_->socket(), boost::bind(
        &Server::on_accept, this, boost::asio::placeholders::error));
  }
  //-----------------------------------------------------------------------------------------------
  void Server::run()
  {
    typedef boost::ptr_vector<boost::thread> thread_vector;
    
    service_.reset();
    
    thread_vector tv;
    for(size_type x = 0; x < workers_; ++x)
      tv.push_back(new boost::thread(
        boost::bind(&service_type::run, &service_)));

     for(size_type x = 0; x < tv.size(); ++x)
       tv[x].join();
  }
  //-----------------------------------------------------------------------------------------------
  void Server::stop()
  {
    service_.stop();
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

