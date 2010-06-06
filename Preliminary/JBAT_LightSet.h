/*
 *  BaseGLViewComponents.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 31/03/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_LIGHT_SET_
#define _JBAT_LIGHT_SET_

#include "JBAT_Light.h"
#include <iostream>
#include <list>
#include "Math3d.h"

#define DEFAULT_LIGHT_POSITION {-20.0, 0.0, 20.0}

class JBAT_LightSet {
	
protected:
	
	// used for main "background radiation" ambient lighting, independent of individual positioned lights
	M3DVector4f ambientColor; 
	std::list<JBAT_Light*> lights;
	
public:
	JBAT_LightSet(void);
	~JBAT_LightSet(void);
	
	// LIGHTS
	bool addLight(void);
	bool addLight(JBAT_Light* light);
	void removeLightAtIndex(int index);
	void removeLight(JBAT_Light* light);
	void removeAllLights(void);
	JBAT_Light* getLightAtIndex(int index);
	int numberOfLights(void);
	
	friend std::ostream & operator<<(std::ostream& s, JBAT_LightSet const & components);
	
};



#endif