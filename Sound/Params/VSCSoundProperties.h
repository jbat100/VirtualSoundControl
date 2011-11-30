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
		
		DomainSoundElement,
		
		DomainMultiChannelElement,
		
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
		
		KeySoundElementType,
		
		// multi chan		
		KeyChannelSetup,
		
		// File 
		KeyFilePath,
		
		// BiQuad Filter
		KeyBiQuadType,
		
		KeyAll

	};
	
	typedef std::map<Key, std::string>		KeyLabelMap;
	typedef std::pair<Key, std::string>		KeyLabelPair;
	
	/*
	 *	Singleton instance
	 */
	static VSCSProperty& sharedInstance(void);
	
	VSCSProperty();
	~VSCSProperty();
	
	
	std::string getLabelForPropertyWithKey(Key k);
	void setLabelForPropertyWithKey(std::string label, Key k);
	
	
private:
	
	KeyLabelMap keyLabelMap;

	void generatePropertyLabels(void);
	
};

#endif
