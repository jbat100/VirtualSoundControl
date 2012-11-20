
#ifndef _VSC_OB_OSX_SCENE_COLLISION_LISTENER_H_
#define _VSC_OB_OSX_SCENE_COLLISION_LISTENER_H_

#import <Foundation/Foundation.h>

#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

@protocol VSCOBOSXSceneCollisionListenerTarget <NSObject>

@optional

-(void) collisionProspectDetected:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) collisionProspectUpdated:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) collisionProspectEnded:(VSC::OB::Scene::Collision::SPtr)collision;

-(void) collisionDetected:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) collisionUpdated:(VSC::OB::Scene::Collision::SPtr)collision;
-(void) collisionEnded:(VSC::OB::Scene::Collision::SPtr)collision;

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
            
            OSXSceneCollisionListener() : mTarget(nil) {}
            virtual ~OSXSceneCollisionListener() {}
            
            id<VSCOBOSXSceneCollisionListenerTarget> getTarget(void) {return mTarget;}
            void setTarget(id<VSCOBOSXSceneCollisionListenerTarget> target) {mTarget = target;}
            
            /*
             *  Scene::Listener overrides
             */
            
            virtual void collisionProspectDetected(Scene::Collision::SPtr collision);
            virtual void collisionProspectUpdated(Scene::Collision::SPtr collision);
            virtual void collisionProspectEnded(Scene::Collision::SPtr collision);
            
            virtual void collisionDetected(Scene::Collision::SPtr collision);
            virtual void collisionUpdated(Scene::Collision::SPtr collision);
            virtual void collisionEnded(Scene::Collision::SPtr collision);
            
        private:
            
            id<VSCOBOSXSceneCollisionListenerTarget> mTarget;
            
        };
        
    }
}

#endif // _VSC_OB_OSX_SCENE_LISTENER_H_
