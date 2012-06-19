/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/***************************************************************************
 File modified for VSC project
 -----------------------------------------------------------------------------*/
/*
 VSCOgreBulletListener.h
-------------
A basic test framework that minimize code in each test scene listener.
*/

#ifndef _VSC_OGRE_BULLET_LISTENER_H_
#define _VSC_OGRE_BULLET_LISTENER_H_

#include "OgreBulletDynamics.h"
#include "VSCOgreBulletInputListener.h"
#include "VSCOgreBulletGuiListener.h"
#include "OIS.h"

#define BASIC_HELP_INFO0 "Use F1, F2, F3, F4"
#define BASIC_HELP_INFO1 "B,N,G,H to Throw a Cube, Sphere, Cone, Cylinder"
#define BASIC_HELP_INFO2 "J,K,I,U to Drop  a Cube, Sphere, Cone, Cylinder"
#define BASIC_HELP_INFO3 ""
#define BASIC_HELP_INFO4 "left click to drag"
#define BASIC_HELP_INFO5 "right to camera move"
#define BASIC_HELP_INFO6 "middle for impulse"

enum QueryFlags
{
	ANY_QUERY_MASK					= 1<<0,
	RAGDOLL_QUERY_MASK				= 1<<1,
	GEOMETRY_QUERY_MASK				= 1<<2,
	VEHICLE_QUERY_MASK				= 1<<3,
	STATIC_GEOMETRY_QUERY_MASK		= 1<<4
};

class VSCOgreBulletApplication;

/*
The base Test class, is also able to listen for collisions and thus change the contact properties
*/
class VSCOgreBulletListener 
{
public:

	/**--------------------------------------------------------------
     *  Constructor/Destructor/Initialization 
     */
    VSCOgreBulletListener();
    virtual ~VSCOgreBulletListener(){};
    virtual void init(Ogre::Root *root, Ogre::RenderWindow *win, VSCOgreBulletApplication *application);

    /**--------------------------------------------------------------
     *  Setup/Teardown 
     */
    virtual void shutdown();
    void setPhysicGUI();
    void setBasicLight();

    const OIS::KeyCode getNextKey() const {return mActivationKeyCode;};
    void setNextKey(OIS::KeyCode code){mActivationKeyCode = code;};

    /**
     *  MISC Info
     */
    const Ogre::String getName() {return mName;}
    void setInfoText();
    void setDebugText(const Ogre::String &debugText){mDebugText = debugText;}

    /**--------------------------------------------------------------
     *  Ogre Frame Listener Overrides 
     */
    virtual bool frameStarted(Ogre::Real elapsedTime);
    virtual bool frameEnded(Ogre::Real elapsedTime);

    bool checkIfEnoughPlaceToAddObject(float maxDist);
    void throwDynamicObject(OIS::KeyCode key);
    void dropDynamicObject(OIS::KeyCode key);

#ifdef VSC_ENABLE_OIS_INPUT_SYSTEM
    /**--------------------------------------------------------------
     *  GUI stuff seperate this to keep this class GUI Agnostic?
     */
    VSCOgreBulletInputListener *getInputListener(){return mInputListener;}
#endif

    virtual void mouseMoved(Ogre::Real x, Ogre::Real y, Ogre::Real relX, Ogre::Real relY);
    virtual void mouseButtonPressed(MouseButtonID buttonID);
    virtual void mouseButtonReleased(MouseButtonID buttonID);

    virtual void keyPressed(OIS::KeyCode key);
    virtual void keyReleased(OIS::KeyCode key);

    bool *getBoolActivator(){return &mActivationBool;}

protected:

