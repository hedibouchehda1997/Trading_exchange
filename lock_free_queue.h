#pragma once 

#include <iostream> 
#include <vector> 
#include <atomic> 

#include "macros.h" 

namespace Common {
    template<typename T> 
    class LockFreeQueue final {
        public : 
            LockFreeQueue(std::size_t numElems) : 
                store_.assing(numElems) {} 

            auto getNextToWriteTo() noexcept -> T* {
                return &store_[new_write_index_] ;  
            }
            
            auto updatWriteIndex() noexcept -> void {
                new_write_index_ = (new_write_index_+1) % store_.size() ;  
                num_elems_++ ; 
            }
            
            auto getNextToRead() noexcept -> const T* {
                return (size() ? &store_[next_read_index_] : nullptr) 
            }

            auto updateReadIndex() noexcept -> void {
                next_read_index_ = (next_read_index_+1) % store_.size() ;  
                ASSERT(num_elems_ != 0 , "Read an invalid relement in "+ std::to_string(std::pthread_self())) ; 
                num_elems_-- ; 
            } 
            
            auto size() noexcept -> const size_t {
                return num_elems_.load() ; 
            }

            LockFreeQueue(const LockFreeQueue&) = delete ; 
            LockFreeQueue(const LockFreeQueue&&) = delete ;  
            
            LockFreeQueue& operator=(const LockFreeQueue&) = delete ;  
            LockFreeQueue& operator=(const LockFreeQueue&&) = delete ;  



        private : 
            std::vector<T> store_ ; 

            std::atomic<size_t> new_write_index_ = {0} ; 
            std::atomic<size_t> next_read_index_ = {0} ; 

            std::atomic<size_t> num_elems_ = {0} ; 
    }; 
}