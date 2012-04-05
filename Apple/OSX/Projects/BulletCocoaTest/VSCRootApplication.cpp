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


#include "VSCRootApplication.h"
#include "LinearMath/btIDebugDraw.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h" //picking
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h" //picking

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "GL_ShapeDrawer.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btSerializer.h"
#include "GLDebugFont.h"

#include <iostream>
#include <sstream>

#include "btVectorVSCAds.h"


static bool use6Dof = false;
extern bool gDisableDeactivation;
int numObjects = 0;
const int maxNumObjects = 16384;
btTransform startTransforms[maxNumObjects];
btCollisionShape* gShapePtr[maxNumObjects]; //1 rigidbody has 1 shape (no re-use of shapes)

#define USE_MOTIONSTATE 1
#define NUM_SPHERES_ON_DIAGONAL 9

#define SHOW_NUM_DEEP_PENETRATIONS 1
extern int gNumClampedCcdMotions;
#ifdef SHOW_NUM_DEEP_PENETRATIONS 
extern int gNumDeepPenetrationChecks;
extern int gNumSplitImpulseRecoveries;
extern int gNumGjkChecks;
extern int gNumAlignedAllocs;
extern int gNumAlignedFree;
extern int gTotalBytesAlignedAllocs;
#endif


#pragma mark --- Constructor

//Constructor with default values, see btIDebugDraw.h for modes

VSCRootApplication::VSCRootApplication() :

// default values

m_dynamicsWorld(0),
m_pickConstraint(0),
m_shootBoxShape(0),
m_debugMode(0),

m_ele(-25.f),
m_azi(-180.f),
m_cameraPosition(25.f,20.f,0.f),
m_cameraForward(1.f,0.f,0.f),
m_cameraUp(0.f,1.f,0.f),
m_cameraDistance(50.0), // only used for central centered control
m_cameraSpeed(0.5),
m_cameraMouseSensitivity(0.5),

m_scaleBottom(0.5f),
m_scaleFactor(2.f),
m_glutScreenWidth(0),
m_glutScreenHeight(0),
m_frustumZNear(1.f),
m_frustumZFar(10000.f),
m_ortho(0),

m_shootBoxInitialSpeed(40.f),
m_idle(false),
m_enableshadows(false),
m_sundirection(btVector3(1,-2,1)*1000),
m_defaultContactProcessingThreshold(BT_LARGE_FLOAT)

{
#ifndef BT_NO_PROFILE
	m_profileIterator = CProfileManager::Get_Iterator();
#endif //BT_NO_PROFILE

	m_shapeDrawer = new GL_ShapeDrawer ();
	m_shapeDrawer->enableTexture(true);
	m_enableshadows = false;
	
	m_controlSetup = new VSCControlSetup();
	m_controlSetup->setToDefault();
    
    m_movingForward = false;
    m_movingBackward = false;
    m_movingRight = false;
    m_movingLeft = false;
    m_movingUp = false;
    m_movingDown = false;
	
}

#pragma mark --- Destructor


VSCRootApplication::~VSCRootApplication()
{
#ifndef BT_NO_PROFILE
	CProfileManager::Release_Iterator(m_profileIterator);
#endif //BT_NO_PROFILE

	if (m_shootBoxShape)
		delete m_shootBoxShape;

	if (m_shapeDrawer)
		delete m_shapeDrawer;
	
	if (m_controlSetup)
		delete m_controlSetup;
}

#pragma mark --- Timing

btScalar VSCRootApplication::getDeltaTimeMicroseconds()
{
#ifdef USE_BT_CLOCK
	btScalar dt = (btScalar)m_clock.getTimeMicroseconds();
	m_clock.reset();
	return dt;
#else
	return btScalar(16666.);
#endif
}

#pragma mark --- Physics and Simulation

void VSCRootApplication::toggleIdle() {
	if (m_idle) {
		m_idle = false;
	}
	else {
		m_idle = true;
	}
}


void VSCRootApplication::setDebugMode(int mode)
{
	m_debugMode = mode;
	if (getDynamicsWorld() && getDynamicsWorld()->getDebugDrawer())
		getDynamicsWorld()->getDebugDrawer()->setDebugMode(mode);
}


