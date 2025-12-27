#pragma once 

#include <string> 
#include <fstream> 
#include <cstdio> 

#include "macros.h" 
#include "lock_free_queue.h" 
#include "utils_times.h"
#include "utils_threads.h"

constexpr size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024 ; 

enum class LogType : int8_t {
    CHAR = 0,
    INTEGER = 1,
    LONG_INTEGER = 2,
    LONG_LONG_INTEGER = 3,
    UNSIGNED_INTEGER = 4,
    UNSIGNED_LONG_INTEGER = 5,
    UNSIGNED_LONG_LONG_INTEGER = 6,
    FLOAT = 7,
    DOUBLE = 8
};

struct LogElement {
    LogType type_ = LogType::CHAR ;  
    union {
        char c ; 
        int i ; 
        long l ; 
        long long ll ; 
        unsigned u ; 
        unsigned long ul ;  
        unsigned long long ull ; 
        float f ; 
        double d; 
    } u_ ;  
} ; 

class Logger final 
{
    public : 
        Logger(std::string file_name) ; 
        ~Logger() ; 
        void flushQueue() noexcept ; 
        void pushValue(const LogElement& log_elem)  noexcept
        {
            *(queue_.getNextToWriteTo()) = log_elem ;  
            queue_.updatWriteIndex() ;
        }

        void pushValue(const char c) 
        {
            pushValue(LogElement{LogType::CHAR,{.c=c}}) ; 
        }

        void pushValue(const char* c_string) 
        {
            while (*c_string)
            {
                pushValue(c_string) ; 
                c_string++ ; 
            }
        }

        void pushValue(const std::string & str) 
        {
            pushValue(str.c_str()) ;  
        }

        void pushValue(const int i) 
        {
            pushValue(LogElement{LogType::INTEGER,{.i=i}}) ; 
        }
        void puhsValue(const long l ) 
        {
            pushValue(LogElement{LogType::LONG_INTEGER,{.l=l}}) ; 
        }
        void pushValue(const long long ll ) 
        {
            pushValue(LogElement{LogType::LONG_LONG_INTEGER,{.ll=ll}}) ; 
        }
        void pushValue(const unsigned u) 
        {
            pushValue(LogElement{LogType::UNSIGNED_INTEGER,{.u=u}}) ; 
        }
        void pushValue(const unsigned long ul) 
        {
            pushValue(LogElement{LogType::UNSIGNED_LONG_INTEGER,{.ul=ul}}) ; 
        }  
        void pushValue(const unsigned long long ull) 
        {
            pushValue(LogElement{LogType::UNSIGNED_LONG_LONG_INTEGER,{.ull=ull}}) ; 
        }
        template<typename T, typename... A > 
        auto log(const char* s, const T& value, A ... args) 
        {
            while(*s) 
            {
                if (*s == '%') 
                {
                    if (UNLIKELY(*(s+1) == '%'))
                    {
                        s++ ; 
                    }
                    else 
                    {
                        pushValue(value) ; 
                        log(s+1, args...) ; 
                        return ; 
                    }
                }
                pushValue(*s++) ; 
            }
            FATAL("extra argument provided for log") ; 
        }

        auto log(const char* s) 
        {
            while (*s) 
            {
                if (*s == '%') 
                {
                    if (UNLIKELY(*(s+1) == '%'))
                    {
                        s++ ; 
                    }
                    else 
                    {
                        FATAL("missing value") ; 
                    }
                }
                pushValue(*s++) ; 
            }
        }
        
    private :  
        const std::string file_name_ ; 
        std::ofstream file_ ; 
        LockFreeQueue<LogElement> queue_ ; 
        std::atomic<bool> running_ = {true} ; 
        std::thread* logger_thread_ = nullptr ;  

} ; 

