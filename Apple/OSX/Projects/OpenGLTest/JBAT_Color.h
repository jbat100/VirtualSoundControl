/*
 *  JBAT_Color.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 02/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_COLOR_
#define _JBAT_COLOR_

#include "math3d.h"


class JBAT_Color {
	
protected:
	
	M3DVector4f color;
	
public:
	
	JBAT_Color(void) {color = {1.0, 1.0, 1.0, 1.0};}
	JBAT_Color(float r, float g, float b) {color[0] = r; color[1] = g; color[2] = b; color[3] = 1.0;}
	JBAT_Color(float r, float g, float b, float a) {color[0] = r; color[1] = g; color[2] = b; color[3] = a;}
	JBAT_Color(JBAT_Color *col) {m3dCopyVector3(color, col->color);}
	
	inline void setToDefault(void) {color = {1.0, 1.0, 1.0, 1.0};}
	
	inline void setTo(JBAT_Color *col) {m3dCopyVector3(color, col->color);}
	inline void setTo(const M3DVector4f col) {m3dCopyVector3(color, col);};
	inline void setTo(float r, float g, float b) {color[0] = r; color[1] = g; color[2] = b; color[3] = 1.0;}
	inline void setTo(float r, float g, float b, float a) {color[0] = r; color[1] = g; color[2] = b; color[3] = a;}
	
	inline float getR(void) {return color[0];}
	inline float getG(void) {return color[1];}
	inline float getB(void) {return color[2];}
	inline float getA(void) {return color[3];}
	
	inline void copyTo(JBAT_Color *col) {m3dCopyVector3(col->color, color);}
	inline void copyTo(M3DVector4f col) {m3dCopyVector3(col, color);}
	

};

#endif

