/**
 * @file daemon.cpp
 * @author  Litvinova Alina
 */
//-------------------------------------------------------------------------------------------------
#include <iostream>
#include "sys/stat.h"

#include "daemon.hpp"
#include "logger.hpp"
//-------------------------------------------------------------------------------------------------
namespace wapstart {
  namespace ugly {
    Daemon *__this = NULL;
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::signal_handler(int sig, siginfo_t *siginfo, void *context)
  {
    if(ugly::__this) ugly::__this->dispatch_signal_handler(sig);
  }
  //-----------------------------------------------------------------------------------------------
  Daemon::Daemon(const std::string &config_path): 
    cfg_(config_path), 
    done_(false), 
    server_(NULL),
    storage_(NULL),
    filler_(NULL),
    filler_thread_(NULL)
  {
    ugly::__this = this;
  }
  //-----------------------------------------------------------------------------------------------
  Daemon::~Daemon()
  {
    reset_server();
    __LOG_INFO << "Goodbye!...";
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::run(bool is_daemonize)
  {
    init_logger();
    
    try {

      if(is_daemonize) daemonize();

      // Счетчик перезапусков
      std::size_t rn = 0;

      while(!done_) {
        if(rn++) init_logger();
        
        create_server();
      
        set_signal_handlers();
        
        __LOG_INFO << "I have started " << rn << " time...";
        __LOG_INFO << "I'm waiting for the events...";
      
        server_->run();

        __LOG_INFO << "Event loop has stopped...";
      }
    }
    catch(const std::exception &x) {
      __LOG_CRIT << "Exceptions! " << x.what();
    }
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::init_logger()
  {
    bool fl = false;
    
    logger_backends_init_start();
    
    if(cfg_.is_log_syslog()) {
      logger_syslog_sink_init();
    }

    if(cfg_.is_log_stdout()) {
      logger_stdout_sink_init();
    }
    
    if(cfg_.is_log_file()) {
      fl = logger_file_sink_init(cfg_.log_file_path());
    }
    
    logger_set_severity_level(cfg_.log_level());

    logger_backends_init_commit();

    if(cfg_.is_log_file() && !fl)
      __LOG_ERROR << "Failed to open '" << cfg_.log_file_path() << "' for logging...";
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::set_signal_handlers() 
  {
    struct sigaction act;
     
    memset (&act, '\0', sizeof(act));
       
    act.sa_sigaction = &signal_handler;
           
    act.sa_flags = SA_SIGINFO;
               
    if (sigaction(SIGTERM, &act, NULL) < 0) {
      throw std::runtime_error("sigaction fuck!");
    }
    if (sigaction(SIGINT, &act, NULL) < 0) {
      throw std::runtime_error("sigaction fuck!");
    }
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
      throw std::runtime_error("sigaction fuck!");
    }
    if (sigaction(SIGUSR2, &act, NULL) < 0) {
      throw std::runtime_error("sigaction fuck!");
    }
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::dispatch_signal_handler(signal_type signal) 
  {
    if(signal == SIGTERM || signal == SIGINT)
      service_.dispatch(boost::bind(&Daemon::on_exit, this));
    else if(signal == SIGUSR1)
      service_.dispatch(boost::bind(&Daemon::on_config, this));
    else if(signal == SIGUSR2)
      service_.dispatch(boost::bind(&Daemon::on_expirate, this));
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::daemonize()
  {
    __LOG_DEBUG << "I'm demonizing...";
    
    if(pid_t pid = fork()) {
      if(pid > 0) {
        exit(0);
      }
      else {
        __LOG_EMERG << "Failed to first fork!";
        exit(1);
      }
    }

    __LOG_DEBUG << "First fork ok..."; 

    __LOG_DEBUG << "setsid()...";
    
    setsid();
    
    __LOG_DEBUG << "chdir(\"/\")...";

    chdir("/");
    
    __LOG_DEBUG << "umask(0)...";
    
    umask(0);

    if(pid_t pid = fork()) {
      if(pid > 0) {
        exit(0);
      }
      else {
        __LOG_EMERG << "Failed to second fork!";
        exit(1);
      }
    }

    __LOG_DEBUG << "Second fork ok...";

    __LOG_DEBUG << "Closing std io descriptors...";
    
    close(0);
    close(1);
    close(2);
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::create_server()
  {
    reset_server();

    __LOG_DEBUG << "I'm creating the storage...";
    storage_ = new Storage(
      cfg_.storage_ttl(),
      cfg_.storage_size(),
      cfg_.filler_queue_size(),
      cfg_.storage_expirate_size()
    );
    storage_->Configure(cfg_.filler());

    __LOG_DEBUG << "I'm creating the filler...";
    filler_ = new AbstractFiller(storage_, cfg_.get_config());
      
    __LOG_DEBUG << "I'm configuring the filler...";
    filler_->Configure(cfg_.filler(), cfg_.filler_function(), cfg_.max_fill_size());

    __LOG_DEBUG << "I'm creating the filler thread...";
    // Создаем поток наполнятора
    filler_thread_ = new boost::thread(boost::ref(*filler_));
    
    __LOG_DEBUG << "I'm creating the server...";
    // Конструируем новый сервер
    server_ = new Server(service_, *storage_, cfg_.port(), cfg_.workers());
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::reset_server()
  {
    if(filler_thread_) {
      __LOG_DEBUG << "I'm stopping the filler thread...";
      // Останавливаем поток наполнятора
      filler_->Shutdown();
      filler_thread_->join();
      delete filler_thread_;
    }
    // Убиваем существующий сервер
    // Все должно корректно остановиться в деструкторах
    if(server_) {
      __LOG_DEBUG << "I'm stopping the server...";
      server_->stop();
      delete server_;
    }

    if(filler_) {
      __LOG_DEBUG << "I'm deleting the filler...";
      delete filler_;
    }

    if(storage_) {
      __LOG_DEBUG << "I'm deleting the storage...";
      delete storage_;
    }
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::on_exit()
  {
    __LOG_INFO << "I'm stopping...";
    
    done_ = true;
    
    server_->stop();
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::on_config()
  {
    __LOG_INFO << "I'm restarting...";
    
    cfg_.reload();
    
    server_->stop();
  }
  //-----------------------------------------------------------------------------------------------
  void Daemon::on_expirate()
  {
    __LOG_INFO << "I'm expirating the storage...";
    
    storage_->expirate();
  }
  //-----------------------------------------------------------------------------------------------
} // namespace wapstart
//-------------------------------------------------------------------------------------------------

