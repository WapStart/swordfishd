/**
 * @file daemon.hpp
 * @author Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#ifndef __WAPSTART_SWORDFISH_DAEMON__H__
#define __WAPSTART_SWORDFISH_DAEMON__H__
//-------------------------------------------------------------------------------------------------
#include <signal.h>
//-------------------------------------------------------------------------------------------------
#include "server.hpp"
#include "cfg.hpp"
#include "abstract_filler.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  class Daemon {
  public:
    typedef Daemon                  class_type;
    typedef int                     signal_type;
    typedef boost::asio::io_service service_type;
    /**
     *
     */
    Daemon(const std::string &config_path);
    /**
     *
     */
    ~Daemon();
    /**
     *
     */
    void run(bool is_daemonize);

  private:
    Daemon(const class_type &);
    void operator =(const class_type &);
    /**
     *
     */
    Config cfg_;
    /**
     *
     */
    bool done_;
    /**
     * Абстрагирует систему ввода/вывода.
     */
    service_type service_;
    /**
     *
     */
    Storage storage_;
    /**
     *  
     */
    AbstractFiller filler_;
    /**
     *  
     */
    boost::thread *filler_thread_;
    /**
     *
     */
    Server *server_;
    /**
     * Устанавливает обработчики сигналов.
     */
    void set_signal_handlers();
    /**
     *  
     */
    void init_logger();
    /**
     *
     */
    void dispatch_signal_handler(signal_type signal);
    /**
     * 
     */
    void on_exit();
    /**
     *
     */
    void on_config();
    /**
     *  
     */
    void on_expirate();
    /**
     *
     */
    void create_server();
    /**
     *  
     */
    void daemonize();
    /**
     *  
     */
    void reset_server();
    /**
     *
     */
    static void signal_handler(int sig, siginfo_t *siginfo, void *context);
  };
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------
#endif // __WAPSTART_SWORDFISH_DAEMON__H__
//-------------------------------------------------------------------------------------------------

