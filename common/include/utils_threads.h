
#pragma once


#include <iostream>

#include <iostream> 
#include <atomic> 
#include <thread> 
#include <unistd.h> 
#include <chrono>

#include <sys/syscall.h>


inline auto setThreadCore(int core_id)  noexcept -> bool {
        cpu_set_t cpuset ;  

        CPU_ZERO(&cpuset) ;  
  

        CPU_SET(core_id,&cpuset) ;

        return (pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset) == 0 ) ; 
    } 

template<typename T, typename...A> 

inline auto createAndStartThread(int core_id, const std::string& name, T &&func, A && ... args) noexcept -> std::thread*
{
        auto t = new std::thread([&]() {
            if (core_id >= 0 && !setThreadCore(core_id)) {
                std::cerr<< "Failed to set core affinity for "<<name<< " "<<pthread_self()<<" to "<<core_id<<std::endl ; 
                exit(EXIT_FAILURE) ; 
            }

            
        std::cerr<<" Set core affinity for "<<name<< " "<< pthread_self()<<" to "<<core_id<<std::endl ;  


        std::forward<T> (func) ((std::forward<A>(args)) ...) ; 
        }) ;    


        
        std::this_thread::sleep_for(std::chrono::seconds(1)) ; 

        return t ; 
    }


