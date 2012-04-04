/*
 *  JBAT_ObjectSet.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 25/05/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_OBJECT_SET_
#define _JBAT_OBJECT_SET_

#include "JBAT_Object.h"
#include <iostream>
#include <list>


class JBAT_ObjectSet {
	
protected:
	
	std::list<JBAT_Object*> rootObjects;
	
public:
	JBAT_ObjectSet(void);
	~JBAT_ObjectSet(void);
	
	// LIGHTS
	void addObject();
	void addObject(JBAT_Object* light, bool duplicate);
	void removeObjectAtIndex(int index);
	void removeObject(JBAT_Object* object);
	void removeAllObjects(void);
	JBAT_Object* getObjectAtIndex(int index);
	int numberOfObjects(void);
	
	friend std::ostream & operator<<(std::ostream& s, JBAT_ObjectSet const & objectSet);
	
};



#endif