//
//  NSLogDefinitions.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 01/10/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#ifdef VSC_DEBUG
	#define NSLog(...) NSLog(__VA_ARGS__)
#else
	#define NSLog(...) void(0)
#endif

#ifdef VSC_DEBUG_COCOA_DRAW
	#define NSDrawLog(...) NSLog(__VA_ARGS__)
#else
	#define NSDrawLog(...) void(0)
#endif
