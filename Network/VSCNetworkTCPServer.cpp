//
//  VSCNetworkASIOTest.cpp
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#include "VSCNetworkTCPServer.h"
#include "VSCNetwork.h"

#include <iostream>

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/current_function.hpp>
#include <boost/foreach.hpp>

using namespace VSC::Network;
using namespace boost::asio;

TCPServer::TCPServer(io_service& service, short port) :
mIOService(service),
mAcceptor(service, ip::tcp::endpoint(ip::tcp::v4(), port))
{
    this->startAccept();
}

void TCPServer::startAccept()
{
    TCPSession_SPtr newSession = TCPSession_SPtr(new TCPSession(*this));
    mAcceptor.async_accept(newSession->getSocket(),
                           boost::bind(&TCPServer::handleAccept,
                                       this,
                                       newSession,
                                       boost::asio::placeholders::error));
}

void TCPServer::handleAccept(TCPSession_SPtr newSession, const boost::system::error_code& error)
{
    if (!error)
    {
        BOOST_ASSERT(newSession);
        BOOST_ASSERT(std::find(mSessions.begin(), mSessions.end(), newSession) == mSessions.end());
        
        mSessions.push_back(newSession);
        newSession->start();
        
        BOOST_FOREACH(Listener_WPtr listener, this->getListeners())
        {
            TCPServerListener_SPtr serverListener = boost::dynamic_pointer_cast<TCPServerListener>(listener.lock());
            serverListener->createdTCPSession(newSession);
        }
    }
    // no need to delete if error, the smart pointer magic should delete the session
    
    this->startAccept();
}

void TCPServer::handleError(TCPSession_SPtr session, const boost::system::error_code& error)
{
    std::cout << BOOST_CURRENT_FUNCTION << " " << session.get() << std::endl;
    TCPSessions::iterator it = std::find(mSessions.begin(), mSessions.end(), session);
    BOOST_ASSERT(it != mSessions.end());
    if (it != mSessions.end())
    {
        mSessions.erase(it);
    }
    BOOST_FOREACH(Listener_WPtr listener, this->getListeners())
    {
        TCPServerListener_SPtr serverListener = boost::dynamic_pointer_cast<TCPServerListener>(listener.lock());
        serverListener->endedTCPSession(session);
    }
}

void TCPServer::write(std::string content)
{    
    BOOST_FOREACH(TCPSession_SPtr session, mSessions)
    {
        session->write(content);
    }
}

bool TCPServer::checkListener(Listener_SPtr listener)
{
    TCPServerListener_SPtr serverListener = boost::dynamic_pointer_cast<TCPServerListener>(listener);
    if (!serverListener) return false;
    return true;
}


TCPSession::TCPSession(TCPServer& server) :
mServer(server),
mSocket(server.mIOService)
{
    
}

TCPSession::~TCPSession()
{
    std::cout << BOOST_CURRENT_FUNCTION << " destroying " << this << std::endl;
}

void TCPSession::start()
{
    mSocket.async_read_some(boost::asio::buffer(mData, maxLength),
                            boost::bind(&TCPSession::handleRead,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void TCPSession::write(std::string content)
{
    async_write(mSocket,
                boost::asio::buffer(content, 1),
                boost::bind(&TCPSession::handleWrite,
                            this,
                            boost::asio::placeholders::error));
}


void TCPSession::handleRead(const boost::system::error_code& error, size_t bytesTransferred)
{
    if (!error)
    {
        std::string readString(mData, bytesTransferred);
        
        std::cout << BOOST_CURRENT_FUNCTION << " no error, read: " << readString << std::endl;
        
        
    }
    else
    {
        std::cerr << BOOST_CURRENT_FUNCTION << " ERROR" << std::endl;
        
        mServer.handleError(this->shared_from_this(), error);
    }
}

void TCPSession::handleWrite(const boost::system::error_code& error)
{
    if (!error)
    {
        std::cout << BOOST_CURRENT_FUNCTION << " no error" << std::endl;
        
        mSocket.async_read_some(boost::asio::buffer(mData, maxLength),
                                boost::bind(&TCPSession::handleRead,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cerr << BOOST_CURRENT_FUNCTION << " ERROR" << std::endl;
        
        mServer.handleError(this->shared_from_this(), error);
    }
}
