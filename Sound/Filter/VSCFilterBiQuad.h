/*
 *  VSCFilterBiQuad.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_FILTER_BIQUAD_
#define _VSC_FILTER_BIQUAD_

#include "VSCFilter.h"
#include "BiQuad.h"


/*
 *	This class extends the STK BiQuad implementation by providing further 
 *	configuration methods for LPF, HPF, HSF, LSF. The thechniques for computing 
 *	the fiter coefficients were taken from 
 *	
 *	http://dspwiki.com/index.php?title=Cookbook_Formulae_for_audio_EQ_biquad_filter_coefficients
 *
 *	Basic points condensed here
 *
 *------------------------------------------------------------------------------
 *
 *	 First, given a biquad transfer function defined as:
 *	
 *			 b0 + b1*z^-1 + b2*z^-2
 *	 H(z) = ------------------------                             
 *			 a0 + a1*z^-1 + a2*z^-2
 *	
 *------------------------------------------------------------------------------
 *
 *	 Begin with these user defined parameters:
 *	 
 *	 - Fs (the sampling frequency)
 *	 
 *	 - f0 ("wherever it's happenin', man."  Center Frequency or Corner Frequency, or 
 *	shelf Midpoint Frequency, depending on which filter type.  The "Significant Frequency".)
 *	 
 *	 - dBgain (used only for peaking and shelving filters)
 *	
 *	 - Q (the EE kind of definition, except for peakingEQ in which A*Q is the classic EE Q.  
 *	That adjustment in definition was made so that a boost of N dB followed by a cut of N dB 
 *	for identical Q and f0/Fs results in a precisely flat unity gain filter or "wire".)
 *	 
 *	 _or_ BW, the bandwidth in octaves (between -3 dB frequencies for BPF and notch or between 
 *	midpoint (dBgain/2) gain frequencies for peaking EQ)
 *	
 *	 _or_ S, a "shelf slope" parameter (for shelving EQ only).  When S = 1, the shelf slope is 
 *	as steep as it can be and remain monotonically increasing or decreasing gain with frequency.  
 *	The shelf slope, in dB/octave, remains proportional to S for all other values for a fixed f0/Fs 
 *	and dBgain.
 *	
 *------------------------------------------------------------------------------
 *	 
 *	Then compute a few intermediate variables:
 *	
 *	 A  =       sqrt(10^(dBgain/20))
 *	    =       10^(dBgain/40)         (for peaking and shelving EQ filters only)
 *	 
 *	 w0 = 2*pi*f0/Fs
 *	 
 *	 alpha	=	sin(w0)/(2*Q)                                 (case: Q)
 *			=	sin(w0)*sinh( ln(2)/2 * BW * w0/sin(w0) )     (case: BW)
 *			=	sin(w0)/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 )   (case: S)
 *	
 *------------------------------------------------------------------------------
 *	
 *	Coefficient calculations from there are shown in cpp file.
 *	
 */

class VSCFilterBiQuad : VSCFilter, stk::BiQuad  {
	
public:
	
	enum {
		FilterTypeNone = 0,
		FilterTypeLowPass,
		FilterTypeHighPass,
		FilterTypeConstantSkirtGainBandPass,
		FilterTypeConstantPeakGainBandPass,
		FilterTypePeakingEQ,
		FilterTypeHighShelf,
		FilterTypeLowShelf
	} FilterType;
	
	VSCFilterBiQuad(void);
	VSCFilterBiQuad(FilterType t, double f = 100.0, double dB = 0.0, double q = 1.0);
	
	FilterType getFilterType(void);
	void setFilterType(FilterType filterType);
	
	void setParameters(double freq, double dBgain, double q, bool clear = false);
	
	void setFrequency(double freq, bool clear = false);
	double getFrequency(void);
	
	void setDBGain(double dBGain, bool clear = false);
	double getDBGain(void);
	
	void setQ(double q, bool clear = false);
	double getQ(void);
	
protected:
	
	FilterType _filterType;
	double _frequency;
	double _dBgain;
	double _Q;
	
	void setToLowPassFilter(double freq, double dBgain, double Q, bool clear = false);
	void setToHighPassFilter(double freq, double dBgain, double Q, bool clear = false);
	void setToConstantSkirtGainBandPassFilter(double freq, double dBgain, double Q, bool clear = false);
	void setToConstantPeakGainBandPassFilter(double freq, double dBgain, double Q, bool clear = false);
	void setToPeakingEQFilter(double freq, double dBgain, double Q, bool clear = false);
	void setToHighShelfFilter(double freq, double dBgain, double Q, bool clear = false);
	void setToLowShelfFilter(double freq, double dBgain, double Q, bool clear = false);
	
	/*
	 *	Virtual method to update frequency responce for display
	 */
	void updateFrequencyPoints(void);
	
};


#endif