#pragma once 

#include <sstream> 
#include "common.h"
#include <lock_free_queue.h> 


#pragma pack(push,1) 
enum class ClientRequestType : uint8_t {
    INVALID = 0 , 
    NEW = 1, 
    CANCEL = 2 
} ; 


inline std::string ClientRequestTypeToString(ClientRequestType type) 
{
    switch(type) 
    {
        case ClientRequestType::INVALID : 
            return "INVALID" ; 
        case ClientRequestType::NEW : 
            return "NEW" ; 
        case  ClientRequestType::CANCEL : 
            return "CANCEL" ; 

        return "UNKOWN" ; 
    }
}


//the structure that information for a single order
struct MEClientRequest 
{
    ClientRequestType type_ = ClientRequestType::INVALID ; 

    ClientId Client_id_ = ClientId_INVALID ; 
    TickerId ticker_id_ = TickerId_INVALID ; 

    OrderId order_id_ = OrderId_INVALID ; 
    Side side_ = Side::INVALID ; 

    Price price_ = Price_INVALID ; 
    Qty qty_ = Qty_INVALID ; 

    auto toString() const {
        std::stringstream ss; 
        ss <<"MEClientRequest"
            <<"["
            <<"type : "<<ClientRequestTypeToString(type_)
            <<"client id :"<<clientToString(Client_id_) 
            <<"ticker id : "<<tickerIdToString(ticker_id_) 
            <<"order id : "<<orderIdToString(order_id_)
            <<"side "<<sideToString(side_)
            <<"qty : "<<qtyToString(qty_)
            <<"price "<<priceToString(price_)<<"]" ; 
    
        return ss.str() ;
    }
}; 


typedef LockFreeQueue<MEClientRequest> MEClientRequestLFQueue ; 


#pragma pack(push,1)
enum class ClientResponseType 
{
    INVALID=0,
    ACCEPTED=1,
    CANCELED=2, 
    FILLED=3,
    CANCEL_REJECTED=4,
} ; 

inline std::string clientRequestTypeToString(ClientResponseType client_response_type) 
{
    switch(client_response_type) 
    {
        case ClientResponseType::INVALID  :
            return "INVALID" ; 
        case ClientResponseType::ACCEPTED : 
            return "ACCEPTED" ; 
        case ClientResponseType::CANCELED : 
            return "CANCELED" ; 
        case ClientResponseType::FILLED : 
            return "FILLED" ; 
        case ClientResponseType::CANCEL_REJECTED : 
            return "CANCEL_REJECTED" ; 
    }
}

//structure used by the matching engine to send data to the participent about his order 
struct MEClientResponse 
{
    ClientResponseType type_ = ClientResponseType::INVALID; 

    ClientId client_id_ = ClientId_INVALID ; 
    TickerId ticker_id_ = TickerId_INVALID ; 

    //client_order_id_ identfies the order this message affect
    OrderId client_order_id_ = OrderId_INVALID; 

    //market_order_id identifis the order in the market data stream
    OrderId market_order_id_ = OrderId_INVALID;

    Side side_ = Side::INVALID ; 

    Price price_ = Price_INVALID ; 

    Qty exec_qty_ = Qty_INVALID; 

    Qty leaves_qty_ = Qty_INVALID; 


    std::string toString() 
    {
        std::stringstream ss ; 

        ss<<"MEClientResponse "
          <<" [ "
          <<"type : "<<clientRequestTypeToString(type_) 
          <<"client id : "<<clientToString(client_id_)
          <<"ticker id : "<<tickerIdToString(ticker_id_) 
          <<"client order id "<<orderIdToString(client_order_id_) 
          <<"market order id "<<orderIdToString(market_order_id_) 
          <<"side "<<sideToString(side_) 
          <<"price_ "<<priceToString(price_) 
          <<"exec_qty "<<qtyToString(exec_qty_) 
          <<"leaves_qty "<<qtyToString(leaves_qty_) 
          <<"]" ; 
        
        return ss.str() ; 
    }

} ; 