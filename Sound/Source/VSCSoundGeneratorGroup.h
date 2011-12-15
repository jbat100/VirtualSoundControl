/*
 *  VSCSoundGeneratorGroup.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 01/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SYNTH_SOURCE_GROUP_
#define _VSC_SYNTH_SOURCE_GROUP_

#include "VSCSound.h"
#include "VSCSoundGenerator.h"

#include <list>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#define VSCSoundGeneratorGroupPtr      boost::shared_ptr<VSCSoundGeneratorGroup>



class VSCSoundGeneratorGroup : public VSCSoundGenerator {
			
public:
	
	virtual void addGenerator(VSCSoundGeneratorPtr elem);
	virtual void removeGenerator(VSCSoundGeneratorPtr elem);
	
	const std::list<VSCSoundGeneratorPtr>& generators(void);
    
    virtual void initialize(void);
	
protected:
	
	std::list<VSCSoundGeneratorPtr> _generators;
	
};




#endif // _VSC_SYNTH_SOURCE_GROUP_