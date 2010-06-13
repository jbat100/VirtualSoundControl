/*
 *  JBAT_MathTools.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 03/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_MathTools.h"
#include <math.h>

float fullRangedAngle (float angle) {
	
	if (angle > M_PI) {
		while (angle > M_PI) {
			angle -= (2*M_PI);
		}
	}
	
	else if (angle < -M_PI) {
		while (angle < -M_PI) {
			angle += (2*M_PI);
		}
	}
	
	return angle;
}

float halfRangedAngle (float angle) {
	
	angle = fullRangedAngle(angle);
	
	if (angle > (M_PI/2.0)) {
		angle = M_PI/2.0;
	}
	
	else if (angle < (-M_PI/2.0)) {
		angle = -M_PI/2.0;
	}

	return angle;
}

