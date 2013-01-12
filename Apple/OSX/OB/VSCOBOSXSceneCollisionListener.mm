
#include "VSCOBOSXSceneCollisionListener.h"

void VSC::OB::OSXSceneCollisionListener::collisionProspectDetected(Scene::Collision::SPtr collision)
{
    [mTarget collisionProspectDetected:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionProspectUpdated(Scene::Collision::SPtr collision)
{
    [mTarget collisionProspectUpdated:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionProspectEnded(Scene::Collision::SPtr collision)
{
    [mTarget collisionProspectEnded:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionDetected(Scene::Collision::SPtr collision)
{
    [mTarget collisionDetected:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionUpdated(Scene::Collision::SPtr collision)
{
    [mTarget collisionUpdated:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionEnded(Scene::Collision::SPtr collision)
{
    [mTarget collisionEnded:collision];
}
