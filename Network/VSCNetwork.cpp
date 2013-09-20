
#include "VSCNetwork.h"

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

static boost::asio::io_service VSC::Network::sharedIOServiceInstance;

boost::asio::io_service& VSC::Network::sharedIOService(void)
{
    return sharedIOServiceInstance;
}