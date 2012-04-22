/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef _VSC_HACKDAY_APPLICATION_H_
#define _VSC_HACKDAY_APPLICATION_H_

#include "VSCRootApplication.h"

#include "VSCCollision.h"

#include <LinearMath/btAlignedObjectArray.h>
#include <vector>
#include <set>
#include <list>

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;

///VSCBaseApplication is good starting point for learning the code base and porting.

class VSCHackdayApplication : public VSCRootApplication
{

public:

	VSCHackdayApplication() {} // superclass constructor should be called automatically
    ~VSCHackdayApplication() { exitPhysics();} // superclass destructor should be called automatically
	
	void initPhysics();
	void exitPhysics();

	void clientResetScene();
	void swapBuffers();
    void collisionCheck();
    void simulate();

	static VSCRootApplication* Create()
	{
		VSCHackdayApplication* demo = new VSCHackdayApplication();
		demo->myinit();
		demo->initPhysics();
		return demo;
	}
    
private:
    
    //keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface* m_broadphase;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver*	m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
    
    //VSCCollisionTracker _collsisionTracker;
    
    std::list<btPersistentManifold*> _currentManifolds;
	
};

#endif //BASIC_DEMO_H