void VSCRootApplication::simulate()
{
	
	if (!m_idle) {
		// simple dynamics world doesn't handle fixed-time-stepping
		float ms = getDeltaTimeMicroseconds();
		// step the simulation
		if (m_dynamicsWorld)
			m_dynamicsWorld->stepSimulation(ms / 1000000.f);
	}
	
}


void VSCRootApplication::display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	renderme();
	
	//optional but useful: debug drawing to detect problems
	if (m_dynamicsWorld)
		m_dynamicsWorld->debugDrawWorld();
	
	glFlush();
	swapBuffers();
}


#pragma mark --- World Objects

btRigidBody* VSCRootApplication::localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
	
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);
	
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);
	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);
#else
	btRigidBody* body = new btRigidBody(mass,0,shape,localInertia);	
	body->setWorldTransform(startTransform);
#endif//
	
	m_dynamicsWorld->addRigidBody(body);
	
	return body;
}

void VSCRootApplication::setShootBoxShape ()
{
	if (!m_shootBoxShape)
	{
		btBoxShape* box = new btBoxShape(btVector3(.5f,.5f,.5f));
		box->initializePolyhedralFeatures();
		m_shootBoxShape = box;
	}
}

void VSCRootApplication::shootBox(const btVector3& destination)
{

	if (m_dynamicsWorld)
	{
		
		float mass = 1.f;
		btTransform startTransform;
		startTransform.setIdentity();
		btVector3 camPos = getCameraPosition();
		startTransform.setOrigin(camPos);

		setShootBoxShape ();

		btRigidBody* body = this->localCreateRigidBody(mass, startTransform,m_shootBoxShape);
		body->setLinearFactor(btVector3(1,1,1));
		//body->setRestitution(1);

		btVector3 linVel(destination[0]-camPos[0],destination[1]-camPos[1],destination[2]-camPos[2]);
		linVel.normalize();
		linVel*=m_shootBoxInitialSpeed;

		body->getWorldTransform().setOrigin(camPos);
		body->getWorldTransform().setRotation(btQuaternion(0,0,0,1));
		body->setLinearVelocity(linVel);
		body->setAngularVelocity(btVector3(0,0,0));
		body->setCcdMotionThreshold(0.5);
		body->setCcdSweptSphereRadius(0.9f);
		
	}
}

#pragma mark --- Picking Methods

int gPickingConstraintId = 0;
btVector3 gOldPickingPos;
btVector3 gHitPos(-1,-1,-1);
float gOldPickingDist  = 0.f;
btRigidBody* pickedBody = 0;//for deactivation state

btVector3 VSCRootApplication::getRayTo(int x,int y)
{

	float top = 1.f;
	float bottom = -1.f;
	float nearPlane = 1.f;
	float tanFov = (top-bottom)*0.5f / nearPlane;
	float fov = btScalar(2.0) * btAtan(tanFov);

	btVector3 rayFrom = getCameraPosition();
	btVector3 rayForward = getCameraLook();
	rayForward.normalize();
	float farPlane = 10000.f;
	rayForward *= farPlane;

	btVector3 rightOffset;
	btVector3 vertical = m_cameraUp;

	btVector3 hor;
	hor = rayForward.cross(vertical);
	hor.normalize();
	vertical = hor.cross(rayForward);
	vertical.normalize();

	float tanfov = tanf(0.5f*fov);

	hor *= 2.f * farPlane * tanfov;
	vertical *= 2.f * farPlane * tanfov;

	btScalar aspect;
	
	if (m_glutScreenWidth > m_glutScreenHeight) 
	{
		aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;
		hor*=aspect;
	} 
    else 
	{
		aspect = m_glutScreenHeight / (btScalar)m_glutScreenWidth;
		vertical*=aspect;
	}

	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.f/float(m_glutScreenWidth);
	btVector3 dVert = vertical * 1.f/float(m_glutScreenHeight);

	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;
	return rayTo;
}

btScalar mousePickClamping = 30.f;

