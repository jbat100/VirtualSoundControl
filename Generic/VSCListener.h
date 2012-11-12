
#ifndef _VSC_LISTENER_H_
#define _VSC_LISTENER_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    /*
     *  Very abstract class to be used with Broadcaster
     */
    
    class Listener
    {
        
    public:
        
        typedef boost::shared_ptr<Listener> SPtr;
        typedef boost::weak_ptr<Listener>   WPtr;
        
        virtual ~Listener();
    };
    
    typedef std::vector<Listener::WPtr> Listeners;
	
}

#endif