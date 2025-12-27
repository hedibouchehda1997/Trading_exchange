#include "logging.h"

Logger::Logger(std::string file_name) : queue_(LOG_QUEUE_SIZE),file_name_(file_name)
{
            file_.open(file_name_) ; 
            ASSERT(file_.is_open(),"failed to open the logging file" + file_name_) ; 
            logger_thread_ = createAndStartThread(-1,"Logger_thread",[this](){
                flushQueue() ; 
            }) ; 
            ASSERT(logger_thread_ != nullptr, "Failed to launch logger thread") ; 
}

Logger::~Logger() 
{
    std::cout<<"Flushing and closing Logger for "<<file_name_<<std::endl ; 

    while (queue_.size()) 
    {
        using namespace std::literals::chrono_literals ;  
        std::this_thread::sleep_for(1s) ; 
    }

    running_ = false ; 
    file_.close() ; 
}


void Logger::flushQueue() noexcept
{
    while (running_)
    {
        for (auto next = queue_.getNextToRead(); queue_.size() && next ; next = queue_.getNextToRead()) 
        {
            switch (next->type_) 
            {
                case LogType::CHAR : 
                    file_<<next->u_.c ; 
                    break ; 
                case LogType::INTEGER : 
                    file_<<next->u_.i; 
                    break ; 
                case LogType::LONG_INTEGER : 
                    file_<<next->u_.l ; 
                    break ; 
                case LogType::LONG_LONG_INTEGER : 
                    file_<<next->u_.ll ; 
                    break ; 
                case LogType::UNSIGNED_INTEGER : 
                    file_<<next->u_.u ; 
                    break ; 
                case LogType::UNSIGNED_LONG_INTEGER : 
                    file_<<next->u_.ul ; 
                    break ; 
                case LogType::UNSIGNED_LONG_LONG_INTEGER : 
                    file_<<next->u_.ull ; 
                    break ; 
                case LogType::FLOAT : 
                    file_<<next->u_.f ; 
                    break ; 
                case LogType::DOUBLE : 
                    file_<<next->u_.d ; 
                    break ; 
            }
            queue_.updateReadIndex() ; 
            next = queue_.getNextToRead() ; 
        }
        using namespace std::literals::chrono_literals ;  
        std::this_thread::sleep_for(1ms) ; 
    }
}