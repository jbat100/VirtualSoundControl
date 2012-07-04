/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#ifndef _VSC_OGRE_BULLET_TERRAIN_DEMO_H_
#define _VSC_OGRE_BULLET_TERRAIN_DEMO_H_

#include "OgreBulletDynamics.h"
#include "VSCOgreBulletListener.h"

class VSCOgreBulletApplication;

// -------------------------------------------------------------------------
class VSCOgreBulletTerrainDemo : public VSCOgreBulletListener 
{
public:

    VSCOgreBulletTerrainDemo() : VSCOgreBulletListener() {mName = "Terrain Test Scene";};
    virtual ~VSCOgreBulletTerrainDemo(){};

    void init(Ogre::Root *root, Ogre::RenderWindow *win, VSCOgreBulletApplication *application);

    void keyPressed(OIS::KeyCode key);
    void keyReleased(OIS::KeyCode key);

    bool frameStarted(Ogre::Real elapsedTime);

private:

	OgreBulletCollisions::HeightmapCollisionShape       *mTerrainShape;


    OgreBulletDynamics::WheeledRigidBody                *mCarChassis;
    OgreBulletDynamics::VehicleTuning                   *mTuning;
    OgreBulletDynamics::VehicleRayCaster                *mVehicleRayCaster;
    OgreBulletDynamics::RaycastVehicle                  *mVehicle;

    Ogre::Entity    *mChassis;
    Ogre::Entity    *mWheels[4];
    Ogre::SceneNode *mWheelNodes[4];


    int mWheelsEngine[4];
    int mWheelsEngineCount;
    int mWheelsSteerable[4];
    int mWheelsSteerableCount;

    float mEngineForce;
    float mSteering;

    int mWheelEngineStyle;
    int mWheelSteeringStyle;


    bool mSteeringLeft;
    bool mSteeringRight;
};


#endif //_VSC_OGRE_BULLET_TERRAIN_DEMO_H_