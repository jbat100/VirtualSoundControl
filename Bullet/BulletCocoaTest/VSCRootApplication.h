/*
Original file heavily mofified JBAT100
 
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

#ifndef DEMO_APPLICATION_H
#define DEMO_APPLICATION_H

#include "GlutStuff.h"
#include "GL_ShapeDrawer.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"

#include "VSCControlSetup.h"

class	btCollisionShape;
class	btDynamicsWorld;
class	btRigidBody;
class	btTypedConstraint;

#define RADIANS_PER_DEGREE 0.01745329251994329547



class VSCRootApplication
{
	
protected:
	
	/*
	 * Clock and Profiling
	 */
	class CProfileIterator* m_profileIterator;
#ifdef USE_BT_CLOCK
	btClock m_clock;
#endif //USE_BT_CLOCK
	void  displayProfileString(int xOffset,int yStart,char* message);

	/*
	 * Bullet dynamics
	 */
	btDynamicsWorld* m_dynamicsWorld;
	btTypedConstraint* m_pickConstraint; //constraint for mouse picking
	virtual void removePickingConstraint();
	
	/*
	 * Objects
	 */
	btCollisionShape* m_shootBoxShape;

	/*
	 * Camera and Interaction
	 */
	// accessible (these variable should be configurable from outside)
	float m_cameraDistance;
	float m_ele;
	float m_azi;
	btVector3 m_cameraPosition;
	btVector3 m_cameraUp; 
	btVector3 m_cameraForward; // reference for azimuth and elevation
	float m_cameraSpeed;
	float m_cameraMouseSensitivity;
	// non-accessible (these variable should be used within the class only)
	// updated depending on m_cameraDistance, m_ele, m_azi, m_cameraPosition, m_cameraUp, m_cameraForward
	btVector3 m_cameraLook; 
    // mouse action
    VSCMouseAction m_rightMouseAction;
    VSCMouseAction m_leftMouseAction;
    VSCMouseAction m_currentMouseAction;
	// moving state
	bool m_movingForward;
	bool m_movingBackward;
	bool m_movingRight;
	bool m_movingLeft;
	bool m_movingUp;
	bool m_movingDown;
	// movement step functions
	void stepLeft();
	void stepRight();
	void stepUp();
	void stepDown();
	void stepForward();
	void stepBackward();

	/*
	 * GLSettings
	 */
	float m_scaleBottom;
	float m_scaleFactor;
	int m_glutScreenWidth;
	int m_glutScreenHeight;
	float m_frustumZNear;
	float m_frustumZFar;
	int	m_ortho;

	/*
	 * Drawing 
	 */
	GL_ShapeDrawer*	m_shapeDrawer;
	bool m_enableshadows;
	btVector3 m_sundirection;
	btScalar m_defaultContactProcessingThreshold;
	void showProfileInfo(int& xOffset,int& yStart, int yIncr);
	void renderscene(int pass);
	int	m_debugMode;
	
	/*
	 * Simulation 
	 */
	float m_shootBoxInitialSpeed;
	bool m_idle;
	

