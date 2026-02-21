#pragma once 

#include <array.h>
#include <sstream>
#include <common.h>
#include "ME_order.h"

struct MEOrderAtPrice 
{
    Side side_ = Side::INVALID; 
    Price price_ = Price_INVALID;

    MEOrder* first_me_order_ = nullptr; 

    MEOrderAtPrice* prev_entry_ = nullptr ; 
    MEOrderAtPrice* next_entry_ = nullptr ; 

    MEOrderAtPrice() = default ; 

    MEOrderAtPrice(Side side, Price price, MEOrder* first_me_order, 
                    MEOrderAtPrice* prev_entry, MEOrderAtPrice* next_entry) :  
    side_(side), price_(price), first_me_order_(first_me_order), 
    prev_entry_(prev_entry), next_entry_(next_entry)
    {} 

    std::string toString() 
    {
        std::stringstream ss ; 
        ss<<"MEOrderAtPrice [" 
          <<"side "<<sideToString(side_) 
          <<"price "<<sideToString(price_)<<"]" ;  
    }

} ;


typedef std::array<MEOrderAtPrice,ME_MAX_PRICE_LEVELS> OrdersAtPriceHashMap ; 