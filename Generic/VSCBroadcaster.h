
#ifndef _VSC_BROADCASTER_H_
#define _VSC_BROADCASTER_H_

#include "VSCListener.h"

#include <vector>

namespace VSC {
    
    /*
     *  All classes which accept listeners can inherit from this class
     *  to avoid having to re-write the basic functionality...
     */
    
    class Broadcaster
    {
        
    public:
        
        virtual ~Broadcaster();
        
        const Listeners& getListeners(void) {return mListeners;}
        void addListener(Listener::SPtr listener);
        void removeListener(Listener::SPtr listener);
        
    protected:
        
        // so that subclasses can only accept specific Listener subclasses
        virtual bool checkListener(Listener::SPtr listener) {return true;}
        
    private:
        
        Listeners mListeners;
        
    };
	
}

#endif