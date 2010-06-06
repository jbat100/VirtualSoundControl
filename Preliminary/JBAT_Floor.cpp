/*
 *  JBAT_Floor.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 03/04/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_Floor.h"
#include <iostream>

JBAT_Floor::JBAT_Floor(void) {
	
	setToDefault();
	
}


// DEFAULT

void JBAT_Floor::setToDefault(void) {
	
	on = true;
	length = 100.0;
	detail = 10;
	
}