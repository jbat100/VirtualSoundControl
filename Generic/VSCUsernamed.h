
#ifndef _VSC_USERNAMED_H_
#define _VSC_USERNAMED_H_

#include <string>

namespace VSC {
    
    /*
     *  Very abstract class to be used with Broadcaster
     */
    
    class Usernamed
    {
        
    public:
        
        virtual ~Usernamed() {}
        
        std::string getUsername(void) {return mUsername;}
        void setUsername(std::string username) {mUsername = username;}
        
    private:
        
        std::string mUsername;
    };
	
}

#endif // _VSC_USERNAMED_H_