
#ifndef _VSC_NETWORK_TCP_LISTENER_
#define _VSC_NETWORK_TCP_LISTENER_

#include "VSCListener.h"

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        class TCPClient;
        class TCPServer;
        
        class TCPListener : public Listener
        {
        public:
            
            friend class TCPClient;
            friend class TCPServer;
            
            TCPListener();
            ~TCPListener();
            
        protected:
            
            virtual void readMessage(std::string& message);
        };

    }
}


#endif /* defined(_VSC_NETWORK_TCP_LISTENER_) */
