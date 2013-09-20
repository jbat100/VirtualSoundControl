//
//  VSCNetworkASIOTest.cpp
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#include "VSCNetworkTCPServer.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/current_function.hpp>

using namespace VSC::Network;
using namespace boost::asio;

TCPServer::TCPServer(io_service& service, short port) :
mIOService(service),
mAcceptor(service, tcp::endpoint(tcp::v4(), port))
{
    startAccept();
}

void TCPServer::startAccept()
{
    TCPSession* newSession = new TCPSession(mIOService);
    mAcceptor.async_accept(new_session->socket(),
                           boost::bind(&server::handleAccept, this, newSession,
                                       boost::asio::placeholders::error));
}

void TCPServer::handleAccept(TCPSession* newSession, const boost::system::error_code& error)
{
    if (!error)
    {
        newSession->start();
    }
    else
    {
        delete newSession;
    }
    
    startAccept();
}


TCPSession::TCPSession(boost::asio::io_service& io_service) :
mSocket(io_service),
mState(StateInitialized)
{
    
}

void TCPSession::start()
{
    mSocket.async_read_some(boost::asio::buffer(mData, max_length),
                            boost::bind(&session::handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}


void TCPSession::handleRead(const boost::system::error_code& error, size_t bytesTransferred)
{
    if (!error)
    {
        std::string readString(mData, bytesTransferred);
        
        std::cout << BOOST_CURRENT_FUNCTION << " no error, read: " << readString << std::endl;
        
        /*
         boost::asio::async_write(mSocket,
         boost::asio::buffer(mData, bytes_transferred),
         boost::bind(&session::handleWrite, this,
         boost::asio::placeholders::error));
         */
    }
    else
    {
        std::cerr << BOOST_CURRENT_FUNCTION << " ERROR" << std::endl;
        delete this;
    }
}

void TCPSession::handleWrite(const boost::system::error_code& error)
{
    if (!error)
    {
        std::cout << BOOST_CURRENT_FUNCTION << " no error" << std::endl;
        
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cerr << BOOST_CURRENT_FUNCTION << " ERROR" << std::endl;
        delete this;
    }
}
