/*
 *  GLLight.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 31/03/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_Light.h"
#include "JBAT_MathTools.h"
#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

#pragma mark -
#pragma mark Constructor/Destructor

JBAT_Light::JBAT_Light() {
	setToDefault();
}




JBAT_Light::~JBAT_Light() {
}

// DEFAULT

void JBAT_Light::setToDefault() {
	
	on = true;
	spotlight = false;
	spotAngle = 360.0;
	
	m3dLoadVector4(ambientColor, 1.0, 1.0, 1.0, 1.0);
	m3dLoadVector4(diffuseColor, 1.0, 1.0, 1.0, 1.0);
	m3dLoadVector4(specularColor, 1.0, 1.0, 1.0, 1.0);
	m3dLoadVector4(specularColor, 1.0, 1.0, 1.0, 1.0);
	
	m3dLoadVector3(position, 0.0, 20.0, 20.0);
	m3dLoadVector3(direction, 0.0, 0.0, 0.0);
}


void JBAT_Light::setSpotlightAngle(float angle) {
	spotAngle = fullRangedAngle(angle);
}

#pragma mark -
#pragma mark Colors

void JBAT_Light::setAmbientColor(float r, float g, float b, float a) {
	m3dLoadVector4(ambientColor, r, g, b, a);
	ambientColorNeedsUpdate = true;
}

void JBAT_Light::setDiffuseColor(float r, float g, float b, float a){
	m3dLoadVector4(diffuseColor, r, g, b, a);
	diffuseColorNeedsUpdate = true;
}

void JBAT_Light::setSpecularColor(float r, float g, float b, float a){
	m3dLoadVector4(specularColor, r, g, b, a);
	specularColorNeedsUpdate = true;
}


float* JBAT_Light::getAmbientColor() {
	return &ambientColor[0];
}

float* JBAT_Light::getDiffuseColor() {
	return &diffuseColor[0];
}

float* JBAT_Light::getSpecularColor() {
	return &specularColor[0];
}

#pragma mark -
#pragma mark Position/Direction


void JBAT_Light::setPosition(float x, float y, float z) {
	m3dLoadVector3(position, x, y, z);
	positionNeedsUpdate = true;
}

void JBAT_Light::setDirection(float x, float y, float z) {
	m3dLoadVector3(direction, x, y, z);
	directionNeedsUpdate = true;
}

float* JBAT_Light::getPosition() {
	return &position[0];
}

float* JBAT_Light::getDirection() {
	return &direction[0];
}

#pragma mark -
#pragma mark Graphics


void JBAT_Light::applyLightTo(GLenum lightId) {
	
	
	/*
	 
	 The posiition of the light is relative to the current frame of reference 
	 
	 */
	
	if (onNeedsUpdate) {
		if (on) 
			glEnable(lightId);
		else
			glDisable(lightId);
	}
	
	
	// The light is composed of just diffuse and specular components 
	if (ambientColorNeedsUpdate)
		glLightfv(lightId, GL_AMBIENT, ambientColor); 
	
	if (diffuseColorNeedsUpdate)
		glLightfv(lightId, GL_DIFFUSE, diffuseColor); 
	
	
	if (specularColorNeedsUpdate)
		glLightfv(lightId, GL_SPECULAR, specularColor); 
	
	if (positionNeedsUpdate)
		glLightfv(lightId, GL_POSITION, position);
	
	// Specific spot effects // Cut-off angle is 60 degrees 
	//glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,60.0f);
	
	// Enable this light in particular 
	//glEnable(GL_LIGHT0);
	

	

}

std::ostream & operator<<(std::ostream & s, JBAT_Light & c) {
	
	
	// should be able to access private/protected members as is a friend function
	
	float* color;
	
	color = c.getAmbientColor();
	s << "JBAT_Light with:\nambient: " << color[0] << " " << color[1] << " " << color[2] << " " << color[3];
	color = c.getDiffuseColor();
	s << "JBAT_Light with:\ndiffuse: " << color[0] << " " << color[1] << " " << color[2] << " " << color[3];
	color = c.getSpecularColor();
	s << "JBAT_Light with:\nspecular: " << color[0] << " " << color[1] << " " << color[2] << " " << color[3];
	
	float *position = c.getPosition();
	s << "\nAt position: " << position[0] << " " << position[1] << " " << position[2];
	float *direction = c.getDirection();
	s << "\nIn direction: " << direction[0] << " " << direction[1] << " " << direction[2];
	
	if (c.isOn())
		s << "\nIS ON";
	
	if (c.isSpotlight())
		s << "\nIS SPOTLIGHT, with angle " << c.getSpotlightAngle();
	
	
	return s;
}


void JBAT_SetUpOpenGLLighting(JBAT_OpenGLLightingSetup setup) {
	
	if (setup == JBAT_OpenGLLightingSetupDefault) {
		
		glEnable(GL_LIGHTING);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Set the background color to black
		
		//Ambient lighting (this is for the whole seen and does not have a source position)
		GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
		
		

		glEnable(GL_NORMALIZE); //Have OpenGL automatically normalize our normals
		glShadeModel(GL_SMOOTH); //Enable smooth shading
		

		

	}
	
}


