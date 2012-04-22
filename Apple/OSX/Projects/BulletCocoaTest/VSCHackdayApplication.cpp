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



///scaling of the objects (0.1 = 20 centimeter boxes )
#define SCALING 1.
#define START_POS_X -5
#define START_POS_Y -5
#define START_POS_Z -3

#define ARRAY_SIZE_X 2
#define ARRAY_SIZE_Y 2
#define ARRAY_SIZE_Z 1

#include "VSCHackdayApplication.h"
//#include "GlutStuff.h"
#include <GLUT/GLUT.h>
///btBulletDynamicsCommon.h is the main Bullet include file, contains most common include files.
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <stdio.h> //printf debugging
#include <iostream>


struct ContactCallback : public btCollisionWorld::ContactResultCallback
{
	virtual	btScalar addSingleResult(btManifoldPoint& cp,	
                                     const btCollisionObject* colObj0,
                                     int partId0,
                                     int index0,
                                     const btCollisionObject* colObj1,
                                     int partId1,
                                     int index1)
	{
        
        std::cout << "COLLISION!!! Between " << colObj0 << " and " << colObj1 << std::endl;
        
        
        
        /*
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
        
		btVector3 ptA = cp.getPositionWorldOnA();
		btVector3 ptB = cp.getPositionWorldOnB();
        
		glVertex3d(ptA.x(),ptA.y(),ptA.z());
		glVertex3d(ptB.x(),ptB.y(),ptB.z());
		glEnd();
         */
        
		return 0;
	}
};

void VSCHackdayApplication::swapBuffers()
{
	glutSwapBuffers();
}

void VSCHackdayApplication::initPhysics()
{
	setTexturing(true);
	setShadows(true);

	setCameraDistance(btScalar(SCALING*50.));

	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	
	m_dynamicsWorld->setGravity(btVector3(0,-10,0));

	///create a few basic rigid bodies
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
//	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);
	
	m_collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-50,0));

	//We can also use VSCRootApplication::localCreateRigidBody, but for clarity it is provided here:
	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody(body);
	}


	{
		// create a few dynamic rigidbodies
		// Re-using the same collision is better for memory usage and performance

		btCollisionShape* colShape = new btBoxShape(btVector3(SCALING*1,SCALING*1,SCALING*1));
		//btCollisionShape* colShape = new btSphereShape(btScalar(SCALING));
		m_collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass,localInertia);

		float start_x = START_POS_X - ARRAY_SIZE_X/2;
		float start_y = START_POS_Y;
		float start_z = START_POS_Z - ARRAY_SIZE_Z/2;

		for (int k=0;k<ARRAY_SIZE_Y;k++)
		{
			for (int i=0;i<ARRAY_SIZE_X;i++)
			{
				for(int j = 0;j<ARRAY_SIZE_Z;j++)
				{
					startTransform.setOrigin(SCALING*btVector3(
										btScalar(2.0*i + start_x),
										btScalar(20+2.0*k + start_y),
										btScalar(2.0*j + start_z)));

			
					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
					btRigidBody* body = new btRigidBody(rbInfo);
                    
					m_dynamicsWorld->addRigidBody(body);
				}
			}
		}
	}


}

void VSCHackdayApplication::clientResetScene()
{
	exitPhysics();
	initPhysics();
}	
	

void VSCHackdayApplication::exitPhysics()
{

	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	m_collisionShapes.clear();

	delete m_dynamicsWorld;
	
	delete m_solver;
	
	delete m_broadphase;
	
	delete m_dispatcher;

	delete m_collisionConfiguration;
	
}

void VSCHackdayApplication::simulate() 
{
    VSCRootApplication::simulate();
    
    this->collisionCheck();
    
}

void VSCHackdayApplication::collisionCheck () 
{
    
    // http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Callbacks_and_Triggers
    
    std::list<btPersistentManifold*> localManifolds;
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        
		//btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		//btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
        
        localManifolds.push_back(contactManifold);        
        
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
            btVector3 ptA = pt.getPositionWorldOnA();
			btVector3 ptB = pt.getPositionWorldOnB();
		}
        
		//you can un-comment out this line, and then all points are removed
		contactManifold->clearManifold();	
	}
     
    std::vector<btPersistentManifold*>::iterator diffIt;
    
    std::vector<btPersistentManifold*> newManifolds (localManifolds.size());
    diffIt = std::set_difference(localManifolds.begin(), localManifolds.end(), _currentManifolds.begin(), _currentManifolds.end(), 
                                 newManifolds.begin());
    for (std::vector<btPersistentManifold*>::iterator it = newManifolds.begin(); it != diffIt; it++) {
        std::cout << "COLLISION START " << *it << std::endl;
    }
    
    std::vector<btPersistentManifold*> oldManifolds (_currentManifolds.size());
    diffIt = std::set_difference(_currentManifolds.begin(), _currentManifolds.end(), localManifolds.begin(), localManifolds.end(), 
                                 oldManifolds.begin());
    for (std::vector<btPersistentManifold*>::iterator it = oldManifolds.begin(); it != diffIt; it++) {
        std::cout << "COLLISION END " << *it << std::endl;
    }
    
    _currentManifolds = localManifolds;
    
}




