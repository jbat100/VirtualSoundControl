
#ifndef _VSC_OB_OSX_SCENE_LISTENER_H_
#define _VSC_OB_OSX_SCENE_LISTENER_H_

#import <Foundation/Foundation.h>

#include "VSCOB.h"
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

@protocol VSCOBOSXSceneListenerTarget <NSObject>

@optional

-(void) scene:(VSC::OB::Scene_SPtr)scene registeredElement:(VSC::OB::Element_SPtr)element;
-(void) scene:(VSC::OB::Scene_SPtr)scene changedSetting:(VSC::OB::Scene::Setting)setting toValue:(BOOL)value;
-(void) sceneWasRendered:(VSC::OB::Scene_SPtr)scene;

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
            
            virtual void sceneRegisteredElement(Scene_SPtr scene, Element_SPtr element);
            virtual void sceneChangedSetting(Scene_SPtr scene, Scene::Setting setting, bool value);
            virtual void sceneWasRendered(Scene_SPtr scene);
            
        private:
            
            id<VSCOBOSXSceneListenerTarget> mTarget;
            
        };
        
    }
}

#endif // _VSC_OB_OSX_SCENE_LISTENER_H_