void VSCRootApplication::removePickingConstraint()
{
	if (m_pickConstraint && m_dynamicsWorld)
	{
		
		std::cout << "Removing picking constraint " << m_pickConstraint << std::endl;
		
		m_dynamicsWorld->removeConstraint(m_pickConstraint);
		delete m_pickConstraint;
		//printf("removed constraint %i",gPickingConstraintId);
		m_pickConstraint = 0;
		pickedBody->forceActivationState(ACTIVE_TAG);
		pickedBody->setDeactivationTime( 0.f );
		pickedBody = 0;
	}
}



#pragma mark --- Rendering

void VSCRootApplication::reshape(int w, int h) 
{
	GLDebugResetFont(w,h);
	
	m_glutScreenWidth = w;
	m_glutScreenHeight = h;
	
	glViewport(0, 0, w, h);
	
	updateCamera();
}


void VSCRootApplication::myinit(void)
{
	
	GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
	GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
	GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
	/*	light_position is NOT default value	*/
	GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
	GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glClearColor(btScalar(0.7),btScalar(0.7),btScalar(0.7),btScalar(0));
	
	//  glEnable(GL_CULL_FACE);
	//  glCullFace(GL_BACK);
}

void VSCRootApplication::overrideGLShapeDrawer (GL_ShapeDrawer* shapeDrawer)
{
	shapeDrawer->enableTexture (m_shapeDrawer->hasTextureEnabled());
	delete m_shapeDrawer;
	m_shapeDrawer = shapeDrawer;
}

//See http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
void VSCRootApplication::setOrthographicProjection() 
{

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, m_glutScreenWidth, 0, m_glutScreenHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(btScalar(0), btScalar(-m_glutScreenHeight), btScalar(0));

}

void VSCRootApplication::resetPerspectiveProjection() 
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	updateCamera();
}

extern CProfileIterator * m_profileIterator;

/*
void VSCRootApplication::displayProfileString(int xOffset,int yStart,char* message)
{
	glRasterPos3f(btScalar(xOffset),btScalar(yStart),btScalar(0));
	GLDebugDrawString(xOffset,yStart,message);
}


void VSCRootApplication::showProfileInfo(int& xOffset,int& yStart, int yIncr)
{
#ifndef BT_NO_PROFILE

	static double time_since_reset = 0.f;
	
	if (!m_idle)
	{
		time_since_reset = CProfileManager::Get_Time_Since_Reset();
	}


	{
		//recompute profiling data, and store profile strings

		char blockTime[128];

		double totalTime = 0;

		int frames_since_reset = CProfileManager::Get_Frame_Count_Since_Reset();

		m_profileIterator->First();

		double parent_time = m_profileIterator->Is_Root() ? time_since_reset : m_profileIterator->Get_Current_Parent_Total_Time();

		{
			sprintf(blockTime,"--- Profiling: %s (total running time: %.3f ms) ---",	m_profileIterator->Get_Current_Parent_Name(), parent_time );
			displayProfileString(xOffset,yStart,blockTime);
			yStart += yIncr;
			sprintf(blockTime,"press (1,2...) to display child timings, or 0 for parent" );
			displayProfileString(xOffset,yStart,blockTime);
			yStart += yIncr;

		}


		double accumulated_time = 0.f;

		for (int i = 0; !m_profileIterator->Is_Done(); m_profileIterator->Next())
		{
			double current_total_time = m_profileIterator->Get_Current_Total_Time();
			accumulated_time += current_total_time;
			double fraction = parent_time > SIMD_EPSILON ? (current_total_time / parent_time) * 100 : 0.f;

			sprintf(blockTime,"%d -- %s (%.2f %%) :: %.3f ms / frame (%d calls)",
				++i, m_profileIterator->Get_Current_Name(), fraction,
				(current_total_time / (double)frames_since_reset),m_profileIterator->Get_Current_Total_Calls());
			displayProfileString(xOffset,yStart,blockTime);
			yStart += yIncr;
			totalTime += current_total_time;
		}

		sprintf(blockTime,"%s (%.3f %%) :: %.3f ms", "Unaccounted",
			// (min(0, time_since_reset - totalTime) / time_since_reset) * 100);
			parent_time > SIMD_EPSILON ? ((parent_time - accumulated_time) / parent_time) * 100 : 0.f, parent_time - accumulated_time);

		displayProfileString(xOffset,yStart,blockTime);
		yStart += yIncr;



		sprintf(blockTime,"-------------------------------------------------");
		displayProfileString(xOffset,yStart,blockTime);
		yStart += yIncr;

	}
#endif//BT_NO_PROFILE

}
 
 */

