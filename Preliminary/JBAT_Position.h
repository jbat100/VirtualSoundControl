/*
 *  JBAT_Position.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 02/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_POSITION_
#define _JBAT_POSITION_

#include "math3d.h"


class JBAT_Position {
	
protected:
	
	M3DVector3f position;
	
public:
	
	JBAT_Position(void); {position = {0.0, 0.0, 0.0};}
	JBAT_Position(float x, float y, float z); {position[0] = x; position[1] = y; position[2] = z;}
	JBAT_Position(const M3DVector3f pos); {m3dCopyVector3(position, pos);}
	
	inline void setToDefault(void) {position = {1.0, 1.0, 1.0, 1.0};}
	
	inline void setTo(const M3DVector3f pos) {m3dCopyVector3(position, col);};
	inline void setTo(float x, float y, float z) {position[0] = x; position[1] = y; position[2] = z;}
	
	inline float getX(void) {return position[0];}
	inline float getY(void) {return position[1];}
	inline float getZ(void) {return position[2];}
	
	inline void copyTo(M3DVector3f pos) {m3dCopyVector3(pos, position);}
	
	
};

#endif