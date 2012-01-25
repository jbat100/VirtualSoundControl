/*
 *  JBAT_FPSCamera.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 03/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_CAMERA_
#define _JBAT_CAMERA_

#include "Math3d.h"
#include "GLFrame.h"
#include <iostream>


typedef enum _JBAT_OpenGLCameraType {
	JBAT_OpenGLCameraTypeFPS,
	JBAT_OpenGLCameraTypeRotating
} JBAT_OpenGLCameraType;


class JBAT_Camera : public GLFrame {
	
protected:
	
	float theta;
	float phi;
	
	float movementSpeed;
	float rotationSpeed;
	
	M3DVector3f upReference;
	M3DVector3f forwardReference;
	
	JBAT_OpenGLCameraType cameraType;
	
	bool needsUpdate;
	
	
public:
	
	JBAT_Camera(void);
	~JBAT_Camera(void);
	
	void setToDefault(void);
	
	void setMovementSpeed(float s) {movementSpeed = s;}
	float getMovementSpeed() {return movementSpeed;}
	void setRotationSpeed(float s) {rotationSpeed = s;}
	float getRotationSpeed() {return rotationSpeed;}
	
	void moveForward() {MoveForward(movementSpeed);}
	void moveBackward() {MoveForward(-movementSpeed);}
	void moveRight() {MoveRight(movementSpeed);}
	void moveLeft() {MoveRight(-movementSpeed);}
	void moveUp() {MoveUp(movementSpeed);}
	void moveDown() {MoveUp(-movementSpeed);}
	
	void setTheta(float _theta);
	void setPhi(float _phi);
	void setThetaAndPhi(float _theta, float _phi);
	
	inline float getTheta(void) const {return theta;}
	inline float getPhi(void) const {return phi;}
	
	void addTheta(float deltaTheta);
	void addPhi(float deltaPhi);
	void addThetaAndPhi(float deltaTheta, float _phi);
	

	void setUpReference(float x, float y, float z);
	void setForwardReference(float x, float y, float z);
	
	float* getUpReference();
	float* getForwardReference();
	
	float* getOrigin();
	float* getForward();
	float* getUp();
	
	JBAT_OpenGLCameraType getCameraType();
	void setCameraType(JBAT_OpenGLCameraType type);
	
	void updateFPSFrame(void);
	
	void applyGLCameraTransform();
	
	friend std::ostream & operator<<(std::ostream& s, JBAT_Camera & camera);
	
	/*
	
	 CALL ON RESIZE
	 glMatrixMode(GL_PROJECTION) // for the subsequent call to gluPerspective 
	 gluPerspective(cameraAngle-45.0-m width/height, near z clip -1-, far z clip -200-)
	 
	 
	 CALL ON DRAW
	 glclear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	 glMatrixMode(GL_MODEL_VIEW)  // This is the "normal" mode x
	 glLoadIdentity() // to reset the frame to the origin
	 
	 f after the decimal numbers forces the compiler to treat them as floats (not doubles for example)
	 
	 glutSwapBuffers() sends the 3d scene to the window but may be inneficient or inadequate depending on the setup
	 glutPostRedisplay() to redraw the scene after a change
	 
	 glutTimerFunc (25, update, 0) -- update is the name of the function 
	 
	 */

};



#endif