std::string VSCRootApplication::profileInfoString () {
	
	static double time_since_reset = 0.f;
	
	if (!m_idle)
		time_since_reset = CProfileManager::Get_Time_Since_Reset();
		
	double totalTime = 0;
	
	std::stringstream profileStream;
	profileStream.precision(3);
	
	int frames_since_reset = CProfileManager::Get_Frame_Count_Since_Reset();
	
	m_profileIterator->First();
	
	double parent_time = m_profileIterator->Is_Root() ? time_since_reset : m_profileIterator->Get_Current_Parent_Total_Time();

	profileStream << "--- Profiling: " << std::string(m_profileIterator->Get_Current_Parent_Name()) << " (total running time: ";
	profileStream << parent_time << " ms) --- \n";

	double accumulated_time = 0.f;
	
	for (int i = 0; !m_profileIterator->Is_Done(); m_profileIterator->Next())
	{
		double current_total_time = m_profileIterator->Get_Current_Total_Time();
		accumulated_time += current_total_time;
		double fraction = parent_time > SIMD_EPSILON ? (current_total_time / parent_time) * 100 : 0.f;
		
		profileStream << i++ << " -- " << std::string(m_profileIterator->Get_Current_Name()) << " (";
		profileStream << fraction << "%) : " << (current_total_time / (double)frames_since_reset) << " ms / frame (";
		profileStream << m_profileIterator->Get_Current_Total_Calls() << " calls) \n";
		
		totalTime += current_total_time;
	}
	
	profileStream << "Unaccounted -- (" << (parent_time > SIMD_EPSILON ? ((parent_time - accumulated_time) / parent_time) * 100 : 0.f);
	profileStream << " %) : " << parent_time - accumulated_time << " ms" << std::endl;
	
	return profileStream.str();
	
}


//
void VSCRootApplication::renderscene(int pass)
{
	btScalar	m[16];
	btMatrix3x3	rot;
	rot.setIdentity();
	const int	numObjects=m_dynamicsWorld->getNumCollisionObjects();
	btVector3 wireColor(1,0,0);
	for(int i=0;i<numObjects;i++)
	{
		btCollisionObject*	colObj=m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);
		if(body&&body->getMotionState())
		{
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot=myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else
		{
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot=colObj->getWorldTransform().getBasis();
		}
		btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
		if(i&1) wireColor=btVector3(0.f,0.0f,1.f);
		///color differently for active, sleeping, wantsdeactivation states
		if (colObj->getActivationState() == 1) //active
		{
			if (i & 1)
			{
				wireColor += btVector3 (1.f,0.f,0.f);
			}
			else
			{			
				wireColor += btVector3 (.5f,0.f,0.f);
			}
		}
		if(colObj->getActivationState()==2) //ISLAND_SLEEPING
		{
			if(i&1)
			{
				wireColor += btVector3 (0.f,1.f, 0.f);
			}
			else
			{
				wireColor += btVector3 (0.f,0.5f,0.f);
			}
		}

		btVector3 aabbMin,aabbMax;
		m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
		
		aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);

		if (!(getDebugMode()& btIDebugDraw::DBG_DrawWireframe))
		{
			switch(pass)
			{
			case	0:	m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor,getDebugMode(),aabbMin,aabbMax);break;
			case	1:	m_shapeDrawer->drawShadow(m,m_sundirection*rot,colObj->getCollisionShape(),aabbMin,aabbMax);break;
			case	2:	m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor*btScalar(0.3),0,aabbMin,aabbMax);break;
			}
		}
	}
}

