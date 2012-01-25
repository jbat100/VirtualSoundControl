/*
 *  JBAT_Scene.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 24/05/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_SCENE_
#define _JBAT_SCENE_


#include "JBAT_LightSet.h"
#include "JBAT_Object.h"

#include <iostream>
#include <vector>
#include <list>



class JBAT_Scene {
	
protected:
	
	JBAT_LightSet *lightSet;
	std::list<JBAT_Object*> rootObjects;
	
public:
	
	JBAT_Scene();
	
	
	

};

#endif