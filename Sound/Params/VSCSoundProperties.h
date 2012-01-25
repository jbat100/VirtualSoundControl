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

#include <set>
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
	
	enum Code {
		
		CodeNone,
		
		CodeSoundElementType,
        CodeSoundElementIdentifier,
		
		// multi chan		
		CodeChannelSetup,
		
		// File 
		CodeFilePath,
		
		// BiQuad Filter
		CodeBiQuadType,
		
		CodeAll

	};
	
	struct Key {
		Domain domain;
		Code code;
		bool operator<(const Key& otherKey) const;
	};
    
    typedef std::set<Key>                   KeySet;
	
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
	
	KeyLabelMap customizedKeyLabels;
	
};

#endif
