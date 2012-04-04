/*
 *  GLLight.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 31/03/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_LIGHT_
#define _JBAT_LIGHT_

#define DEFAULT_COLOR_VECTOR {1.0, 1.0, 1.0, 1.0}

#include "Math3d.h"
#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>



class JBAT_Light {
	
	
public:
	
	
	JBAT_Light();
	// No need for a custom copy constuctor as there is no dynamically allocated data
	~JBAT_Light();
	
	void setToDefault();
	void applyGLWithId(GLenum lightId);
	
	void enable() {on = true; onNeedsUpdate = true;}
	void disable() {on = false; onNeedsUpdate = true;}
	bool isOn() {return on;}
	
	void enableSpotlight() {spotlight = true; spotLightNeedsUpdate = true;}
	void disableSpotlight() {spotlight = false; spotLightNeedsUpdate = true;}
	bool isSpotlight() {return spotlight;}
	
	void setSpotlightAngle(float angle);
	float getSpotlightAngle() {return spotAngle;}
	
	void setAmbientColor(float r, float g, float b, float a);
	void setDiffuseColor(float r, float g, float b, float a);
	void setSpecularColor(float r, float g, float b, float a);
	float* getAmbientColor();
	float* getDiffuseColor();
	float* getSpecularColor();
	
	void setPosition(float x, float y, float z);
	void setDirection(float x, float y, float z);
	
	float* getPosition();
	float* getDirection();
	
	void setAmbientColorNeedsUpdate() {ambientColorNeedsUpdate = true;}
	void setDiffuseColorNeedsUpdate() {diffuseColorNeedsUpdate = true;}
	void setSpecularColorNeedsUpdate() {specularColorNeedsUpdate = true;}
	void setPositionNeedsUpdate() {positionNeedsUpdate = true;}
	void setDirectionNeedsUpdate() {directionNeedsUpdate = true;}
	void setSpotLightNeedsUpdate() {spotLightNeedsUpdate = true;}
	void setSpotLightAngleNeedsUpdate() {spotLightAngleNeedsUpdate = true;}
	void setOnNeedsUpdate() {onNeedsUpdate = true;}
	
	friend std::ostream & operator<<(std::ostream& s, JBAT_Light & light);
	
	
protected:
	
	
	bool on;
	bool spotlight;
	float spotAngle;
	
	M3DVector4f ambientColor;
	M3DVector4f diffuseColor;
	M3DVector4f specularColor;
	
	M3DVector3f position;
	M3DVector3f direction;
	
	// To avoid unnecessary GL calls 
	
	bool ambientColorNeedsUpdate;
	bool diffuseColorNeedsUpdate;
	bool specularColorNeedsUpdate;
	bool positionNeedsUpdate;
	bool directionNeedsUpdate;
	bool spotLightNeedsUpdate;
	bool spotLightAngleNeedsUpdate;
	bool onNeedsUpdate;
	
};



typedef enum _JBAT_OpenGLLightingSetup {
	JBAT_OpenGLLightingSetupDefault
} JBAT_OpenGLLightingSetup;


extern void JBAT_SetupOpenGLLighting(JBAT_OpenGLLightingSetup setup);

#endif