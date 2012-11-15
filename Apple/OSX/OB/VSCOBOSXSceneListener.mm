
#import "VSCOBOSXSceneDisplayView.h"

void VSC::OB::OSXSceneListener::sceneRegisteredElement(Scene::SPtr scene, Scene::Element::SPtr element)
{
    if (mTarget && [mTarget respondsToSelector:@selector(scene:registeredElement:)])
    {
        [mTarget scene:scene registeredElement:element];
    }
}

