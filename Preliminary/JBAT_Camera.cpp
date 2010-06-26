/*
 *  JBAT_Camera.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 03/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_Camera.h"
#include "JBAT_MathTools.h"
#include "Math3d.h"
#include <math.h>
#include <iostream>

JBAT_Camera::JBAT_Camera(void) {
	setToDefault();
}

JBAT_Camera::~JBAT_Camera(void) {
	
}

// In FPS (first person shooter) Theta (azimuth) can range -pi to pi, phi (elevation) can range -pi/2 to pi/2

void JBAT_Camera::setToDefault(void) {
	
	m3dLoadVector3(upReference, 0.0, 0.0, 1.0);
	m3dLoadVector3(forwardReference, 1.0, 0.0, 0.0);
	m3dLoadVector3(vOrigin, -20.0, 0.0, 0.0);
	theta = 0.0;
	phi = 0.0;
	movementSpeed = 0.3;
	rotationSpeed = 0.01;
	needsUpdate = true;
}

void JBAT_Camera::setTheta(float _theta) {
	theta = fullRangedAngle(_theta);
	needsUpdate = true;
}

void JBAT_Camera::setPhi(float _phi) {
	phi = halfRangedAngle(_phi);
	needsUpdate = true;
}

void JBAT_Camera::setThetaAndPhi(float _theta, float _phi) {
	theta = fullRangedAngle(_theta);
	phi = halfRangedAngle(_phi);
	needsUpdate = true;
}

void JBAT_Camera::addTheta(float deltaTheta) {
	theta = fullRangedAngle(theta+deltaTheta);
	//std::cout << "\nChanged theta to " << theta;
	needsUpdate = true;
}

void JBAT_Camera::addPhi(float deltaPhi) {
	phi = halfRangedAngle(phi+deltaPhi);
	//std::cout << "\nChanged phi to " << phi;
	needsUpdate = true;
}

void JBAT_Camera::addThetaAndPhi(float deltaTheta, float deltaPhi) {
	theta = fullRangedAngle(theta+deltaTheta);
	phi = halfRangedAngle(phi+deltaPhi);
	needsUpdate = true;
}

void JBAT_Camera::updateFPSFrame(void) {
	
	//std::cout << "\nUpdating FPS camera frame";

	// Rotate reference forward vector by theta around reference up to get new flat forward
	M3DVector3f flatForward;
	M3DMatrix33f thetaRotationMatrix;
	m3dRotationMatrix33(thetaRotationMatrix, theta, upReference[0], upReference[1], upReference[2]);
	m3dRotateVector(flatForward, forwardReference, thetaRotationMatrix);
	
	// Calculate new flat left (to use as rotation axis for phi)
	M3DVector3f flatLeft;
	m3dCrossProduct(flatLeft, upReference, flatForward);
	
	// Rotate new flat forward by phi around new flat left to get new forward vector 
	M3DMatrix33f phiRotationMatrix;
	m3dRotationMatrix33(phiRotationMatrix, phi, flatLeft[0], flatLeft[1], flatLeft[2]);
	m3dRotateVector(vForward, flatForward, phiRotationMatrix);
	
	// Rotate new forward  vector by pi/2 around new flat left to get new up vector
	M3DMatrix33f upRotationMatrix;
	m3dRotationMatrix33(upRotationMatrix, M_PI/2.0, flatLeft[0], flatLeft[1], flatLeft[2]);
	m3dRotateVector(vUp, vForward, upRotationMatrix);
	
	m3dNormalizeVector(vForward);
	m3dNormalizeVector(vUp);

}


void JBAT_Camera::setUpReference(float x, float y, float z) {
	m3dLoadVector3(upReference, x, y, z);
	needsUpdate = true;
}

void JBAT_Camera::setForwardReference(float x, float y, float z) {
	m3dLoadVector3(forwardReference, x, y, z);
	needsUpdate = true;
}

float* JBAT_Camera::getUpReference()  {
	return &upReference[0];
}

float* JBAT_Camera::getForwardReference()  {
	return &forwardReference[0];
}

float* JBAT_Camera::getOrigin()  {
	return &vOrigin[0];
}

float* JBAT_Camera::getForward()  {
	return &vForward[0];
}

float* JBAT_Camera::getUp()  {
	return &vUp[0];
}


JBAT_OpenGLCameraType JBAT_Camera::getCameraType()  {
	return cameraType;
}

void JBAT_Camera::setCameraType(JBAT_OpenGLCameraType type) {
	cameraType = type;
	needsUpdate = true;
}

void JBAT_Camera::applyGLCameraTransform() {
	
	if (needsUpdate) {
		
		if (cameraType == JBAT_OpenGLCameraTypeFPS) 
			updateFPSFrame();

	}
	
	this->ApplyCameraTransform(false);
	
}

std::ostream & operator<<(std::ostream& s, JBAT_Camera & camera) {

	s << "\nJBAT_Camera at position: " << camera.vOrigin[0] << " " << camera.vOrigin[1] << " " << camera.vOrigin[2];
	s << "\ntheta: " << camera.theta << " phi: " << camera.phi;
	s << "\nupReference: " << camera.upReference[0] << " " << camera.upReference[1] << " " << camera.upReference[2];
	s << "\nforwardReference: " << camera.forwardReference[0] << " " << camera.forwardReference[1] << " " << camera.forwardReference[2];
	s << "\nupVector: " << camera.vUp[0] << " " << camera.vUp[1] << " " << camera.vUp[2];
	s << "\nforwardVector: " << camera.vForward[0] << " " << camera.vForward[1] << " " << camera.vForward[2];
	s << "\n";

	return s;
	
}


