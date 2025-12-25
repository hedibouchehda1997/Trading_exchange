#include "utils_threads.h"
#include "mem_pool.h"
#include "lock_free_queue.h"
#include <iostream>




auto randomFunction(int a, int b, bool sleep) 
{
    auto result = a + b ;  

    std::cout<<"the result is "<<result<<std::endl ; 

    if (sleep) 
    {
        using namespace std::literals::chrono_literals ;  
        std::this_thread::sleep_for(5s)  ; 
    }

    std::cout<<" dummy function done "<<std::endl ; 

}

auto consumeLockFreeQueue(LockFreeQueue<double>* lf_queue) 
{
    using namespace std::literals::chrono_literals ; 
    std::this_thread::sleep_for(5s) ;  

    while (lf_queue->size()) 
    {
        auto consumed_elem = lf_queue->getNextToRead() ;  
        lf_queue->updateReadIndex() ;  

        std::cout<<"consumed element : "<<*consumed_elem<<std::endl ;  
        std::this_thread::sleep_for(5s) ;  

    }

    std::cout<<"thread consumer ended "<<std::endl ; 

}

int main() 
{


    //testing the thread creation
    auto t1 = createAndStartThread(0, "dummyfunction1" , randomFunction, 12,1,false) ; 

    auto t2 = createAndStartThread(0, "dummyfunction2" , randomFunction, 1,1,true) ;   

    std::cout<<"waiting for thread to finish executing "<<std::endl ;  

    t1->join() ; 
    t2->join() ; 
    
    delete t1; 
    delete t2; 

    std::cout<<"thread finished and getting out of program  "<<std::endl ; 

    //testing memory pool 
    auto int_mem_pool = MemPool<int>(50) ; 
    auto double_mem_pool = MemPool<double>(50) ; 

    for (size_t i=0; i<30; i++) 
    {
        auto int_ptr = int_mem_pool.allocate(i) ; 
        std::cout<<"allocating "<<*int_ptr<<" at "<<int_ptr<<std::endl ; 
        auto double_ptr = double_mem_pool.allocate(i*0.1) ; 

        if (i%5 == 0) 
        {
            int_mem_pool.deallocate(int_ptr) ; 
            double_mem_pool.deallocate(double_ptr) ; 
        }
    }

    //testing the lock free queue 
    LockFreeQueue<double> lock_free_queue(50) ;  

    auto t_consumer = createAndStartThread(-1,"consumer_function", consumeLockFreeQueue, &lock_free_queue) ; 

    for (size_t i=0; i<30; i++) 
    {
        double generated = 0.1*i ;  
        *(lock_free_queue.getNextToWriteTo()) = generated ; 
        lock_free_queue.updatWriteIndex() ; 
    }


    t_consumer->join() ; 
    delete t_consumer ; 



    return 0 ; 
}