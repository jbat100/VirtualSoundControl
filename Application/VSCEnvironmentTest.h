
#ifndef _VSC_ENVIRONMENT_TEST_H_
#define _VSC_ENVIRONMENT_TEST_H_

#include "VSCEnvironment.h"
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    
    class EnvironmentTest
    {
        
    public:
        
        typedef boost::shared_ptr<EnvironmentTest>  SPtr;
        typedef boost::weak_ptr<EnvironmentTest>    WPtr;
        
        EnvironmentTest() {}
        
        void setupTestForEnvironment(Environment::SPtr environment);
        
    protected:
        
        virtual void internalSetupForEnvironment(Environment::SPtr environment);

    };

}

#endif //_VSC_ENVIRONMENT_TEST_H_

