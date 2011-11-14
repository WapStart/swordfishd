/**
 * @file swordfish.h 
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <iostream>
//-------------------------------------------------------------------------------------------------
#include "server.hpp"
//-------------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
  using namespace boost::asio;

  try {
    io_service service;

    wapstart::Storage storage;
    wapstart::Server  server(service, storage, 9696);

    service.run();
  }
  catch(const std::exception &err) {
    std::cout << "FUCK!\n" << std::endl;
  }
  return 0;
}
//-------------------------------------------------------------------------------------------------

