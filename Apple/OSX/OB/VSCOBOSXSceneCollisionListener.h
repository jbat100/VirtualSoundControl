
#ifndef _VSC_OB_OSX_SCENE_COLLISION_LISTENER_H_
#define _VSC_OB_OSX_SCENE_COLLISION_LISTENER_H_

#import <Foundation/Foundation.h>

#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBCollisionDetector.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

@protocol VSCOBOSXSceneCollisionListenerTarget <NSObject>

@optional

-(void) collisionProspectDetected:(VSC::OB::Collision_SPtr)collision;
-(void) collisionProspectUpdated:(VSC::OB::Collision_SPtr)collision;
-(void) collisionProspectEnded:(VSC::OB::Collision_SPtr)collision;

-(void) collisionDetected:(VSC::OB::Collision_SPtr)collision;
-(void) collisionUpdated:(VSC::OB::Collision_SPtr)collision;
-(void) collisionEnded:(VSC::OB::Collision_SPtr)collision;

@end


namespace VSC
{
    namespace OB
    {
        
        class OSXSceneCollisionListener : public CollisionDetector::Listener
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
            
            virtual void collisionProspectDetected(Collision_SPtr collision);
            virtual void collisionProspectUpdated(Collision_SPtr collision);
            virtual void collisionProspectEnded(Collision_SPtr collision);
            
            virtual void collisionDetected(Collision_SPtr collision);
            virtual void collisionUpdated(Collision_SPtr collision);
            virtual void collisionEnded(Collision_SPtr collision);
            
        private:
            
            id<VSCOBOSXSceneCollisionListenerTarget> mTarget;
            
        };
        
    }
}

#endif // _VSC_OB_OSX_SCENE_LISTENER_H_
