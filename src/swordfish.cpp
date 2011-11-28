/**
 * @file swordfish.h 
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <iostream>
//-------------------------------------------------------------------------------------------------
#include "server.hpp"
#include "abstract_filler.hpp"
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
  using namespace boost::asio;

  try {
    io_service service;

    wapstart::Storage storage;
    wapstart::AbstractFiller filler(&storage);
    filler.Configure();
    wapstart::Server  server(service, storage, 9696);
    boost::thread filler_thread(boost::ref(filler));
    service.run();
  }
  catch(const std::exception &err) {
    std::cout << "FUCK!\n" << std::endl;
  }
  return 0;
}
//-------------------------------------------------------------------------------------------------

