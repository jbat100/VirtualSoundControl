//
//  VSCMath.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 8/9/11.
//  Copyright 2011 NXP. All rights reserved.
//

namespace vsc {
	
	const double kPI = 3.1415926535897932384626433832795028;

	template <typename T>
	T degreesToRadians(T deg) {return (deg/180.0)*kPI;}
	
	template <typename T>
	T radiansToDegrees(T rad) {return (rad/kPI)*180.0;}
	
	template <typename T>
	T normalizedToRadians(T norm) {return norm*(2*kPI);}
	
	template <typename T>
	T radiansToNormalized(T rad) {return rad/(2*kPI);}
	
}