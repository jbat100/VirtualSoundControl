
#include "VSCOBDisplay.h"
#include "VSCOB.h"
#include "VSCOBScene.h"

#include <Ogre/Ogre.h>

void VSC::OB::Display::Display() :
mWindow(0),
mCamera(0)
{
    
}

void VSC::OB::Display::createCamera(void)
{
    // Create the camera
    this->setCamera( this->getScene()->getSceneManager()->createCamera("Camera") );
    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,500));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
}

bool VSC::OB::Display::resetCameraAspectRatio(void)
{
    Ogre::Viewport* vp = mCamera->getViewport();
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    return true;
}


// ------------------------------------------------------------------------- 
bool VSC::OB::Display::checkSpaceInFront(Float minDist)
{
    Ogre::Vector3 pickPos;
    Ogre::Ray rayTo;
    Ogre::Vector2 coord(0.5, 0.5);
    
    Scene::Ptr scene = this->getScene();
    
    BOOST_ASSERT(scene);
    
    if (scene) {
        Scene::Element::SPtr element = scene->getElementAtDisplayCoordinate(mCamera->getViewport(), coord, pickPos, rayTo);
        if (element)
        {
            OgreBulletDynamics::RigidBody *body = element->getRigidBody();
            if (body)
            {
                if ((pickPos - mCamera->getDerivedPosition ()).length () < minDist) return false;
            }
        }
        return true;
    }
    
    return false;
}




// -------------------------------------------------------------------------
const VSC::OB::Display::StatsMap& VSC::OB::Display::getUpdatedStatsMap(void)
{
    const Ogre::RenderTarget::FrameStats& stats = mRenderWindow->getStatistics();
    
    mStatsMap["Average FPS"]    = Ogre::StringConverter::toString(stats.avgFPS);
    mStatsMap["Current FPS"]    = Ogre::StringConverter::toString(stats.lastFPS);
    mStatsMap["Triangle Count"] = Ogre::StringConverter::toString(stats.triangleCount);
    
    return mStatsMap;
}