    /**
     *  World initialization (non-virtual?)
     */
    void initWorld (const Ogre::Vector3 &gravityVector = Ogre::Vector3 (0,-9.81,0), 
                    const Ogre::AxisAlignedBox &bounds = Ogre::AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),
                                                                               Ogre::Vector3 (10000,  10000,  10000)));
    void addGround();

    /**
     *  Add objects to scene, no direct 
     *  QUESTION: (no mesh data for addStaticTrimesh?) why cast shadow only for static tri mesh
     */
    OgreBulletDynamics::RigidBody *addCube(const Ogre::String instanceName,
        const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
        const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
        const Ogre::Real bodyMass);
    OgreBulletDynamics::RigidBody *addCylinder(const Ogre::String instanceName,
        const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
        const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
        const Ogre::Real bodyMass);
    OgreBulletDynamics::RigidBody *addSphere(const Ogre::String instanceName,
        const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Real radius,
        const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
        const Ogre::Real bodyMass);
    OgreBulletDynamics::RigidBody *addCone(const Ogre::String instanceName,
        const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
        const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
        const Ogre::Real bodyMass);
    OgreBulletDynamics::RigidBody *addStaticTrimesh(const Ogre::String &instanceName,
        const Ogre::String &meshName, const Ogre::Vector3 &pos, const Ogre::Quaternion &q, 
        const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, const bool castShadow = true);
    OgreBulletDynamics::RigidBody *addStaticPlane( const Ogre::Real bodyRestitution, 
                                                   const Ogre::Real bodyFriction);
    
    
    
    /*
     *  Debug stuff
     */
    void getDebugLines();

    /**
     *  Mouse picking utils 
     */
    OgreBulletDynamics::RigidBody* getBodyUnderCursorUsingBullet(Ogre::Vector3 &intersectionPoint, Ogre::Ray &rayTo);
    OgreBulletDynamics::RigidBody* getBodyUnderCursorUsingOgre(Ogre::Vector3 &intersectionPoint, Ogre::Ray &rayTo);

protected:
    
    static size_t mNumEntitiesInstanced;
    
    Ogre::RenderWindow                  *mWindow;
    Ogre::Root                          *mRoot;
    Ogre::SceneManager                  *mSceneMgr;

    int                                 mCurrentShadowTechnique;
    Ogre::Light                         *mSunLight;
	Ogre::Light                         *mLight;
	Ogre::Light                         *mLight2;


    Ogre::Camera                        *mCamera;
    Ogre::Radian                        mCameraRotX;
    Ogre::Radian                        mCameraRotY;
    float                               mCameraMove;
    Ogre::Vector3                       mCameraTrans;

    OgreBulletDynamics::DynamicsWorld   *mWorld;
    VSCOgreBulletApplication            *mApplication;


    std::deque<Ogre::Entity *>                          mEntities;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

    bool                    mStatsOn;
    bool                    mQuit;
    bool                    mDoOnestep;

    float                   mShootSpeed;
    float                   mImpulseForce;
    bool                    mPaused;


    bool                    mWireFrame;
    bool                    mDrawAabb;
    bool                    mDrawFeaturesText;
    bool                    mDrawContactPoints;
    bool                    mNoDeactivation;
    bool                    mNoHelpText;
    bool                    mDrawText;
    bool                    mProfileTimings;
    bool                    mEnableSatComparison;
    bool                    mDisableBulletLCP;
    bool                    mEnableCCD;


   BetaGUI::StaticText*     mFpsStaticText;


   /**
    *   Mouse picking, drag and drop
    */
   OgreBulletDynamics::RigidBody                            *mPickedBody;
   OgreBulletDynamics::TypedConstraint                      *mPickConstraint;
   Ogre::Vector3                                            mOldPickingPos;
   Ogre::Vector3                                            mOldPickingDist;
   OgreBulletCollisions::CollisionClosestRayResultCallback  *mCollisionClosestRayResultCallback;

   OgreBulletCollisions::DebugLines    *mDebugRayLine;
   Ogre::RaySceneQuery                 *mRayQuery;
   
   OIS::KeyCode                      mActivationKeyCode;
   bool                                 mActivationBool;

#ifdef VSC_ENABLE_OIS_INPUT_SYSTEM
   VSCOgreBulletInputListener          *mInputListener;
   VSCOgreBulletGuiListener            *mGuiListener;
#endif
    
   Ogre::String                         mDebugText;

   Ogre::String                         mName;
   std::vector<Ogre::String>            mHelpKeys;
   void updateStats();
};

#endif //_VSC_OGRE_BULLET_LISTENER_H_

