

#ifndef _VSC_NETWORK_
#define _VSC_NETWORK_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        typedef boost::shared_ptr<boost::asio::ip::tcp::socket> TCPSocket_Ptr;
        
        boost::asio::io_service& sharedIOService(void);
    }
}


#endif /* defined(_VSC_NETWORK_) */
