
#include "VSCOBDisplay.h"
#include "VSCOB.h"
#include "VSCOBScene.h"
#include "VSCOBApplication.h"
#include "VSCException.h"

#include <Ogre/Ogre.h>

static const Ogre::Vector3 CameraStartPos = Ogre::Vector3(30.0, 10.0, 0.0);
static const Ogre::Vector3 CameraStartLookAt = Ogre::Vector3(0.0, 0.0, 0.0);

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
    
    mCamera->setPosition(CameraStartPos);
    mCamera->lookAt(CameraStartLookAt);
    
    mCamera->setNearClipDistance(1.0);
    
}

void VSC::OB::Display::createViewport(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* viewport = this->getRenderWindow()->addViewport(this->getCamera());
    viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
}

Ogre::Vector2 VSC::OB::Display::normalizedViewportCoordinates(const Ogre::Vector2& absCoord)
{
    Ogre::Viewport* v = this->getCamera()->getViewport();
    return Ogre::Vector2(absCoord.x / (float) v->getActualWidth(), 1.0 - (absCoord.y / (float) v->getActualHeight()));
}

void VSC::OB::Display::destroyCamera(void)
{
    /*
     *  Shutdown can be called when there is no camera/scene manager so no asserts
     */
    
    if (this->getScene() && this->getScene()->getSceneManager() && this->getCamera())
    {
        this->getScene()->getSceneManager()->destroyCamera(this->getCamera());
    }
    else if (this->getCamera() && (!this->getScene() || !this->getScene()->getSceneManager()))
    {
        // THIS SHOULD NOT HAPPEN... LEAAAK
        BOOST_ASSERT(0);
    }
    
    this->setCamera(0);
}

void VSC::OB::Display::destroyRenderWindow(void)
{
    if (this->getRenderWindow())
    {
        std::cout << "VSC::OB::Display::destroyRenderWindow destroying " << this->getRenderWindow() << std::endl;
        std::cout << "VSC::OB::Application::singletonApplication().get ";
        std::cout << VSC::OB::Application::singletonApplication().get() << std::endl;
        std::cout << "VSC::OB::Application::singletonApplication()->getOgreRoot() ";
        std::cout << VSC::OB::Application::singletonApplication()->getOgreRoot() << std::endl;
        
        VSC::OB::Application::singletonApplication()->getOgreRoot()->destroyRenderTarget(this->getRenderWindow());
    }
    
    this->setRenderWindow(0);
}

bool VSC::OB::Display::resetCameraAspectRatio(void)
{
    //BOOST_ASSERT(this->getCamera());
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