//
void VSCRootApplication::renderme()
{
	myinit();

	//updateCamera();

	if (m_dynamicsWorld)
	{			
		if(m_enableshadows)
		{
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			renderscene(0);

			glDisable(GL_LIGHTING);
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_STENCIL_TEST);
			glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
			glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
			glFrontFace(GL_CCW);
			glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
			renderscene(1);
			glFrontFace(GL_CW);
			glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
			renderscene(1);
			glFrontFace(GL_CCW);

			glPolygonMode(GL_FRONT,GL_FILL);
			glPolygonMode(GL_BACK,GL_FILL);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_LIGHTING);
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

			glDepthFunc(GL_LEQUAL);
			glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
			glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
			glDisable(GL_LIGHTING);
			renderscene(2);
			glEnable(GL_LIGHTING);
			glDepthFunc(GL_LESS);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
			renderscene(0);
		}


		glDisable(GL_LIGHTING);
		glColor3f(0, 0, 0);

		if ((m_debugMode & btIDebugDraw::DBG_NoHelpText)==0)
		{
			setOrthographicProjection();

			//showProfileInfo(xOffset,yStart,yIncr);

#ifdef USE_QUICKPROF
			if ( getDebugMode() & btIDebugDraw::DBG_ProfileTimings)
			{
				static int counter = 0;
				counter++;
				std::map<std::string, hidden::ProfileBlock*>::iterator iter;
				for (iter = btProfiler::mProfileBlocks.begin(); iter != btProfiler::mProfileBlocks.end(); ++iter)
				{
					char blockTime[128];
					sprintf(blockTime, "%s: %lf",&((*iter).first[0]),btProfiler::getBlockTime((*iter).first, btProfiler::BLOCK_CYCLE_SECONDS));//BLOCK_TOTAL_PERCENT));
					glRasterPos3f(xOffset,yStart,0);
					GLDebugDrawString(BMF_GetFont(BMF_kHelvetica10),blockTime);
					yStart += yIncr;

				}

			}
#endif //USE_QUICKPROF

			resetPerspectiveProjection();
		}

		glDisable(GL_LIGHTING);


	}

	updateCamera();

}

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"


void VSCRootApplication::clientResetScene()
{
	removePickingConstraint();

#ifdef SHOW_NUM_DEEP_PENETRATIONS
	gNumDeepPenetrationChecks = 0;
	gNumGjkChecks = 0;
#endif //SHOW_NUM_DEEP_PENETRATIONS

	gNumClampedCcdMotions = 0;
	int numObjects = 0;
	int i;

	if (m_dynamicsWorld)
	{
		int numConstraints = m_dynamicsWorld->getNumConstraints();
		for (i=0;i<numConstraints;i++)
		{
			m_dynamicsWorld->getConstraint(0)->setEnabled(true);
		}
		numObjects = m_dynamicsWorld->getNumCollisionObjects();
	
		///create a copy of the array, not a reference!
		btCollisionObjectArray copyArray = m_dynamicsWorld->getCollisionObjectArray();

		for (i=0;i<numObjects;i++)
		{
			btCollisionObject* colObj = copyArray[i];
			btRigidBody* body = btRigidBody::upcast(colObj);
			if (body)
			{
				if (body->getMotionState())
				{
					btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
					myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
					body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans );
					colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans );
					colObj->forceActivationState(ACTIVE_TAG);
					colObj->activate();
					colObj->setDeactivationTime(0);
					//colObj->setActivationState(WANTS_DEACTIVATION);
				}
				//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
				if (m_dynamicsWorld->getBroadphase()->getOverlappingPairCache())
					m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(),getDynamicsWorld()->getDispatcher());

				btRigidBody* body = btRigidBody::upcast(colObj);
				if (body && !body->isStaticObject())
				{
					btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0,0,0));
					btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0,0,0));
				}
			}

		}

		///reset some internal cached data in the broadphase
		m_dynamicsWorld->getBroadphase()->resetPool(getDynamicsWorld()->getDispatcher());
		m_dynamicsWorld->getConstraintSolver()->reset();

	}

}

#pragma mark --- VSC Interface Methods

// interface calls glut-independant
void VSCRootApplication::keyDown(VSCKeyboardCombination &keyComb) 
{
	
	//std::wcout << "\nGot key down with " << keyComb;
	
	VSCKeyboardAction action = m_controlSetup->getActionForKeyboardCombination(&keyComb);
	
	std::cout << "Key down action: " << std::dec << action << std::endl;
	
	switch (action) {
		case VSCKeyboardActionMoveForward:
			moveForward(true);
			break;
		case VSCKeyboardActionMoveBackward:
			moveBackward(true);
			break;
		case VSCKeyboardActionMoveUp:
			moveUp(true);
			break;
		case VSCKeyboardActionMoveDown:
			moveDown(true);
			break;
		case VSCKeyboardActionMoveLeft:
			moveLeft(true);
			break;
		case VSCKeyboardActionMoveRight:
			moveRight(true);
			break;
		default:
			break;
	}
	
}

