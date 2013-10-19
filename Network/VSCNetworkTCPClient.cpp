
#include "VSCNetworkTCPClient.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/current_function.hpp>

using namespace VSC::Network;
using namespace boost::asio;


TCPClient::TCPClient(boost::asio::io_service& io_service) :
mStopped(false),
mSocket(io_service),
mDeadlineTimer(io_service),
mHeartbeatTimer(io_service)
{
    std::cout << "Creating TCPClient " << this << std::endl;
}

TCPClient::~TCPClient()
{
    std::cout << "Destroying TCPClient " << this << std::endl;
}

// Called by the user of the client class to initiate the connection process.
// The endpoint iterator will have been obtained using a tcp::resolver.
void TCPClient::start(ip::tcp::resolver::iterator endpoint_iter)
{
    // Start the connect actor.
    this->startConnect(endpoint_iter);
    
    // Start the deadline actor. You will note that we're not setting any
    // particular deadline here. Instead, the connect and input actors will
    // update the deadline prior to each asynchronous operation.
    mDeadlineTimer.async_wait(boost::bind(&TCPClient::checkDeadline, this));
}
    
// This function terminates all the actors to shut down the connection. It
// may be called by the user of the client class, or by the class itself in
// response to graceful termination or an unrecoverable error.
void TCPClient::stop()
{
    mStopped = true;
    boost::system::error_code ignored_ec;
    mSocket.close(ignored_ec);
    mDeadlineTimer.cancel();
    mHeartbeatTimer.cancel();
}
    
void TCPClient::startConnect(ip::tcp::resolver::iterator endpointIter)
{
    if (endpointIter != ip::tcp::resolver::iterator())
    {
        std::cout << "Trying to connect to " << endpointIter->endpoint() << "...\n";
        
        // Set a deadline for the connect operation.
        mDeadlineTimer.expires_from_now(boost::posix_time::seconds(60));
        
        // Start the asynchronous connect operation.
        mSocket.async_connect(endpointIter->endpoint(),
                              boost::bind(&TCPClient::handleConnect, this, _1, endpointIter));
    }
    else
    {
        // There are no more endpoints to try. Shut down the client.
        this->stop();
    }
}

void TCPClient::handleConnect(const boost::system::error_code& ec, ip::tcp::resolver::iterator endpointIter)
{
    if (mStopped)
    {
        std::cout << BOOST_CURRENT_FUNCTION << " client is stopped" << std::endl;
        return;
    }
    
    // The async_connect() function automatically opens the socket at the start
    // of the asynchronous operation. If the socket is closed at this time then
    // the timeout handler must have run first.
    if (!mSocket.is_open())
    {
        std::cout << BOOST_CURRENT_FUNCTION << " connect timed out" << std::endl;
        // Try the next available endpoint.
        this->startConnect(++endpointIter);
    }
    
    // Check if the connect operation failed before the deadline expired.
    else if (ec)
    {
        std::cout << BOOST_CURRENT_FUNCTION << " connect error: " << ec.message() << std::endl;
        
        // We need to close the socket used in the previous connection attempt
        // before starting a new one.
        mSocket.close();
        
        // Try the next available endpoint.
        this->startConnect(++endpointIter);
    }
    
    // Otherwise we have successfully established a connection.
    else
    {
        std::cout << BOOST_CURRENT_FUNCTION << " connected successfully to " << endpointIter->endpoint() << std::endl;
        
        // Start the input actor.
        this->startRead();
    }
}

void TCPClient::startRead()
{
    std::cout << "Reading up to new line..." << std::endl;
    
    // Set a deadline for the read operation.
    mDeadlineTimer.expires_from_now(boost::posix_time::seconds(30));
    
    // Start an asynchronous operation to read a newline-delimited message.
    async_read_until(mSocket, mInputBuffer, '\n', boost::bind(&TCPClient::handleRead, this, _1));
}

void TCPClient::handleRead(const boost::system::error_code& ec)
{
    std::cout << BOOST_CURRENT_FUNCTION << " start" << std::endl;
    
    if (mStopped)
    {
        return;
    }
    
    if (!ec)
    {
        // Extract the newline-delimited message from the buffer.
        std::string line;
        std::istream is(&mInputBuffer);
        std::getline(is, line);
        
        // Empty messages are heartbeats and so ignored.
        if (!line.empty())
        {
            std::cout << "Received: " << line << std::endl;
        }
        else
        {
            std::cout << "Received heartbeat " << std::endl;
        }
        
        this->startRead();
    }
    else
    {
        std::cout << "Error on receive: " << ec.message() << "\n";
        this->stop();
    }
}

void TCPClient::write(std::string& message)
{
    std::cout << BOOST_CURRENT_FUNCTION << " start" << std::endl;
    
    if (mStopped)
    {
        std::cout << "write stopped" << std::endl;
        return;
    }
    
    std::cout << "Writing: " << message << std::endl;
    
    // Start an asynchronous operation to send a heartbeat message.
    async_write(mSocket, boost::asio::buffer(message, 1), boost::bind(&TCPClient::handleWrite, this, _1));
}

void TCPClient::handleWrite(const boost::system::error_code& ec)
{
    std::cout << "handleWrite: " << std::endl;
    
    if (mStopped)
    {
        std::cout << "stopped: " << std::endl;
        return;
    }
    
    if (!ec)
    {
        std::cout << "handleWrite with no error" << std::endl;
        // Wait 10 seconds before sending the next heartbeat.
        //mHeartbeatTimer.expires_from_now(boost::posix_time::seconds(10));
        //mHeartbeatTimer.async_wait(boost::bind(&client::write, this, "Hello, I'm the client\n"));
    }
    else
    {
        std::cerr << "handleWrite ERROR: " << ec.message() << "\n";
        this->stop();
    }
}

void TCPClient::checkDeadline()
{
    if (mStopped)
    {
        return;
    }
    
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (mDeadlineTimer.expires_at() <= deadline_timer::traits_type::now())
    {
        std::cout << BOOST_CURRENT_FUNCTION << " DEADLINE PASSED, STOPPING!!!" << std::endl;
        
        // The deadline has passed. The socket is closed so that any outstanding
        // asynchronous operations are cancelled.
        mSocket.close();
        
        // There is no longer an active deadline. The expiry is set to positive
        // infinity so that the actor takes no action until a new deadline is set.
        mDeadlineTimer.expires_at(boost::posix_time::pos_infin);
    }
    
    // Put the actor back to sleep.
    mDeadlineTimer.async_wait(boost::bind(&TCPClient::checkDeadline, this));
}



