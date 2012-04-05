/**
 * @file swordfish.h 
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <iostream>
#include <getopt.h>
//-------------------------------------------------------------------------------------------------
#include "daemon.hpp"
//-------------------------------------------------------------------------------------------------
struct globalArgs_t
{
  std::string filename;
  bool        daemon;
} globalArgs;

static const char *optString = "dc:h?";
static const struct option longOpts[] = 
{
  { "daemon"  , no_argument       , NULL, 'd' },
  { "config"  , required_argument , NULL, 'c' },
  { "help"    , no_argument       , NULL, 'h' },
  { NULL      , no_argument       , NULL,  0  } 
};

void display_usage( void )
{
  std::cout << "Swordfish" << std::endl
    << "Обязательный параметр:" <<std::endl
    << "\t--config (-c) <путь_к_конфигурационному_файлу>" << std::endl
    << "Необязательный параметр:" << std::endl
    << "\t--daemon (-d) запускать как демона" << std::endl;
}

void init_global_args()  
{
  globalArgs.daemon = false;
}

void process_args(int argc, char**argv)
{
  init_global_args();
  int opt = 0;
  int longIndex = 0;
  opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  while( opt != -1 ) 
  {
    switch( opt ) 
    {
      case 'd':
       globalArgs.daemon = true;
       break;
      
      case 'c':
       globalArgs.filename = optarg;
       if (globalArgs.filename.empty())
       {
         display_usage();
         exit(0);
       }
       break;
      
      case 'h':
      case '?':
      default:
       display_usage();
       exit(0);  
       break;
    }
    opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  }
}

int main(int argc, char **argv) 
{
  process_args(argc, argv);  

  if (globalArgs.filename.empty())
  {
    display_usage();
    exit(0);
  }
  
  using namespace boost::asio;

  try {
    wapstart::Daemon daemon(globalArgs.filename);
    
    daemon.run(globalArgs.daemon);
  }
  catch(const std::exception &x) {
  }
  
  return 0;
}
//-------------------------------------------------------------------------------------------------