void VSCRootApplication::keyUp(VSCKeyboardCombination &keyComb) 
{
	
	//std::cout << "\nGot key up with " << keyComb;
	
	VSCKeyboardAction action = m_controlSetup->getActionForKeyboardCombination(&keyComb);
	
	//std::cout << "\nCorresponding action is " << action;
	
	switch (action) {
		case VSCKeyboardActionMoveForward:
			moveForward(false);
			break;
		case VSCKeyboardActionMoveBackward:
			moveBackward(false);
			break;
		case VSCKeyboardActionMoveUp:
			moveUp(false);
			break;
		case VSCKeyboardActionMoveDown:
			moveDown(false);
			break;
		case VSCKeyboardActionMoveLeft:
			moveLeft(false);
			break;
		case VSCKeyboardActionMoveRight:
			moveRight(false);
			break;
		default:
			break;
	}
	
}

void VSCRootApplication::mouseDown(VSCMouseCombination &mouseComb, int x, int y) 
{
    
    VSCMouseAction action = m_controlSetup->getActionForMouseCombination(&mouseComb);
	
	std::wcout << "Mouse down with comb " << mouseComb << ", action " << action << std::endl;
	
	if (action & (VSCMouseActionGrab | VSCMouseActionDefault | VSCMouseActionCamera)) {
		m_currentMouseAction = (VSCMouseAction) (m_currentMouseAction | action);
	}
	
	btVector3 rayTo = getRayTo(x,y);
	
	switch (action)
	{
		case VSCMouseActionShootBox:
			
			shootBox(rayTo);
			break;

		case VSCMouseActionGrab:
		case VSCMouseActionDefault:
				
			//add a point to point constraint for picking
			if (m_dynamicsWorld)
			{
				
				btVector3 rayFrom;
				rayFrom = m_cameraPosition;
				
				btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);
				m_dynamicsWorld->rayTest(rayFrom,rayTo,rayCallback);
				
				if (rayCallback.hasHit())
				{
					
					btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
					
					if (body)
					{
						//other exclusions?
						if (!(body->isStaticObject() || body->isKinematicObject()))
						{
							
							//std::cout << "Picked body " << body << std::endl; 
							
							pickedBody = body;
							pickedBody->setActivationState(DISABLE_DEACTIVATION);
							
							btVector3 pickPos = rayCallback.m_hitPointWorld;
							
							std::cout << "Picked object at position (" << pickPos.getX() << "; "; 
							std::cout << pickPos.getY() << "; " << pickPos.getZ() << ")" << std::endl;
							
							
							btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
							
							if (use6Dof)
							{
								btTransform tr;
								tr.setIdentity();
								tr.setOrigin(localPivot);
								btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*body, tr,false);
								dof6->setLinearLowerLimit(btVector3(0,0,0));
								dof6->setLinearUpperLimit(btVector3(0,0,0));
								dof6->setAngularLowerLimit(btVector3(0,0,0));
								dof6->setAngularUpperLimit(btVector3(0,0,0));
								
								m_dynamicsWorld->addConstraint(dof6);
								m_pickConstraint = dof6;
								
								dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
								dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
								dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
								dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
								dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
								dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
								
								dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
								dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
								dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
								dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
								dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
								dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
							} 
							else
							{
								btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body,localPivot);
								m_dynamicsWorld->addConstraint(p2p);
								m_pickConstraint = p2p;
								p2p->m_setting.m_impulseClamp = mousePickClamping;
								//very weak constraint for picking
								p2p->m_setting.m_tau = 0.001f;								
							}
							
							use6Dof = !use6Dof;
							
							//save mouse position for dragging
							gOldPickingPos = rayTo;
							gHitPos = pickPos;
							
							gOldPickingDist  = (pickPos-rayFrom).length();
						}
					}
				}
			}
		
			break;
			
		default:
			break;
	}
	
	
}

