//
//  VSCNetworkASIOTest.h
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#ifndef _VSC_NETWORK_TCP_SERVER_
#define _VSC_NETWORK_TCP_SERVER_

#include "VSCNetwork.h"
#include "VSCBroadcaster.h"
#include "VSCListener.h"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        
        /**
         TCPServerListener : subclass this to be informed when the server creates or ends a TCPSession
         */
        
        class TCPServerListener : public Listener
        {
            
        public:
            
            friend class TCPServer;
            
        protected:
            
            virtual void createdTCPSession(TCPSession_SPtr session);
            virtual void endedTCPSession(TCPSession_SPtr session);
            
        };
        
        typedef boost::shared_ptr<TCPServerListener> TCPServerListener_SPtr;
        
        /**
         TCPServer : responsible for creating and ending TCPSessions
         */
        
        class TCPServer : public Broadcaster
        {
            
        public:
            
            friend class TCPSession;
            
            TCPServer(boost::asio::io_service& io_service, short port);
            
            const TCPSessions& getSessions(void) { return mSessions; }
            
            void write(std::string content);
            
        private:
            
            void startAccept();
            void handleAccept(TCPSession_SPtr newSession, const boost::system::error_code& error);
            void handleError(TCPSession_SPtr session, const boost::system::error_code& error);
            
            bool checkListener(Listener_SPtr listener);
            
            boost::asio::io_service& mIOService;
            boost::asio::ip::tcp::acceptor mAcceptor;
            
            TCPSessions mSessions;
        };
        
        /**
         TCPSession : interaction with a single client
         */
        
        class TCPSession : public Broadcaster, public boost::enable_shared_from_this<TCPSession>
        {
            
        public:
            
            TCPSession(TCPServer& server);
            ~TCPSession();
            
            TCPSocket& getSocket() { return mSocket; }
            
            void start();
            
            void write(std::string content);
            
        private:
            
            void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
            void handleWrite(const boost::system::error_code& error);
            
            TCPServer& mServer;
            
            TCPSocket mSocket;
            
            enum { maxLength = 1024 };
            
            char mData[maxLength];
            
        };
    }
}


#endif /* defined(_VSC_NETWORK_TCP_SERVER_) */
