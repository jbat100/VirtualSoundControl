

#ifndef _VSC_NETWORK_
#define _VSC_NETWORK_

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        boost::asio::io_service& sharedIOService(void);
        
        class TCPServer;
        class TCPClient;
        class TCPSession;
        
        typedef boost::shared_ptr<TCPServer>            TCPServer_SPtr;
        typedef boost::shared_ptr<TCPClient>            TCPClient_SPtr;
        typedef boost::shared_ptr<TCPSession>           TCPSession_SPtr;
        
        typedef boost::asio::ip::tcp::resolver          TCPResolver;
        typedef boost::shared_ptr<TCPResolver>          TCPResolver_SPtr;
        
        typedef boost::asio::ip::tcp::socket            TCPSocket;
        typedef boost::shared_ptr<TCPSocket>            TCPSocket_SPtr;
        
        typedef boost::asio::ip::tcp::resolver::query   TCPQuery;
        typedef boost::shared_ptr<TCPQuery>             TCPQuery_SPtr;
        
        typedef std::vector<TCPSession_SPtr>            TCPSessions;
    }
}


#endif /* defined(_VSC_NETWORK_) */
