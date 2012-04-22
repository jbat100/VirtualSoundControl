/*
 *  VSCConstraintDemo.h
 *  BulletCocoaTest
 *
 *  Created by Jonathan Thorpe on 16/06/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_CONSTRAINT_APPLICATION_H_
#define _VSC_CONSTRAINT_APPLICATION_H_

#include "VSCRootApplication.h"
#include <LinearMath/btAlignedObjectArray.h>

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;

///VSCBaseApplication is good starting point for learning the code base and porting.

class VSCConstraintApplication : public VSCRootApplication
{
	
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btBroadphaseInterface* m_overlappingPairCache;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_constraintSolver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	
	void setupEmptyDynamicsWorld();
	
public:
	
	VSCConstraintApplication() {} // superclass constructor should be called automatically
    ~VSCConstraintApplication() { exitPhysics(); } // superclass destructor should be called automatically
	
	void	initPhysics();
	void	exitPhysics();
	
	virtual void clientResetScene();
	virtual void swapBuffers();
	
	virtual void display(void);
	virtual void simulate();
	
	static VSCConstraintApplication* Create()
	{
		VSCConstraintApplication* demo = new VSCConstraintApplication();
		demo->myinit();
		demo->initPhysics();
		return demo;
	}
	
	float m_Time;
	class btConeTwistConstraint* m_ctc;
	
};

#endif //CONSTRAINT_DEMO_H