#include "utils_threads.h" 


auto randomFunction(int a, int b, bool sleep) 
{
    auto result = a + b ;  

    std::cout<<"the result is "<<result<<std::endl ; 

    if (sleep) 
    {
        using namespace std::literals::chrono_literals ;  
        std::this_thread::sleep_for(5s) 
    }

    std::cout<<" dummy function done "<<std::endl ; 

}

int main() 
{
    using namespace Common ;   

    auto t1 = createAndStartThread(-1, "dummyfunction1" , randomFunction, 12,1,false) ; 

    auto t2 = createAndStartThread(-1, "dummyfunction2" , randomFunction, 1,1,true) ;   

    std::cout<<"waiting for thread to finish executing "<<std::endl ;  

    t1->join() ; 
    t2->join() ; 

    std::cout<<"thread finished and getting out of program  "<<std::endl ; 

    return 0 ; 
}
