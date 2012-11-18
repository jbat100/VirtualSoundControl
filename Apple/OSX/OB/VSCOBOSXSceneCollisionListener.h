
#ifndef _VSC_OB_OSX_SCENE_COLLISION_LISTENER_H_
#define _VSC_OB_OSX_SCENE_COLLISION_LISTENER_H_

#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

@protocol VSCOBOSXSceneCollisionListenerTarget <NSObject>

@optional

-(void) scene:(VSC::OB::Scene::SPtr)scene collisionProspectDetected:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) scene:(VSC::OB::Scene::SPtr)scene collisionProspectUpdated:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) scene:(VSC::OB::Scene::SPtr)scene collisionProspectEnded:(VSC::OB::Scene::Collision::SPtr)collision;

-(void) scene:(VSC::OB::Scene::SPtr)scene collisionDetected:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) scene:(VSC::OB::Scene::SPtr)scene collisionUpdated:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) scene:(VSC::OB::Scene::SPtr)scene collisionEnded:(VSC::OB::Scene::Collision::SPtr)collision;

@end


namespace VSC
{
    namespace OB
    {
        
        class OSXSceneCollisionListener : public Scene::CollisionDetector::Listener
        {
            
        public:
            
            typedef boost::shared_ptr<OSXSceneCollisionListener>    SPtr;
            typedef boost::weak_ptr<OSXSceneCollisionListener>      WPtr;
            
            id<VSCOBOSXSceneCollisionListenerTarget> getTarget(void) {return mTarget;}
            void setTarget(id<VSCOBOSXSceneCollisionListenerTarget> target) {mTarget = target;}
            
            /*
             *  Scene::Listener overrides
             */
            
            virtual void collisionProspectDetected(Scene::Collision::SPtr collision) {}
            virtual void collisionProspectUpdated(Scene::Collision::SPtr collision) {}
            virtual void collisionProspectEnded(Scene::Collision::SPtr collision) {}
            
            virtual void collisionDetected(Scene::Collision::SPtr collision) {}
            virtual void collisionUpdated(Scene::Collision::SPtr collision) {}
            virtual void collisionEnded(Scene::Collision::SPtr collision) {}
            
        private:
            
            id<VSCOBOSXSceneCollisionListenerTarget> mTarget;
            
        };
        
    }
}

#endif // _VSC_OB_OSX_SCENE_LISTENER_H_
