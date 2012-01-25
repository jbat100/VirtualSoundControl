/*
 *  VSCFilterBiQuad.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCFilterBiQuad.h"
#include "VSCSound.h"

#include <cmath>

VSCFilterBiQuad::VSCFilterBiQuad(void) {
	_filterType = FilterTypeLowPass;
	setParameters(100.0, 0.0, 1.0);
}

VSCFilterBiQuad::VSCFilterBiQuad(VSCFilterBiQuad::FilterType t, VSCSFloat f, VSCSFloat dB, VSCSFloat q) {
	
	if (!t)
		t = FilterTypeLowPass;
	
	_filterType = t;
	setParameters(f, dB, q);
}

VSCFilterBiQuad::FilterType VSCFilterBiQuad::getFilterType(void) {
	return _filterType;
}

void VSCFilterBiQuad::setFilterType(VSCFilterBiQuad::FilterType filterType) {
	_filterType = filterType;
}

void VSCFilterBiQuad::setParameters(VSCSFloat freq, VSCSFloat dBgain, VSCSFloat Q, bool clear) {
	
	_frequency = freq;
	_dBgain = dBgain;
	_Q = Q;
	
	switch (_filterType) {
			
		case FilterTypeLowPass:
			setToLowPassFilter(freq, dBgain, Q, clear);
			break;
			
		case FilterTypeHighPass:
			setToHighPassFilter(freq, dBgain, Q, clear);
			break;

		case FilterTypeConstantSkirtGainBandPass:
			setToConstantSkirtGainBandPassFilter(freq, dBgain, Q, clear);
			break;

		case FilterTypeConstantPeakGainBandPass:
			setToConstantPeakGainBandPassFilter(freq, dBgain, Q, clear);
			break;

		case FilterTypePeakingEQ:
			setToPeakingEQFilter(freq, dBgain, Q, clear);
			break;

		case FilterTypeHighShelf:
			setToHighShelfFilter(freq, dBgain, Q, clear);
			break;

		case FilterTypeLowShelf:
			setToLowShelfFilter(freq, dBgain, Q, clear);
			break;

		default:
			break;
			
	}
	
}

void VSCFilterBiQuad::setFrequency(VSCSFloat freq, bool clear) {
	setParameters(freq, _dBgain, _Q, clear);
}

VSCSFloat VSCFilterBiQuad::getFrequency(void) {
	return _frequency;
}

void VSCFilterBiQuad::setDBGain(VSCSFloat dBGain, bool clear) {
	setParameters(_frequency, dBGain, _Q, clear);
}

VSCSFloat VSCFilterBiQuad::getDBGain(void) {
	return _dBgain;
}

void VSCFilterBiQuad::setQ(VSCSFloat q, bool clear) {
	setParameters(_frequency, _dBgain, q, clear);
}

VSCSFloat VSCFilterBiQuad::getQ(void) {
	return _Q;
}

/*------------------------------------------------------------------------------
*	 
*	Compute a few intermediate variables:
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
*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
*
*	  LPF:        H(s) = 1 / (s^2 + s/Q + 1)
*	  
*	  b0 =  (1 - cos(w0))/2
*	  b1 =   1 - cos(w0)
*	  b2 =  (1 - cos(w0))/2
*	  a0 =   1 + alpha
*	  a1 =  -2*cos(w0)
*	  a2 =   1 - alpha
*
------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToLowPassFilter(VSCSFloat freq, VSCSFloat dBgain, VSCSFloat Q, bool clear) {
	
	//VSCSFloat A = std::sqrt(std::pow(10,(dBgain/20)));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 =  (1.0 - std::cos(w0)) / 2.0;
	stk::StkFloat b1 =   1.0 - std::cos(w0);
	stk::StkFloat b2 =  (1.0 - std::cos(w0)) / 2.0;
	stk::StkFloat a0 =   1.0 + alpha;
	stk::StkFloat a1 =  -2.0 * std::cos(w0);
	stk::StkFloat a2 =   1.0 - alpha;
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}

/*------------------------------------------------------------------------------
*
*	  HPF:        H(s) = s^2 / (s^2 + s/Q + 1)
*	 
*	  b0 =  (1 + cos(w0))/2
*	  b1 = -(1 + cos(w0))
*	  b2 =  (1 + cos(w0))/2
*	  a0 =   1 + alpha
*	  a1 =  -2*cos(w0)
*	  a2 =   1 - alpha
*	  
------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToHighPassFilter(VSCSFloat freq, VSCSFloat dBgain, VSCSFloat Q, bool clear) {
	
	//VSCSFloat A = std::sqrt(std::pow(10,(dBgain/20)));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 =  (1.0 + std::cos(w0)) / 2.0;
	stk::StkFloat b1 = -(1.0 + std::cos(w0));
	stk::StkFloat b2 =  (1.0 + std::cos(w0)) / 2.0;
	stk::StkFloat a0 =   1.0 + alpha;
	stk::StkFloat a1 =  -2.0 * std::cos(w0);
	stk::StkFloat a2 =   1.0 - alpha;
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}

/*------------------------------------------------------------------------------
*
*	  BPF:        H(s) = s / (s^2 + s/Q + 1)  (constant skirt gain, peak gain = Q)
*	  
*	  b0 =   sin(w0)/2  =   Q*alpha
*	  b1 =   0
*	  b2 =  -sin(w0)/2  =  -Q*alpha
*	  a0 =   1 + alpha
*	  a1 =  -2*cos(w0)
*	  a2 =   1 - alpha
*
*------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToConstantSkirtGainBandPassFilter(VSCSFloat freq, VSCSFloat dBgain, VSCSFloat Q, bool clear) {
	
	//VSCSFloat A = std::sqrt(std::pow(10,(dBgain/20)));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 =   std::sin(w0) / 2.0;  
	stk::StkFloat b1 =   0.0;
	stk::StkFloat b2 =  -std::sin(w0) / 2.0; 
	stk::StkFloat a0 =   1.0 + alpha;
	stk::StkFloat a1 =  -2.0 * std::cos(w0);
	stk::StkFloat a2 =   1.0 - alpha;
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}

/*------------------------------------------------------------------------------
*
*	   BPF:        H(s) = (s/Q) / (s^2 + s/Q + 1)  (constant 0 dB peak gain)
*	   
*	   b0 =   alpha
*	   b1 =   0
*	   b2 =  -alpha
*	   a0 =   1 + alpha
*	   a1 =  -2*cos(w0)
*	   a2 =   1 - alpha
*	   
*------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToConstantPeakGainBandPassFilter(VSCSFloat freq, VSCSFloat dBgain, VSCSFloat Q, bool clear) {
	
	//VSCSFloat A = std::sqrt(std::pow(10,(dBgain/20)));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 =   alpha;
	stk::StkFloat b1 =   0.0;
	stk::StkFloat b2 =  -alpha;
	stk::StkFloat a0 =   1.0 + alpha;
	stk::StkFloat a1 =  -2.0 * std::cos(w0);
	stk::StkFloat a2 =   1.0 - alpha;
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}

/*------------------------------------------------------------------------------
*	   
*	   peakingEQ:  H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
*	   
*	   b0 =   1 + alpha*A
*	   b1 =  -2*cos(w0)
*	   b2 =   1 - alpha*A
*	   a0 =   1 + alpha/A
*	   a1 =  -2*cos(w0)
*	   a2 =   1 - alpha/A
*
*------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToPeakingEQFilter(VSCSFloat freq, VSCSFloat Q, VSCSFloat dBgain, bool clear) {
	
	VSCSFloat A = std::pow(10,(dBgain/40));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 =   1.0 + (alpha*A);
	stk::StkFloat b1 =  -2.0 * std::cos(w0);
	stk::StkFloat b2 =   1.0 - (alpha*A);
	stk::StkFloat a0 =   1.0 + (alpha/A);
	stk::StkFloat a1 =  -2.0 * std::cos(w0);
	stk::StkFloat a2 =   1.0 - (alpha/A);
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}

/*------------------------------------------------------------------------------
*	   
*	   highShelf:  H(s) = A * (A*s^2 + (sqrt(A)/Q)*s + 1)/(s^2 + (sqrt(A)/Q)*s + A)
*	   
*	   b0 =    A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha )
*	   b1 = -2*A*( (A-1) + (A+1)*cos(w0)                   )
*	   b2 =    A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha )
*	   a0 =        (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha
*	   a1 =    2*( (A-1) - (A+1)*cos(w0)                   )
*	   a2 =        (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha
*
*------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToHighShelfFilter(VSCSFloat freq, VSCSFloat dBGain, VSCSFloat Q, bool clear) {
	
	VSCSFloat A = std::pow(10,(dBGain/40));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 = A * ((A+1.0) + ((A-1.0)*std::cos(w0)) + (2.0*std::sqrt(A)*alpha));
	stk::StkFloat b1 = -2.0 * A * ((A-1.0) + ((A+1.0)*std::cos(w0)));
	stk::StkFloat b2 = A * ((A+1.0) + ((A-1.0)*std::cos(w0)) - (2.0*std::sqrt(A)*alpha));
	stk::StkFloat a0 = (A+1.0) - ((A-1.0)*std::cos(w0)) + (2.0*std::sqrt(A)*alpha);
	stk::StkFloat a1 = 2.0 * ((A-1.0) - ((A+1.0)*std::cos(w0)));
	stk::StkFloat a2 = (A+1.0) - ((A-1.0)*std::cos(w0)) - (2.0*std::sqrt(A)*alpha);
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}

/*------------------------------------------------------------------------------
*	   
*	   lowShelf:   H(s) = A * (s^2 + (sqrt(A)/Q)*s + A)/(A*s^2 + (sqrt(A)/Q)*s + 1)
*	   
*	   b0 =    A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha )
*	   b1 =  2*A*( (A-1) - (A+1)*cos(w0)                   )
*	   b2 =    A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha )
*	   a0 =        (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha
*	   a1 =   -2*( (A-1) + (A+1)*cos(w0)                   )
*	   a2 =        (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha
*
*------------------------------------------------------------------------------*/