void VSCRootApplication::mouseUp(VSCMouseCombination &comb, int x, int y) 
{
    
    VSCMouseAction action = m_controlSetup->getActionForMouseCombination(&comb);
	
	std::wcout << "Mouse up with comb " << comb << ", action " << action << std::endl;
	
	m_currentMouseAction = (VSCMouseAction)(m_currentMouseAction & (~action));
	
	if (action & VSCMouseActionGrab) {
		removePickingConstraint();
	}
	
}

void VSCRootApplication::mouseMotion(int dx, int dy, int x, int y) 
{
	
	
	if (m_currentMouseAction & (VSCMouseActionGrab | VSCMouseActionDefault)) 
	{
		
		if (m_pickConstraint)
		{
			//move the constraint pivot
			
			if (m_pickConstraint->getConstraintType() == D6_CONSTRAINT_TYPE)
			{
				btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(m_pickConstraint);
				
				if (pickCon)
				{
					//keep it at the same picking distance
					
					btVector3 newRayTo = getRayTo(x,y);
					btVector3 rayFrom;
					btVector3 oldPivotInB = pickCon->getFrameOffsetA().getOrigin();
					
					btVector3 newPivotB;

					rayFrom = m_cameraPosition;
					btVector3 dir = newRayTo-rayFrom;
					dir.normalize();
					dir *= gOldPickingDist;
					
					newPivotB = rayFrom + dir;

					pickCon->getFrameOffsetA().setOrigin(newPivotB);
				}
				
			} 
			
			else
			{
				btPoint2PointConstraint* pickCon = static_cast<btPoint2PointConstraint*>(m_pickConstraint);
				if (pickCon)
				{
					//keep it at the same picking distance
					
					btVector3 newRayTo = getRayTo(x,y);
					btVector3 rayFrom;
					btVector3 oldPivotInB = pickCon->getPivotInB();
					btVector3 newPivotB;

					rayFrom = m_cameraPosition;
					btVector3 dir = newRayTo-rayFrom;
					dir.normalize();
					dir *= gOldPickingDist;
					
					newPivotB = rayFrom + dir;

					pickCon->setPivotB(newPivotB);
				}
			}
		}
		
	}
	

	if (m_currentMouseAction & VSCMouseActionCamera || (m_currentMouseAction & VSCMouseActionDefault && !m_pickConstraint))
	{

		// default sensitivity 0.2
		
		// - not + dy for some interesting reason
		m_azi -= dx * btScalar(m_cameraMouseSensitivity);
		//m_azi = fmodf(m_azi + 180.f, btScalar(360.f)) - 180.f;
		if (m_azi > 180.0)
			m_azi -= 360.0;
		else if (m_azi < -180.0)
			m_azi += 360.0;
		
		m_ele += dy * btScalar(m_cameraMouseSensitivity);
		// not 90 because we want to avoid having a look parrallel to up (creates problems...)
		if (m_ele < -89.9)
			m_ele = -89.9;
		else if (m_ele > 89.9)
			m_ele = 89.9;
		
		updateCamera();
		
		std::cout << cameraStateString() << std::endl;

	}
		
}

void VSCRootApplication::updateCamera() {
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	btVector3 forward = m_cameraForward;
	forward.normalize();
	
	btScalar rele = m_ele * btScalar(RADIANS_PER_DEGREE);
	btScalar razi = m_azi * btScalar(RADIANS_PER_DEGREE);
	
	btQuaternion aziRot(m_cameraUp, razi);
	btVector3 horizontalLook = btMatrix3x3(aziRot) * forward;
	horizontalLook.normalize();
	
	btVector3 right = horizontalLook.cross(m_cameraUp);
	btQuaternion eleRot(right, rele);
	m_cameraLook = btMatrix3x3(eleRot) * horizontalLook;
	m_cameraLook.normalize();
	
	
	if (m_glutScreenWidth == 0 && m_glutScreenHeight == 0)
		return;
	
	btScalar aspect;
	//btVector3 extents;
	
	if (m_glutScreenWidth > m_glutScreenHeight) 
	{
		aspect = m_glutScreenWidth / (btScalar)m_glutScreenHeight;
		//extents.setValue(aspect * 1.0f, 1.0f,0);
	} 
	else 
	{
		aspect = m_glutScreenHeight / (btScalar)m_glutScreenWidth;
		//extents.setValue(1.0f, aspect*1.f,0);
	}
		
	if (m_glutScreenWidth > m_glutScreenHeight) 
		glFrustum (-aspect * m_frustumZNear, aspect * m_frustumZNear, -m_frustumZNear, m_frustumZNear, m_frustumZNear, m_frustumZFar);
	else 
		glFrustum (-aspect * m_frustumZNear, aspect * m_frustumZNear, -m_frustumZNear, m_frustumZNear, m_frustumZNear, m_frustumZFar);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	btVector3 cameraTargetPosition = m_cameraPosition + m_cameraLook;
	
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2], 
			  cameraTargetPosition[0], cameraTargetPosition[1], cameraTargetPosition[2], 
			  m_cameraUp[0], m_cameraUp[1], m_cameraUp[2]);
	
}

