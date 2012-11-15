
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

@protocol VSCOBOSXSceneListenerTarget <NSObject>

@optional

-(void) scene:(VSC::OB::Scene::SPtr)scene registeredElement:(VSC::OB::Scene::Element::SPtr)element;

@end


namespace VSC
{
    namespace OB
    {
        
        class OSXSceneListener : public Scene::Listener
        {
            
        public:
            
            typedef boost::shared_ptr<OSXSceneListener>    SPtr;
            typedef boost::weak_ptr<OSXSceneListener>      WPtr;
            
            id<VSCOBOSXSceneListenerTarget> getTarget(void);
            void getTarget(id<VSCOBOSXSceneListenerTarget>);
            
            /*
             *  Scene::Listener overrides
             */
            
            virtual void sceneRegisteredElement(Scene::SPtr scene, Scene::Element::SPtr element);
            
        private:
            
            id<VSCOBOSXSceneListenerTarget> mTarget;
            
        };
        
    }
}
