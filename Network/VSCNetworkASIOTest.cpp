//
//  VSCNetworkASIOTest.cpp
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#include "VSCNetworkASIOTest.h"

#include <iostream>
#include <boost/bind.hpp>

using namespace VSC::Network;
using namespace boost::asio;

ASIOTest::ASIOTest() :
mEndpoint( ip::tcp::v4(), 2001),
mAcceptor( mService, mEndpoint )
{
    mSocket = Socket_Ptr(new ip::tcp::socket(mService));
    this->startAccept(mSocket);
    mService.run();
}

void ASIOTest::startAccept(Socket_Ptr sock)
{
    mAcceptor.async_accept(*sock,  boost::bind( &ASIOTest::handleAccept, this, sock, _1) );
}

void ASIOTest::handleAccept(Socket_Ptr sock, const boost::system::error_code &err)
{
    if (err)
    {
        std::cerr << "handleAccept error " << err << std::endl;
        return;
    }
    
    std::cerr << "handleAccept error " << err << std::endl;
    
    // at this point, you can read/write to the socket
    mSocket = Socket_Ptr(new ip::tcp::socket(mService));
    this->startAccept(mSocket);
}