void VSCFilterBiQuad::setToLowShelfFilter(VSCSFloat freq, VSCSFloat dBGain, VSCSFloat Q, bool clear) {
	
	VSCSFloat A = std::pow(10,(dBGain/40));
	VSCSFloat w0 = 2.0 * M_PI * (freq*Stk::sampleRate());
	VSCSFloat alpha = std::sin(w0) / (2.0*Q);
	
	stk::StkFloat b0 = A * ((A+1.0) + ((A-1.0)*std::cos(w0)) + (2.0*std::sqrt(A)*alpha));
	stk::StkFloat b1 = 2.0 * A * ((A-1.0) + ((A+1.0)*std::cos(w0)));
	stk::StkFloat b2 = A * ((A+1.0) + ((A-1.0)*std::cos(w0)) - (2.0*std::sqrt(A)*alpha));
	stk::StkFloat a0 = (A+1.0) - ((A-1.0)*std::cos(w0)) + (2.0*std::sqrt(A)*alpha);
	stk::StkFloat a1 = -2.0 * ((A-1.0) - ((A+1.0)*std::cos(w0)));
	stk::StkFloat a2 = (A+1.0) - ((A-1.0)*std::cos(w0)) - (2.0*std::sqrt(A)*alpha);
	
	setCoefficients(b0, b1, b2, a0, a1, a2);
	
}



/*------------------------------------------------------------------------------
 *
 *	Other filters ...
 *
 *------------------------------------------------------------------------------	 
 *	   
 *	   notch:      H(s) = (s^2 + 1) / (s^2 + s/Q + 1)
 *	   
 *	   b0 =   1
 *	   b1 =  -2*cos(w0)
 *	   b2 =   1
 *	   a0 =   1 + alpha
 *	   a1 =  -2*cos(w0)
 *	   a2 =   1 - alpha
 *	   
 *------------------------------------------------------------------------------
 *	   
 *	   APF:        H(s) = (s^2 - s/Q + 1) / (s^2 + s/Q + 1)
 *	   
 *	   b0 =   1 - alpha
 *	   b1 =  -2*cos(w0)
 *	   b2 =   1 + alpha
 *	   a0 =   1 + alpha
 *	   a1 =  -2*cos(w0)
 *	   a2 =   1 - alpha
 *	   
 *------------------------------------------------------------------------------*/

void VSCFilterBiQuad::updateFrequencyPoints(void) {
	
}

