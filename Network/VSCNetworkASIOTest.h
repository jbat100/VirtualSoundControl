//
//  VSCNetworkASIOTest.h
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#ifndef _VSC_NETWORK_ASIO_TEST_
#define _VSC_NETWORK_ASIO_TEST_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        typedef boost::shared_ptr<boost::asio::ip::tcp::socket> Socket_Ptr;
        
        class ASIOTest {
            
        public:
            
            ASIOTest();
            
        protected:
            
            void startAccept(Socket_Ptr sock);
            
            void handleAccept(Socket_Ptr sock, const boost::system::error_code &err);
            
            
        private:
            
            Socket_Ptr mSocket;
            
            boost::asio::io_service mService;
            
            boost::asio::ip::tcp::endpoint mEndpoint;
            boost::asio::ip::tcp::acceptor mAcceptor;
            
        };
    }
}


#endif /* defined(_VSC_NETWORK_ASIO_TEST_) */
