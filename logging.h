#pragma once 

#include <string> 
#include <fstream> 
#include <cstdio> 

#include "macros.h" 
#include "lock_free_queue.h" 
#include "utils_times.h"
#include "utils_threads.h"

namespace Common {
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
        LogType type = LogType::CHAR ;  
        union {
            char c ; 
            int i ; 
            long l ; 
            long long ll ; 
            unsigned u ; 
            unsigned ul ; 
            unsigned long ul ;  
            unsigned long long ull ; 
            float f ; 
            double d; 
        } u_ ;  
    } ; 

    class Logger final {
        public : 
        
        private :  
        const std::string file_name_ ; 
        std::ofsteram file_ ; 
    }

}