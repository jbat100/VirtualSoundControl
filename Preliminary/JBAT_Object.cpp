/*
 *  JBAT_Object.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 14/05/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_Object.h"
#include "Math3d.h"
#include "GLFrame.h"
#include <iostream>

JBAT_Object::JBAT_Object() : JBAT_BufferedMesh() {
	setToDefault();
}

JBAT_Object::JBAT_Object(const std::string& objectName) : JBAT_BufferedMesh(){
	setToDefault();
}

JBAT_Object::JBAT_Object(const JBAT_Object& object) : JBAT_BufferedMesh(object) {
	
	frame = new GLFrame(); // allocate the memory
	
	*frame = *(object.frame); // this should work or does it allocate memory again?
	
	shininess = object.shininess;
	shininessNeedsUpdate = true;
	m3dCopyVector4(specularColor, object.specularColor);
	specularColorNeedsUpdate = true;
	on = object.on;
}

JBAT_Object::~JBAT_Object() {
	
	// delete child objects
	for (std::list<JBAT_Object*>::iterator iter = childObjects.begin(); iter != childObjects.end(); iter++) {
		delete *iter;
	}
	
	//std::cout << "In JBAT_Object destructor";
	if (frame)
		delete frame;
	
	

}

void JBAT_Object::setToDefault() {
	
	shininess = 0.0;
	shininessNeedsUpdate = true;
	m3dLoadVector4(specularColor, 1.0, 1.0, 1.0, 1.0);
	specularColorNeedsUpdate = true;
	on = true;
	
	if (frame)
		delete frame;
	
	frame = new GLFrame();
	
	// delete child objects
	for (std::list<JBAT_Object*>::iterator iter = childObjects.begin(); iter != childObjects.end(); iter++) {
		delete *iter;
	}
	childObjects.clear();
	
}



void JBAT_Object::addChildObject(JBAT_Object* childObject, bool duplicate) {
	
	if (duplicate) {
		JBAT_Object *copyObject = new JBAT_Object(*childObject); // need to pass the actual object, not the pointer
		childObjects.push_back(copyObject);
	}
	else {
		childObjects.push_back(childObject);
	}
	
}

void JBAT_Object::removeChildObject(JBAT_Object* childObject) {
	for (std::list<JBAT_Object*>::iterator iter = childObjects.begin(); iter != childObjects.end(); iter++) {
		if (childObject == *iter)
			delete *iter;
	}
	childObjects.remove(childObject);
}

void JBAT_Object::removeChildObjectAtIndex(int index) {
	
	if (index >= childObjects.size()) {
		std::cerr << "Attempted to remove child object at index " << index << " beyond list bounds " << childObjects.size() << std::endl;
		return;
	}
	//std::cout << "Removing light at index " << index << " from set containing " << lights.size() << " lights";
	std::list<JBAT_Object*>::iterator iter = childObjects.begin();
	std::advance(iter,index);
	delete *iter; // erase does not call the destructor as *iter is a JBAT_Object*
	childObjects.erase(iter);
	
	
}

void JBAT_Object::removeAllChildObjects() {
	
	for (std::list<JBAT_Object*>::iterator iter = childObjects.begin(); iter != childObjects.end(); iter++) {
		delete *iter;
	}
	childObjects.clear();
	
}

JBAT_Object* JBAT_Object::getChildObjectAtIndex(int index) {
	
	if (index >= childObjects.size()) {
		std::cerr << "Attempted to get childObject at index " << index << " beyond list bounds " << childObjects.size() << std::endl;
		return NULL;
	}
	//std::cout << "In JBAT_LightSet getLightAtIndex " << index;
	std::list<JBAT_Object*>::iterator iter = childObjects.begin();
	std::advance(iter,index);
	//std::cout << "Got light: " << *(*iter);
	return *iter;
	
}


void JBAT_Object::getChildObjects(std::list<JBAT_Object*>& l) const {
	
	for (std::list<JBAT_Object*>::const_iterator iter = childObjects.begin(); iter != childObjects.end(); iter++) {
		l.push_back(*iter);
	}
	
}

void JBAT_Object::addChildObjects(const std::list<JBAT_Object*>& c, bool duplicate) {
	
	for (std::list<JBAT_Object*>::const_iterator iter = c.begin(); iter != c.end(); iter++) {
		if (duplicate) {
			JBAT_Object *copiedObject = new JBAT_Object(**iter);
			childObjects.push_back(copiedObject);
		}
		else {
			childObjects.push_back(*iter);
		}

	}
	
}




void JBAT_Object::setName(const std::string n) {
	name = n;
}

std::string JBAT_Object::getName() {
	return name;
}

void JBAT_Object::setFrame(const GLFrame* f) {
	*frame = *f;
}

GLFrame* JBAT_Object::getFrame() {
	return frame;
}

void JBAT_Object::setSpecularColor(const float r, const float g, const float b, const float a) {
	m3dLoadVector4(specularColor, r, g, b, a);
	specularColorNeedsUpdate = true;
}

float* JBAT_Object::getSpecularColor() {
	return &specularColor[0];
}

void JBAT_Object::setShininess(float s) {
	shininess = s;
}

float JBAT_Object::getShininess() {
	return shininess;
}

void JBAT_Object::setupOpenGL() {
	
	/*
	 
	 http://www.videotutorialsrock.com/opengl_tutorial/reference.php
	
	 //Set up the material
	 
	 //The color of the object
	 GLfloat materialColor[] = {0.2f, 0.2f, 1.0f, 1.0f};
	 //The specular (shiny) component of the material
	 GLfloat materialSpecular[] = {0.8f, 0.8f, 0.8f, 1.0f};
	 //The color emitted by the material
	 GLfloat materialEmission[] = {0, 0, 0, 1.0f};
	 
	 
	 IMPORTANT:
	 
	 glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work <--- IS IT?
	 glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	 glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	 glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	 
	 The shininess parameter.  A higher number indicates a more concentrated
	 shiny area, while a lower number indicates a larger shiny area.  The
	 shininess must be between 0 and 128.
	 
	 glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);
	 
	 
	 EXPLANATION:
	 
	 http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=145662
	 
	 
	 Great, now you have the right colours, but no lighting.
	 Do the following to get the right colours AND lighting:-
	 glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	 glEnable(GL_COLOR_MATERIAL);
	 
	 Now your lighting will work with your colours correctly.
	 What is happening is this:-
	 colours are ignored when lighting is enabled, because lighting uses materials instead. 
	 
	 -----------------------------------------------------------------------------------------------
	 
	 When you enable GL_COLOR_MATERIAL you are telling openGL that every time a glColor3f 
	 (or equivelent entry in your colour array) is issued, change the specified material property 
	 to that colour instead (GL_DIFFUSE, GL_AMBIENT or a convenient way of combining the two by 
	 using the GL_AMBIENT_AND_DIFFUSE constant).
	 
	 -----------------------------------------------------------------------------------------------
	 
	 ME: 
	 
	 So given I always want to use the color array, I do not want to set the ambient and diffuse color of the
	 material. Just set the specular color and the shininness
	 
	 */
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	
	

}


std::ostream & operator<<(std::ostream& s, JBAT_Object const & object) {
	s << "Object: (<< needs to be implemented)";
	return s;
}