/*
 *  JBAT_ObjectSet.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 25/05/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_ObjectSet.h"
#include "JBAT_Object.h"

#include <string>
#include <list>
#include <iostream>



JBAT_ObjectSet::JBAT_ObjectSet(void) {
	
	
}

JBAT_ObjectSet::~JBAT_ObjectSet(void) {
	
	// rootObjects is a list of pointers so the following clear() will not call the destructor, 
	// this has to be done explicitly
	for (std::list<JBAT_Object*>::iterator iter = rootObjects.begin(); iter != rootObjects.end(); iter++) {
		delete *iter;
	}

	rootObjects.clear();
	
}

void JBAT_ObjectSet::addObject() {
	JBAT_Object* newObject = new JBAT_Object();
	rootObjects.push_back(newObject);
}

void JBAT_ObjectSet::addObject(JBAT_Object *o, bool duplicate) {
	
	if (duplicate) {
		JBAT_Object* copiedObject = new JBAT_Object(*o);
		rootObjects.push_back(copiedObject);
	}
	else {
		rootObjects.push_back(o);
	}


}

void JBAT_ObjectSet::removeObjectAtIndex(int index) {
	if (index >= rootObjects.size()) {
		std::cerr << "Attempted to remove object at index " << index << " beyond vector bounds " << rootObjects.size() << std::endl;
		return;
	}
	std::cout << "Removing object at index " << index << " from set containing " << rootObjects.size() << " rootObjects";
	std::list<JBAT_Object*>::iterator iter = rootObjects.begin();
	std::advance(iter,index);
	delete *iter; // *iter contains a JBAT_Object pointer
	rootObjects.erase(iter);
	std::cout << "After removing object, the are " << rootObjects.size() << " rootObjects in the set";
}

void JBAT_ObjectSet::removeObject(JBAT_Object* object) {
	for (std::list<JBAT_Object*>::iterator iter = rootObjects.begin(); iter != rootObjects.end(); iter++) {
		if (*iter == object)
			delete *iter;
	}
	rootObjects.remove(object);
}

void JBAT_ObjectSet::removeAllObjects(void) {
	for (std::list<JBAT_Object*>::iterator iter = rootObjects.begin(); iter != rootObjects.end(); iter++) {
		delete *iter;
	}
	rootObjects.clear();
}

JBAT_Object* JBAT_ObjectSet::getObjectAtIndex(int index) {
	if (index >= rootObjects.size()) {
		std::cerr << "Attempted to get object at index " << index << " beyond vector bounds " << rootObjects.size() << std::endl;
		return NULL;
	}
	//std::cout << "In JBAT_ObjectSet getObjectAtIndex " << index;
	std::list<JBAT_Object*>::iterator iter = rootObjects.begin();
	std::advance(iter,index);
	//std::cout << "Got object: " << *(*iter);
	return *iter;
	
}

int JBAT_ObjectSet::numberOfObjects(void) {
	
	//std::cout << "In JBAT_ObjectSet::numberOfObjects returning" << rootObjects.size();
	
	return rootObjects.size();
}


std::ostream & operator<<(std::ostream& s, JBAT_ObjectSet const & objectSet) {
	
	s << "\nJBAT_ObjectSet with rootObjects: \n";
	for (std::list<JBAT_Object*>::const_iterator iter = objectSet.rootObjects.begin(); iter != objectSet.rootObjects.end(); iter++) {
		s << "\n" << **iter;
	}
	return s;
	
}