void VSCRootApplication::stepCamera() {
	
    bool log = true;
    bool filterConflicts = false;
	
	bool stepping = m_movingUp || m_movingDown || m_movingLeft || m_movingRight || m_movingForward || m_movingBackward;
    
    if (log && stepping)
        std::cout << "Camera step (";
    
	if (m_movingLeft && (!m_movingRight || !filterConflicts)) {
		stepLeft();
        if(log) std::cout << " left";
    }
	if (m_movingRight && (!m_movingLeft || !filterConflicts)) {
		stepRight();
        if(log) std::cout << " right";
    }
	if (m_movingUp && (!m_movingDown || !filterConflicts)) {
		stepUp();
        if(log) std::cout << " up";
    }
	if (m_movingDown && (!m_movingUp || !filterConflicts)) {
		stepDown();
        if(log) std::cout << " down";
    }
	if (m_movingForward && (!m_movingBackward || !filterConflicts)) {
		stepForward();
        if(log) std::cout << "forward";
    }
	if (m_movingBackward && (!m_movingForward || !filterConflicts)) {
		stepBackward();
        if(log) std::cout << " backward";
    }
	
    if (log && stepping) {
        std::cout << ")" << std::endl;
		std::cout << cameraStateString() << std::endl;
	}
    
}

std::string VSCRootApplication::cameraStateString() {
	
	std::stringstream camStream;
	
	camStream << "Camera : pos->" << m_cameraPosition << " look->" << m_cameraLook << " up->" << m_cameraUp << " ";
	camStream << "azi: " << m_azi << " ele: " << m_ele;
	
	//std::string camString = camStream.str();
	//std::cout << camString << std::endl;
	
	return camStream.str();

	
}

#pragma mark --- VSC Step Movement

void VSCRootApplication::stepLeft() {
	
	btVector3 left = m_cameraLook.cross(m_cameraUp);
	left.normalize();
	left *= m_cameraSpeed;
	//m_cameraPosition += left; 
	// don't understand why but it's the other way around 
	m_cameraPosition -= left; 
	
}

void VSCRootApplication::stepRight() {
	
	btVector3 left = m_cameraLook.cross(m_cameraUp);
	left.normalize();
	left *= m_cameraSpeed;
	//m_cameraPosition -= left;
	// don't understand why but it's the other way around 
	m_cameraPosition += left;
	
}

void VSCRootApplication::stepUp() {
	
	btVector3 left = m_cameraLook.cross(m_cameraUp);
	btVector3 down = m_cameraLook.cross(left);
	down.normalize();
	down *= m_cameraSpeed;
	m_cameraPosition -= down;
	
}

void VSCRootApplication::stepDown() {
	
	btVector3 left = m_cameraLook.cross(m_cameraUp);
	btVector3 down = m_cameraLook.cross(left);
	down.normalize();
	down *= m_cameraSpeed;
	m_cameraPosition += down;
	
}

void VSCRootApplication::stepForward() {
	
	btVector3 forward = m_cameraLook;
	forward.normalize();
	forward *= m_cameraSpeed;
	m_cameraPosition += forward;
	
}

void VSCRootApplication::stepBackward() {
	
	btVector3 forward = m_cameraLook;
	forward.normalize();
	forward *= m_cameraSpeed;
	m_cameraPosition -= forward;
	
}


