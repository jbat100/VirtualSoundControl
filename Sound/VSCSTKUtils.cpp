/*
 *  VSCSTKUtils.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSTKUtils.h"

#include "VSCSynthSourceGroup.h"
#include "VSCSynthSourceFile.h"
#include "VSCSynthSourceFileLoop.h"
#include "VSCSynthSourceSine.h"
#include "VSCSynthSourceSaw.h"
#include "VSCSynthSourceSquare.h"

stk::Generator* castVSCSynthSourceElementToStkGenerator(VSCSynthSourceElement* elem) {
	
	VSCSynthSourceGroup* group = dynamic_cast<VSCSynthSourceGroup*> (elem);
	if (group)
		return group;
	
	VSCSynthSourceSine* sine = dynamic_cast<VSCSynthSourceSine*> (elem);
	if (sine)
		return sine;
	
	VSCSynthSourceSaw* saw = dynamic_cast<VSCSynthSourceSaw*> (elem);
	if (saw)
		return saw;
	
	VSCSynthSourceSquare* square = dynamic_cast<VSCSynthSourceSquare*> (elem);
	if (square)
		return square;
	
	return NULL;

}

stk::FileWvIn* castVSCSynthSourceElementToStkFileWvIn(VSCSynthSourceElement* elem) {
	
	VSCSynthSourceFile* file = dynamic_cast<VSCSynthSourceFile*> (elem);
	if (file)
		return file;
	
	return NULL;
	
}

stk::FileLoop* castVSCSynthSourceElementToStkFileLoop(VSCSynthSourceElement* elem) {

	VSCSynthSourceFileLoop* loop = dynamic_cast<VSCSynthSourceFileLoop*> (elem);
	if (loop)
		return loop;
	
	return NULL;
	
}
