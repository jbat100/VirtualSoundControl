
#include "VSCOBOSXSceneCollisionListener.h"

void VSC::OB::OSXSceneCollisionListener::collisionProspectDetected(Collision::SPtr collision)
{
    [mTarget collisionProspectDetected:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionProspectUpdated(Collision::SPtr collision)
{
    [mTarget collisionProspectUpdated:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionProspectEnded(Collision::SPtr collision)
{
    [mTarget collisionProspectEnded:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionDetected(Collision::SPtr collision)
{
    [mTarget collisionDetected:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionUpdated(Collision::SPtr collision)
{
    [mTarget collisionUpdated:collision];
}

void VSC::OB::OSXSceneCollisionListener::collisionEnded(Collision::SPtr collision)
{
    [mTarget collisionEnded:collision];
}
