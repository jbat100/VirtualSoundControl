/*
 *  VSCSoundProperties.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSCS_PROPERTY_H_
#define _VSCS_PROPERTY_H_

#include "VSCSound.h"
#include <map>
#include <string>



class VSCSProperty {
	
public:
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter domain enum 
	 */
	
	enum Domain {
		DomainNone,
		DomainSourceGenerator,
		DomainSourceGroup,
		DomainSourceFile,
		DomainSourceFileLoop,
		DomainSourceNoise
	};
	
	/*-------------------------------------------------------------------------------------------
	 *
	 *	General sound parameter key enum 
	 */	
	
	enum Key {
		
		KeyNone,
		
		// multi chan		
		KeyChannelSetup,
		
		// File 
		KeyFilePath,
		
		// BiQuad Filter
		KeyBiQuadType,

	};
	
	
	static std::string getLabelForPropertyWithKey(Key k);
	static void setLabelForPropertyWithKey(Key k);
	
	
private:
	
	static std::map<Key, std::string> propertyLabels;

	static bool generatedPropertyLabels;
	static void generatePropertyLabels(void);
	
};

#endif
