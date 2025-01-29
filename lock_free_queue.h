#pragma once 

#include <iostream>  
#include <vector> 
#include <atomic>

namespace Common {
    template<typename T> 
    class LockFreeQueue final 
    {
        public : 
           LockFreeQueue(std::size_t num_elems) : 
            store_(num_elems,T()){} 

            auto getNextToWriteTo() noexcept
            {
                return &store_[next_write_index_] ; 
            }

            auto updateWriteIndex() noexcept 
            {
                next_write_index_ = (next_write_index_ + 1) % store_.size() ; 
                num_elements_++ ; 
            }
            
            auto getNextToRead() const noexcept -> T* 
            {
                return (size() ? &store[next_read_index_]) : nullptr ; 
                
            }

            auto updateReadIndex() noexcept 
            {
                next_read_index_ = (next_read_index_ + 1 ) % store_.size() ; 
                num_elements_++ ; 
            }

            auto size() const noexcept 
            {
                return num_elements_.load() ; 
            }

            LockFreeQueue() = delete ; 
            LockFreeQueue(LockFreeQueue&) = delete ; 
            LockFreeQueue(LockFreeQueue&&) = delete ;  
            LockFreeQueue& operator=(const LockFreeQueue&) = delete ; 
            LockFreeQueue& operator=(const LockFreeQueue&&) = delete ; 



        private : 
            std::vector<T> store_ ; 
            std::atomic<std::size_t> next_write_index_   ; 
            std::atomic<std::size_t> next_read_index_ ; 
            std::atomic<std::size_t> num_elements_ ; 
    };

    
}