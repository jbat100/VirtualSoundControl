//
//  VSCNetworkASIOTest.h
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#ifndef _VSC_NETWORK_TCP_SERVER_
#define _VSC_NETWORK_TCP_SERVER_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        class TCPSession;
        
        class TCPServer
        {
            
        public:
            
            TCPServer(boost::asio::io_service& io_service, short port);
            
        private:
            
            void startAccept();
            void handleAccept(TCPSession* new_session, const boost::system::error_code& error);
            
            boost::asio::io_service& mIOService;
            boost::asio::tcp::acceptor mAcceptor;
        };
        
        class TCPSession
        {
            
        public:
            
            TCPSession(boost::asio::io_service& io_service);
            boost::asio::tcp::socket& socket() { return mSocket; }
            void start();
            
        private:
            
            void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
            void handleWrite(const boost::system::error_code& error);
            
            boost::asio::tcp::socket mSocket;
            enum { maxLength = 1024 };
            char mData[maxLength];
            
        }
    }
}


#endif /* defined(_VSC_NETWORK_TCP_SERVER_) */
