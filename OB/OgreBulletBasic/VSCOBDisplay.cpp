
#include "VSCOBDisplay.h"
#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBApplication.h"
#include "VSCException.h"

#include <Ogre/Ogre.h>

const Ogre::Vector3 CameraStart = Ogre::Vector3(13,4.5,0);

VSC::OB::Display::Display() :
mRenderWindow(0),
mCamera(0)
{

}

VSC::OB::Display::~Display()
{
    this->destroyRenderWindow();
}

void VSC::OB::Display::setupWithScene(Scene::SPtr scene)
{
    this->shutdown();
    
    if (!scene) throw VSCInvalidArgumentException("Excpected non-NULL scene");
    mScene = Scene::WPtr(scene);
    
    BOOST_ASSERT(mRenderWindow);
    this->createCamera();
    
    BOOST_ASSERT(mCamera);
    this->createViewport();
    
    this->resetCameraAspectRatio();
    
    this->internalSetup();
}

void VSC::OB::Display::shutdown()
{
    this->destroyCamera();
    this->internalShutdown();
}

void VSC::OB::Display::createCamera(void)
{
    // Create the camera
    this->setCamera( this->getScene()->getSceneManager()->createCamera("Camera") );
    
    // Position it at 500 in Z direction
    //mCamera->setPosition(Ogre::Vector3(0,0,500));
    
    mCamera->setPosition(CameraStart);
    
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
    
}

void VSC::OB::Display::createViewport(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* viewport = this->getRenderWindow()->addViewport(this->getCamera());
    viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
}

void VSC::OB::Display::destroyCamera(void)
{
    /*
     *  Shutdown can be called when there is no 
     */
    
    //BOOST_ASSERT(this->getScene()->getSceneManager());
    //BOOST_ASSERT(this->getCamera());
    
    if (this->getScene()->getSceneManager() && this->getCamera())
    {
        this->getScene()->getSceneManager()->destroyCamera(this->getCamera());
    }
    
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

