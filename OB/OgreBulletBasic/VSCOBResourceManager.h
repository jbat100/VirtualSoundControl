
#ifndef _VSC_OB_RESOURCE_MANAGER_H_
#define _VSC_OB_RESOURCE_MANAGER_H_

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        class Application;
        
        class ResourceManager
        {
            
        public:
            
            friend class Application;
            
            typedef boost::shared_ptr<ResourceManager>  SPtr;
            typedef boost::weak_ptr<ResourceManager>    WPtr;
            
            /*------------------------------------------------------
             *  Constructor / Destructor
             */
            
            ResourceManager(std::string resourceFilePath) : mResourceFilePath(resourceFilePath) {}
            virtual ~ResourceManager();
            
        protected:
            
            virtual void internalSetupResources(void) {}
            virtual void internalLoadResources(void) {}
            
        private:
            
            /*
             *  setupResources() and loadResources() should be called by the application singleton
             */
            
            void setupResources(); // call init before doing anything
            void loadResources();
            
            std::string mResourceFilePath;


        };
        
    }
}

#endif //_VSC_OB_RESOURCE_MANAGER_H_

