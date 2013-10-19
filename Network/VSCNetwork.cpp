
#include "VSCNetwork.h"

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <boost/bind.hpp>
#include <boost/current_function.hpp>

namespace VSC
{
    namespace Network
    {
        static boost::asio::io_service VSC::Network::sharedIOServiceInstance;
        
        static boost::once_flag once = BOOST_ONCE_INIT;
        
        void runIOService(boost::asio::io_service& service);
        
        void runIOServiceTarget(boost::asio::io_service& service);
    }
}

void VSC::Network::runIOServiceTarget(boost::asio::io_service& service)
{
    std::cout << BOOST_CURRENT_FUNCTION << " running service shared service ..." << std::endl;
    service.run();
    std::cout << BOOST_CURRENT_FUNCTION << " finished running service shared service ..." << std::endl;
}

void VSC::Network::runIOService(boost::asio::io_service& service)
{
    boost::thread(runIOServiceTarget, service);
}

boost::asio::io_service& VSC::Network::sharedIOService(void)
{
    boost::call_once(once, boost::bind(runIOService, sharedIOServiceInstance));
    
    return sharedIOServiceInstance;
}