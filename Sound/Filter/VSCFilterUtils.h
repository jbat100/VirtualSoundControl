/*
 *  VSCSTKFilter.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_FILTER_UTILS_
#define _VSC_FILTER_UTILS_

#include "VSCFilter.h"
#include "VSCSound.h"
#include "VSCFrequencyPoint.h"

#include <vector>
#include <boost/shared_ptr.hpp>

#include <Accelerate/Accelerate.h>

#define VSCFilterUtilsPtr			boost::shared_ptr<VSCFilterUtils>

class VSCFilterUtils {
	
public:
	
	void calculateFrequencyResponse(std::vector<VSCFrequencyPoint>& response, 
									std::vector<VSCSFloat>& b, 
									std::vector<VSCSFloat>& a, 
									VSCSFloat samplingFrequency,
									int log2NumberOfPoints);
	
	
	// returns magnitude and phase (array size must be power of 2 hence passing log2ArraySize)
	void inPlaceRealForwardFft(VSCSFloat* realArray, uint32_t log2ArraySize);
	
	// clean allocations made for running FFTs (use under memory pressure)
	// the allocations are kept so as to save time when running the FFTs
	void cleanFFTAllocations(void);
	
protected:
	
	/* 
	 *	complex split and real FFTSetup used during FFT
	 *	declared as class instance so the imagp and realp dont need to be 
	 *	allocated every time an fft needs to be done, log2FFTCapacity
	 *	determines what 
	 */
#ifdef VSCS_USE_DOUBLE_PRECISION
	FFTSetupD _setupReal;
	DOUBLE_COMPLEX_SPLIT _A_COMPLEX_SPLIT; 
#else
	FFTSetup _setupReal;
	COMPLEX_SPLIT _A_COMPLEX_SPLIT; 
#endif
	uint32_t _log2FFTCapacity;
	
	void makeFFTAllocations(uint32_t log2FFTCapacity);
	
};


#endif