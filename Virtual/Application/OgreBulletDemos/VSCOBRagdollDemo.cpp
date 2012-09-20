/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/


#include "VSCOBRagdollDemo.h"
#include "VSCOBApplication.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "VSCOBInputAdapter.h"

using namespace OIS;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

// -------------------------------------------------------------------------
const String ragdollMeshNames[] = {
	"zombie_small.mesh",
	"zombie_small.mesh",
	"ninja.mesh",
	"robot.mesh",
	"GingerBreadMan.mesh"
};
const String ragdollAnimation [] = {
	"Walk1",
	"Walk1",
	"Walk",
	"Walk",
	"Walk1"
};

size_t sSelectedMesh = 1;

#ifdef _DEBUG
const size_t sMaxRagdoll = 10;
#else //_DEBUG
const size_t sMaxRagdoll = 30;
#endif //_DEBUG
// -------------------------------------------------------------------------
const Ogre::Vector3    CameraStart            = Ogre::Vector3(0,-9,1);
// -------------------------------------------------------------------------

typedef std::list<Entity*> RagDollList;

struct ragDollData
{
	OgreBulletDynamics::RagDoll *ragdoll;
	size_t                      sSelectedMesh;
	Ogre::Real                  timeSinceBirth;
	Ogre::Real                  timeSincePhysical;
};

// -------------------------------------------------------------------------
RagDollList     myRagDolls;
Real            _animation_speed;
Entity          *_gun;
SceneNode       *_gun_node;
Overlay         *_over;
Real            _shot_time;
Real            _last_ragdoll;
int             _ragdoll_count;
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void VSC::OB::RagdollDemo::init(Ogre::Root *root, Ogre::RenderWindow *win, VSC::OB::Application *application)
{
    mHelpKeys.clear();

    // ------------------------
    // Start OgreScene
    mSceneMgr = root->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("Cam");
    //mCamera->setFOVy(Degree(90));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);
    
    Viewport *vp = win->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    mCamera->setPosition(CameraStart);
    mCamera->rotate(Ogre::Vector3(1,0,0), Degree(90));
    mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);

    VSC::OB::Scene::init(root, win, application);

    // ------------------------
    // add lights
    setBasicLight();
    
    // ------------------------
    // Add the Gui
    setPhysicGUI();
    // ------------------------
    // Start Bullet
    initWorld();

    // ------------------------
    // Add the ground
    addGround();
}

// -------------------------------------------------------------------------
bool VSC::OB::RagdollDemo::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
	const float trowDist = 2.0f;
    bool handled = true;
    
    OIS::Keyboard::Modifier modifier = this->getInputAdapter()->getCurrentModifier();
    VSC::Keyboard::Combination comb(key, modifier);
    
    const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
    
    BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet)
    {
    
        switch(key)
        {
            case KC_X:
                shootToKill();
                break;	
            default:
                handled = false;
                break;
        }
    
        if (!handled) handled = VSC::OB::Scene::throwDynamicObject(actionKey);
        if (!handled) handled = VSC::OB::Scene::dropDynamicObject(actionKey);
    }
    
    if (handled) return true;

    return VSC::OB::Scene::keyPressed(renderWindow, key);
}
// -------------------------------------------------------------------------
void VSC::OB::RagdollDemo::button0Pressed()
{
	shootToKill();
}
// -------------------------------------------------------------------------
bool VSC::OB::RagdollDemo::frameStarted(Ogre::Real elapsedTime)
{
	_last_ragdoll += elapsedTime;

#ifdef _RAGDOLL_FINISHED
	_gun_node->setOrientation(mCamera->getOrientation());
	_gun_node->setPosition(mCamera->getPosition() + (mCamera->getOrientation() * Ogre::Vector3(0.3,-0.15,-1.1)));
	for ( RagDollList::iterator iRag = myRagDolls.begin();
		 iRag != myRagDolls.end(); 
		 ++iRag)
	{   
		ragDollData *b = any_cast<ragDollData *> ((*iRag)->getUserAny());
		if (b && b->ragdoll)
		{
			if (b->ragdoll->isActive())
			{

				b->timeSincePhysical += time;
				b->ragdoll->update();
			}
		}
		else
		{
			(*iRag)->getAnimationState(ragdollAnimation[b->sSelectedMesh])->addTime(time * _animation_speed);

			(*iRag)->getParentNode ()->translate(
				(*iRag)->getParentNode ()->getOrientation() * (Ogre::Vector3::UNIT_Z * time * 2.5));

		}
	}
#endif

	return VSC::OB::Scene::frameStarted(elapsedTime);;
}
// -------------------------------------------------------------------------
bool VSC::OB::RagdollDemo::frameEnded(Ogre::Real elapsedTime)
{
#ifdef _RAGDOLL_FINISHED
	_shot_time -= time;

	for (RagDollList::iterator i = myRagDolls.begin(); i != myRagDolls.end();)
	{
		ragDollData *b = any_cast<ragDollData *> ((*i)->getUserAny());

		if (b)
		{
			b->timeSinceBirth += time;

			// turn to stone to improve fps,
			// better way to do that would be to tweak
			// simulation parameters to be less jitty.
			// better auto sleep
			if (b->ragdoll
				&& b->ragdoll->isActive() 
				&& b->timeSincePhysical > 5.0)
			{
				b->ragdoll->setSleep();
			}


			if ( b->timeSinceBirth > sMaxRagdoll)
			{
				delete b->ragdoll;
				delete b;

				(static_cast<SceneNode*> ((*i)->getParentNode ()->getParent()))->removeAndDestroyChild(
					(*i)->getParentNode ()->getName ());
				mSceneMgr->destroyMovableObject((*i)->getName(), (*i)->getMovableType());

				i = myRagDolls.erase(i);
			}
			else
			{
				++i;
			}
		}
		else
		{
			++i;
		}
	}
	if (_last_ragdoll > 5.0 && 
		myRagDolls.size() < static_cast<size_t>(sMaxRagdoll))
		createRagDoll();
#endif
	return VSC::OB::Scene::frameEnded(elapsedTime);;
}

