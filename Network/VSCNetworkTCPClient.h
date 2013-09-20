
#ifndef _VSC_NETWORK_TCP_CLIENT_
#define _VSC_NETWORK_TCP_CLIENT_

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace VSC
{
    namespace Network
    {
        
        class TCPClient
        {
            
        public:
            
            TCPClient(boost::asio::io_service& io_service);
            ~TCPClient();
            
            void start(tcp::resolver::iterator endpoint_iter);
            void stop();
            void write(std::string& s);
            
        private:
            
            void startConnect(tcp::resolver::iterator endpoint_iter);
            void handleConnect(const boost::system::error_code& ec, boost::asio::tcp::resolver::iterator endpoint_iter);
            void startRead();
            void handleRead(const boost::system::error_code& ec);
            void handleWrite(const boost::system::error_code& ec);
            void checkDeadline();
            

            bool mStopped;
            boost::asio::tcp::socket mSocket;
            boost::asio::streambuf mInputBuffer;
            boost::asio::deadline_timer mDeadlineTimer;
            boost::asio::deadline_timer mHeartbeatTimer;
        };

    }
}


#endif /* defined(_VSC_NETWORK_TCP_CLIENT_) */
