
#include "VSCOBDisplay.h"
#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBApplication.h"
#include "VSCException.h"

#include <Ogre/Ogre.h>

VSC::OB::Display::Display(Scene::SPtr scene) :
mRenderWindow(0),
mCamera(0)
{
    if (!scene) throw VSCInvalidArgumentException("Excpected non-NULL scene");
    mScene = Scene::WPtr(scene);
}

void VSC::OB::Display::init()
{
    this->createCamera();
}

void VSC::OB::Display::shutdown()
{
    this->destroyCamera();
    this->destroyRenderWindow();
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

void VSC::OB::Display::destroyCamera(void)
{
    BOOST_ASSERT(this->getScene()->getSceneManager());
    BOOST_ASSERT(this->getCamera());
    
    this->getScene()->getSceneManager()->destroyCamera(this->getCamera());
    
    this->setCamera(0);
}

void VSC::OB::Display::destroyRenderWindow(void)
{
    BOOST_ASSERT(this->getScene()->getSceneManager());
    BOOST_ASSERT(this->getRenderWindow());
    
    this->getScene()->getApplication()->getOgreRoot()->destroyRenderTarget(this->getRenderWindow());
    
    this->setRenderWindow(0);
}

bool VSC::OB::Display::resetCameraAspectRatio(void)
{
    BOOST_ASSERT(this->getCamera());
    if (!this->getCamera()) return false;
    
    Ogre::Viewport* vp = this->getCamera()->getViewport();
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    return true;
}


// ------------------------------------------------------------------------- 
bool VSC::OB::Display::checkSpaceInFront(Float minDist)
{
    Ogre::Vector3 pickPos;
    Ogre::Ray rayTo;
    Ogre::Vector2 coord(0.5, 0.5);
    
    Scene::SPtr scene = this->getScene();
    
    BOOST_ASSERT(scene);
    
    if (scene) {
        Scene::Element::SPtr element = scene->getElementAtDisplayCoordinate(shared_from_this(), coord, pickPos, rayTo);
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

