/*
 *  btVectorVSCAds.cpp
 *  BulletCocoaTest
 *
 *  Created by Jonathan Thorpe on 29/05/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "btVectorVSCAds.h"

std::ostream& operator<<(std::ostream& output, const btVector3& v)
{
	output << std::dec;
	output.precision(2);
	output << "("; 
	output << v.getX(); 
	output << "; ";
	output << v.getY();  
	output << "; "; 
	output << v.getY();   
	output << ")";
	return output;
}