public:
	
	/*
	 * Constructor/Destructor
	 */
	VSCRootApplication();
	virtual ~VSCRootApplication();
	
	/*
	 * Misc Utils
	 */
	btScalar getDeltaTimeMicroseconds();
	btVector3 getRayTo(int x,int y);

	/*
	 * Dynamics and Physics
	 */ 
	virtual void simulate();
	btDynamicsWorld* getDynamicsWorld() { return m_dynamicsWorld; }
	virtual	void initPhysics() = 0;
	virtual void myinit();
	btRigidBody* localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape);
	void toggleIdle();
	bool isIdle() const { return	m_idle; }
	void setIdle(bool idle) { m_idle = idle; }

	/*
	 * Drawing Settings
	 */
	void overrideGLShapeDrawer (GL_ShapeDrawer* shapeDrawer);
	void setOrthographicProjection();
	void resetPerspectiveProjection();
	virtual	void setDrawClusters(bool drawClusters) { }
	bool getTexturing() const { return m_shapeDrawer->hasTextureEnabled(); }
	bool setTexturing(bool enable) { return (m_shapeDrawer->enableTexture(enable)); }
	bool getShadows() const { return m_enableshadows; }
	bool setShadows(bool enable)	{ bool p=m_enableshadows;m_enableshadows=enable;return(p); }
	int	getDebugMode() { return m_debugMode ; }
	void setDebugMode(int mode);
	
	/*
	 * Camera Interaction Setters/Getters
	 */
	float getAzi() { return m_azi; }
	void setAzi(float azi) { m_azi = azi; }
	float getEle() { return m_ele; }
	void setEle(float ele) { m_ele = ele; }
	btVector3 getCameraUp() { return m_cameraUp; }
	void setCameraUp(const btVector3& camUp) { m_cameraUp = camUp; }
	btVector3 getCameraPosition() { return m_cameraPosition; }
	void setCameraPosition(const btVector3& camPos) { m_cameraPosition = camPos; }
	btVector3 getCameraForward() { return m_cameraForward; }
	void setCameraForward(const btVector3& camFor) { m_cameraForward = camFor; }
    btVector3 getCameraLook() { return m_cameraLook; }
	void updateCameraLook();
	virtual void updateCamera();

	/*
	 * Old GLUT Stuff
	 */
	//glut callbacks
	float getCameraDistance() { return m_cameraDistance; }
	void setCameraDistance(float dist) { m_cameraDistance = dist;}	
    void reshape(int w, int h);
    void clientResetScene();
    /*
	void moveAndDisplay();
	virtual void clientMoveAndDisplay() = 0;
	virtual void clientResetScene();
	//callback methods by glut	
	virtual void keyboardCallback(unsigned char key, int x, int y);
	virtual void keyboardUpCallback(unsigned char key, int x, int y) {}
	virtual void specialKeyboard(int key, int x, int y){}
	virtual void specialKeyboardUp(int key, int x, int y){}
	virtual void reshape(int w, int h);
	virtual void mouseFunc(int button, int state, int x, int y);
	virtual void mouseMotionFunc(int x, int y);
	virtual	void updateModifierKeys() = 0;
     */

	/*
	 * Generic (GLUT-independant) Camera Interface
	 *
	 * Keyboard / Mouse Input
	 */
	void keyDown(VSCKeyboardCombination keyCombination);
	void keyUp(VSCKeyboardCombination keyCombination);
	void mouseDown(VSCMouseButton button, int x, int y);
	void mouseUp(VSCMouseButton button, int x, int y);
	void mouseMotion(int dx, int dy, int x, int y);
	/*
	 * Movement
	 */
	void moveLeft(const bool move) { m_movingLeft = move; } 
	void moveRight(const bool move) { m_movingRight = move; } 
	void moveUp(const bool move) { m_movingUp = move; } 
	void moveDown(const bool move) { m_movingDown = move; } 
	void moveForward(const bool move) { m_movingForward = move; } 
	void moveBackward(const bool move) { m_movingBackward = move; } 
	/*
	 * Sensitivity Getter/Setters
	 */
	float getCameraSpeed() { return m_cameraSpeed; }
	void setCameraSpeed(const float camSpeed) { m_cameraSpeed = camSpeed; }
	float getCameraMouseSensitivity() { return m_cameraMouseSensitivity; }
	void setCameraMouseSensitivity(const float camMouseSensitivity) { m_cameraMouseSensitivity = camMouseSensitivity; }
	
	/*
	 * GL Stuff
	 */
	virtual void display();
	virtual void renderme();
	virtual	void swapBuffers() = 0;
	void setFrustumZPlanes(float zNear, float zFar) { m_frustumZNear = zNear; m_frustumZFar = zFar; }
	
	/*
	 * Shooting
	 */
	virtual void setShootBoxShape ();
	virtual void shootBox(const btVector3& destination);

};

#endif //DEMO_APPLICATION_H


