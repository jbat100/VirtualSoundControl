
#ifndef _VSC_OB_OSX_SCENE_LISTENER_H_
#define _VSC_OB_OSX_SCENE_LISTENER_H_

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
            
            id<VSCOBOSXSceneListenerTarget> getTarget(void) {return mTarget;}
            void setTarget(id<VSCOBOSXSceneListenerTarget> target) {mTarget = target;}
            
            /*
             *  Scene::Listener overrides
             */
            
            virtual void sceneRegisteredElement(Scene::SPtr scene, Scene::Element::SPtr element); 
            
        private:
            
            id<VSCOBOSXSceneListenerTarget> mTarget;
            
        };
        
    }
}

#endif // _VSC_OB_OSX_SCENE_LISTENER_H_
