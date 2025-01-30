#pragma once 

#include <iostream>  
#include <vector> 
#include <atomic>

namespace Common {
    template<typename T>
    class MemPool final 
    {
        public : 

        explicit MemPool(size_t numElems) 
        {
            store_.assign(numElems,ObjectBlock()) ;  
        }

        template<typename... Args> 
        T* allocate(Args... args) noexcept 
        {
            auto obj_block = &(store_[next_free_index_]) ;  
            ASSERT(obj_block->is_free_ , "Expected free ObjectBlock at index: "+ std::string(next_free_index_)) ;  
            T* ret = &(obj_block->object_) ;   
            ret = new(ret) T(args ...) ;  
            obj_block->is_free = false ;  


            updateNextFreeIndex(); 

            return ret ;  
        }

        auto deallocate(const T* elem) noexcept 
        {
            const auto elem_index = (reinterpret_cast<const ObjectBlock*>(elem) - &store_[0]) ;  
            ASSERT(elem_index >= 0 && static_cast<size_t>(elem_index) < store_.size(), "Element being deallocated is not in the memory pool ") ; 
            ASSERT(!store_[elem_index].is_free_,"Expected in use ObjectBlock at index "+std::to_string(elem_index))  ;  
            store_[elem_index].is_free_ = true ; 
        }

        MemPool() = delete ; 
        MemPool(const MemPool&) = delete ;  
        MemPool(const MemPool&&) = delete ;  
        MemPool &operator=(const MemPool &) = delete ;  
        MemPool &operator=(const MemPool &&) = delete ;  


        private : 

        auto  updateNextFreeIndex() noexcept 
        {
            const auto initial_free_index = next_free_index_ ;  
            while(!store_[next_free_index_].is_free) 
            {
                next_free_index_++ ;  
                if (UNLIKELY(next_free_index_ == store_.size())) 
                {
                    next_free_index_ = 0 ; 
                }
                if (UNLIKELY(initial_free_index == next_free_index_)) 
                {
                    ASSERT(initial_free_index != next_free_index_, "MemoryPool out of space ") ;  
                }
            }
        }
        struct ObjectBlock {
            T object_ ;  
            bool is_free_ = true ; 
        } ;  

        std::vector<ObjectBlock> store_ ;  
        size_t next_free_index_ = 0 ; 
    } ; 
}