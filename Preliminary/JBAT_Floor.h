/*
 *  JBAT_Floor.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 03/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_FLOOR_
#define _JBAT_FLOOR_

#include "JBAT_Object.h"
#include "Math3d.h"

#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>



class JBAT_Floor : JBAT_Object {
	
protected:
	
	float length;
	int detail;
	
public:
	
	
	JBAT_Floor(void);
	~JBAT_Floor(void);
	
	void setToDefault(void);
	void updateMesh(void);

	
};

#endif