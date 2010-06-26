/*
 *  BaseGLViewComponents.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 31/03/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_LightSet.h"
#include "JBAT_Light.h"

#include <string>
#include <list>
#include <iostream>



JBAT_LightSet::JBAT_LightSet(void) {
	
	std::cout << "\nIn JBAT_LightSet constructor";
	JBAT_Light* light = new JBAT_Light();
	std::cout << "\nCreated light";
	lights.push_back(light);
	std::cout << "\nAdded light to list";
	
}

JBAT_LightSet::~JBAT_LightSet(void) {
	
	/*
	 // In Theory clear should call the destructors on all the objects in the list
	for (std::list<JBAT_Light*>::iterator iter = lights.begin(); iter != lights.end(); iter++) {
		delete (*iter);
	 
	 ACTUALLY NO!
	 This would call the destructor if was a list of JBAT_Light objects not JBAT_Light*
	}
	 */
	
	for (std::list<JBAT_Light*>::iterator iter = lights.begin(); iter != lights.end(); iter++) {
		delete *iter;
	}
	
	
	lights.clear();
	
}

bool JBAT_LightSet::addLight(void) {
	if (lights.size() < 8) {
		//std::cout << "Adding light to set containing " << lights.size() << " lights";
		JBAT_Light* light = new JBAT_Light();
		lights.push_back(light);
		//std::cout << "After adding light, there are " << lights.size() << " lights";
		return true;
	}
	return false;
}

bool JBAT_LightSet::addLight(JBAT_Light* light) {
	if (lights.size() < 8) {
		// need to make a copy of the object 
		lights.push_back(light);
		return true;
	}
	return false;
}

void JBAT_LightSet::removeLightAtIndex(int index) {
	if (index >= lights.size()) {
		std::cerr << "Attempted to remove light at index " << index << " beyond vector bounds " << lights.size() << std::endl;
		return;
	}
	//std::cout << "Removing light at index " << index << " from set containing " << lights.size() << " lights";
	std::list<JBAT_Light*>::iterator iter = lights.begin();
	std::advance(iter,index);
	delete *iter; // erase does not call the destructor as *iter is a JBAT_Light*
	lights.erase(iter);
	//std::cout << "After removing light, the are " << lights.size() << " lights in the set";
}

void JBAT_LightSet::removeLight(JBAT_Light* light) {
	for (std::list<JBAT_Light*>::iterator iter = lights.begin(); iter != lights.end(); iter++) {
		if (light == *iter) {
			delete *iter;
		}
	}
	lights.remove(light);
}

void JBAT_LightSet::removeAllLights(void) {
	for (std::list<JBAT_Light*>::iterator iter = lights.begin(); iter != lights.end(); iter++) {
		delete *iter;
	}
	lights.clear();
}

JBAT_Light* JBAT_LightSet::getLightAtIndex(int index) {
	if (index >= lights.size()) {
		std::cerr << "Attempted to get light at index " << index << " beyond vector bounds " << lights.size() << std::endl;
		return NULL;
	}
	//std::cout << "In JBAT_LightSet getLightAtIndex " << index;
	std::list<JBAT_Light*>::iterator iter = lights.begin();
	std::advance(iter,index);
	//std::cout << "Got light: " << *(*iter);
	return *iter;
	
}

int JBAT_LightSet::numberOfLights(void) {
	//std::cout << "In JBAT_LightSet::numberOfLightsm returning" << lights.size();
	return lights.size();
}

void JBAT_LightSet::applyGL() {
	
	GLenum lightIndex;
	int count = 0;
	
	for (std::list<JBAT_Light*>::iterator iter = lights.begin(); iter != lights.end(); iter++) {
		
		if (count > 7) {
			break;
		}
		
		switch (count) {
			case 0:
				lightIndex = GL_LIGHT0;
				break;
			case 1:
				lightIndex = GL_LIGHT1;
				break;
			case 2:
				lightIndex = GL_LIGHT2;
				break;
			case 3:
				lightIndex = GL_LIGHT3;
				break;
			case 4:
				lightIndex = GL_LIGHT4;
				break;
			case 5:
				lightIndex = GL_LIGHT5;
				break;
			case 6:
				lightIndex = GL_LIGHT6;
				break;
			case 7:
				lightIndex = GL_LIGHT7;
				break;
			default:
				break;
		}
		
		(*iter)->applyGLWithId(lightIndex);
		
		count++;
		
	}
	
}


std::ostream & operator<<(std::ostream& s, JBAT_LightSet const & components) {
	
	s << "JBAT_LightSet with lights: \n";
	for (std::list<JBAT_Light*>::const_iterator iter = components.lights.begin(); iter != components.lights.end(); iter++) {
		s << "\n" << **iter;
	}
	return s;
	
}

// Black background glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