// -------------------------------------------------------------------------
void VSC::OB::RagdollDemo::shootToKill()
{
#ifdef _RAGDOLL_FINISHED
	if (_shot_time <= 0.0)
	{
		Ray pickRay = mCamera->getCameraToViewportRay(0.5, 0.5);
		_shot_time = 0.2;

		mRayQuery->setRay (pickRay);
		const RaySceneQueryResult& result = mRayQuery->execute();
		if (!result.empty())
		{
			RaySceneQueryResult::const_iterator i = result.begin();

			//mRayQuery->setSortByDistance (true, 1);//only one hit
			//myBulletRayRay->setDefinition(pickRay.getOrigin(), pickRay.getDirection());
			//myBulletRayRay->enable ();

			while((i != result.end()))
			{
				_last_ragdoll = 5.0;

				Entity *e = i->movable;
				ragDollData * const b = any_cast<ragDollData *> (e->getUserAny());

				// uncomment to see nice debug mesh of animated mesh
				//#define _DEBUG_ZOMBIE_TRIMESH
#ifdef _DEBUG_TRIMESH 
				// ray cast could be tested against that instead of ragdoll.
				// depending on complexity of mesh could be simpler
				VSC::OB::Scene::AnimatedMeshToShapeConverter meshconverter(e, e->getParentNode ()->_getFullTransform());
				//_geoms.push_back(meshconverter.createStaticTriangleMesh(_world, _space));
#else //_DEBUG_TRIMESH
				bool wasPhysical = b && b->ragdoll && b->ragdoll->isCollisionEnabled();
				if (!wasPhysical)
				{
					e->getAnimationState(ragdollAnimation[b->sSelectedMesh])->setEnabled(false);
					// Create the ragdoll
					b->ragdoll->collisionEnable(true);
					//e->setSelfCollisions(true);
				}

				OgreBulletDynamics::RigidBody *hit_body;
				Ogre::Vector3 hit_point;

				bool is_hit = false;
				if (e->pickRagdoll(myBulletRay, hit_body, hit_point))
				{
					if (hit_body)
					{
						hit_body->addForceAt(pickRay.getDirection() * 250000, hit_point);
						is_hit = true;
						ragDollData *b = any_cast<ragDollData *> (e->getUserAny());
						b->timeSincePhysical = 0.0f;
					}
				}

				if (!is_hit&& !wasPhysical)
				{
					ragDollData *b = any_cast<ragDollData *> (e->getUserAny());
					if (b  && b->ragdoll)
						b->ragdoll->collisionEnable(false);

					e->getAnimationState(ragdollAnimation[b->sSelectedMesh])->setEnabled(true);
				}
#endif //_TESTTRIMESH	

				++i;
			}	// if results.	
		}
		//myBulletRay->disable ();
	}

#endif
}