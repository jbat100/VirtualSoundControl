
#import "VSCOBOSXSceneListener.h"

#include "VSCOBElement.h"

void VSC::OB::OSXSceneListener::sceneRegisteredElement(Scene::SPtr scene, Element::SPtr element)
{
    if (mTarget && [mTarget respondsToSelector:@selector(scene:registeredElement:)])
    {
        [mTarget scene:scene registeredElement:element];
    }
}

void VSC::OB::OSXSceneListener::sceneChangedSetting(Scene::SPtr scene, Scene::Setting setting, bool value)
{
    if (mTarget && [mTarget respondsToSelector:@selector(scene:changedSetting:toValue:)])
    {
        [mTarget scene:scene changedSetting:setting toValue:value];
    }
}

void VSC::OB::OSXSceneListener::sceneWasRendered(Scene::SPtr scene)
{
    if (mTarget && [mTarget respondsToSelector:@selector(sceneWasRendered:)])
    {
        [mTarget sceneWasRendered:scene];
    }
}
