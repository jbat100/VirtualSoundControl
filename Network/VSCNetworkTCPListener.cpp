
#include "VSCNetworkTCPListener.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/current_function.hpp>

using namespace VSC::Network;
using namespace boost::asio;


TCPListener::TCPListener()
{
    
}

TCPListener::~TCPListener()
{
    
}

void TCPListener::readMessage(std::string& message)
{
    std::cout << BOOST_CURRENT_FUNCTION << " " << message << std::endl;
}


