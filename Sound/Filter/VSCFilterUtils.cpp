/*
 *  VSCSTKFilter.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/10/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCFilterUtils.h"
#include "VSCSound.h"

#include <cstdlib> // for malloc
#include <vector>
#include <algorithm>
#include <Accelerate/Accelerate.h>

void VSCFilterUtils::calculateFrequencyResponse(std::vector<VSCFrequencyPoint>& response, 
												   std::vector<VSCSFloat>& b, 
												   std::vector<VSCSFloat>& a, 
												   VSCSFloat samplingFrequency,
												   int log2NumberOfPoints) 
{
	
	
	/*
	 *	According to some pretty reputable people the elements of a vector are garantied to be 
	 *	contiguous, which is pretty cool as we want that to be able to use the vDSP functions
	 *
	 *	http://stackoverflow.com/questions/3787074/convert-stdvector-to-array
	 *
	 *	int array[256];
	 *	object->getArray(array);
	 *
	 *	std::vector<int> array(256); // resize the buffer to 256 ints
	 *	object->getArray(&array[0]); // pass address of that buffer
	 *
	 */
	
	uint32_t log2n = log2NumberOfPoints;
    uint32_t n = 1 << log2NumberOfPoints; // do a bit shift, less expensive and pretty :)
	uint32_t nOver2 = n/2;
	
	response.resize(nOver2);
	
	// make zero padded vectors
	std::vector<VSCSFloat> padded_b;
	std::vector<VSCSFloat> padded_a;
	padded_b.resize(n);
	padded_a.resize(n);
	// fill with zeros
	std::fill(padded_a.begin(), padded_b.end(), 0);
	std::fill(padded_a.begin(), padded_a.end(), 0);
	// copy the filter coefficients
	std::copy(b.begin(), b.end(), padded_b.begin());
	std::copy(a.begin(), a.end(), padded_a.begin());
	
	// turn them into arrays for clarity
	VSCSFloat* b_array = (VSCSFloat*) &padded_a[0];
	VSCSFloat* a_array = (VSCSFloat*) &padded_a[0];
	
	
	/*---------------------------------------------------------------------------------------
	 *	http://www.bores.com/courses/intro/iir/5_resp.htm
	 *	http://www.exstrom.com/journal/sigproc/
	 *
	 *	Important bits of the C code from above kept and distilled for model...
	 *
	 *	c are the non recursive coefficients
	 *	d are the recursive coefficients
	 *	Declare the variables
	 
		int i;
		int np = numberOfPoints; // number of FFT points 
		int nc = ; // number of c coefficients in input file 
		int nd; // number of d coefficients in input file 
		double *c; // where c coefficients are stored, and where its FFT is stored 
		double *d; // where d coefficients are stored, and where its FFT is stored 
		double mc, pc; // magnitude & phase of ck coeff 
		double md, pd; // magnitude & phase of dk coeff 
	 
	 *---------------------------------------------------------------------------------------*/
	
	
	/*---------------------------------------------------------------------------------------
	 *	Permorm a forward fourier transform over the specified number of points 
	 *	c and d are padded with zeros 
		FFT(c, np, 1);
		FFT(d, np, 1);
	 *---------------------------------------------------------------------------------------*/
	
	inPlaceRealForwardFft(a_array, log2n);
	inPlaceRealForwardFft(b_array, log2n);
	
	/*---------------------------------------------------------------------------------------
	 *	Convert to magnitude and phase for both c and d
	 *	Then calculate the final freq response by dividing the c magnitudes by the d magnitudes
	 *	and subtracting the d phases from the c phases, storing the result in c
	 *
	 
		for( i = 0; i < np; ++i )
		{
			// convert complex numbers to magnitude and phase 
			mc = sqrt( c[2*i]*c[2*i] + c[2*i+1]*c[2*i+1] );
			pc = atan2( c[2*i+1], c[2*i] );
			md = sqrt( d[2*i]*d[2*i] + d[2*i+1]*d[2*i+1] );
			pd = atan2( d[2*i+1], d[2*i] );
			// divide the FFT's and store the result in c 
			c[2*i] = mc/md;
			c[2*i+1] = pc-pd;
		}
	*---------------------------------------------------------------------------------------*/
	
	// Divide non recursive magnitudes by recursive magnitudes
	// do it in place to save having to do extra allocations ...
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_vdivD(b_array, 2, a_array, 2, a_array, 2, nOver2);
#else
		vDSP_vdiv(b_array, 2, a_array, 2, a_array, 2, nOver2);
#endif
	
	// Subtract recursive phases from non recursive phases 
	// do it in place to save having to do extra allocations ...
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_vsubD(b_array+1, 2, a_array+1, 2, a_array+1, 2, nOver2);
#else
		vDSP_vsub(b_array+1, 2, a_array+1, 2, a_array+1, 2, nOver2);
#endif
	

	
}


void VSCFilterUtils::inPlaceRealForwardFft(VSCSFloat* realArray, uint32_t log2ArraySize) {
	
	if (log2ArraySize < 2) {
		std::cout << "\nLog2 array size is too small";
		// throw...
	}
	
	uint32_t log2n = log2ArraySize;
    uint32_t n = 1 << log2ArraySize; // do a bit shift, less expensive and pretty :)
	uint32_t nOver2 = n/2;
	
	if (_log2FFTCapacity < log2ArraySize) {
		makeFFTAllocations(log2ArraySize);
	}
	
	//double* doubleArray = realArray;
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_ctozD((DOUBLE_COMPLEX *) realArray, 2, &_A_COMPLEX_SPLIT, 1, nOver2);
#else
		vDSP_ctoz((COMPLEX *) realArray, 2, &_A_COMPLEX_SPLIT, 1, nOver2);
#endif
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		// run the FFT on the A and B padded coefficient arrays
		vDSP_fft_zripD(_setupReal, &_A_COMPLEX_SPLIT, 1, log2n, FFT_FORWARD);
#else
		vDSP_fft_zrip(_setupReal, &_A_COMPLEX_SPLIT, 1, log2n, FFT_FORWARD);
#endif
	
	
	/* 
	 *	To make things simple lets not worry about the highest frequency component which for technical
	 *	reasons is packed with the first element (see packing description in vDSP guide).  
	 */
	_A_COMPLEX_SPLIT.imagp[0] = 0.0;
	
	// use vDSP_zvabsD to get the magnitude of the split complex and store it into the original arrays
	// vDSP_zvabsD(&A_COMPLEX_SPLIT, 1, (double*)a_array, 1)
	
	
	
	/*---------------------------------------------------------------------------
	 *	METHOD 1
	 *	The output signal is now in a split real form.  Use the  function
     *	vDSP_ztoc to get a split real vector. 
	 
	if (typeid(VSCSFloat) == typeid(double)) {
		vDSP_ztocD(&A_COMPLEX_SPLIT, 1, (COMPLEX*) a_array, 2, nOver2);
	}
	else if (typeid(VSCSFloat) == typeid(float)) {
		vDSP_ztoc(&A_COMPLEX_SPLIT, 1, (COMPLEX*) a_array, 2, nOver2);
	}
	 */
	/*
	 *	Then convert from rect to polar coordinates
	 *---------------------------------------------------------------------------*/
	
	
	
	/*---------------------------------------------------------------------------
	 *	METHOD 1
	 *	use vDSP_zvabs to get the magnitude of the split complex, and put them in a_array with stride 2
	 */
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_zvabsD(&_A_COMPLEX_SPLIT, 1, realArray, 2, nOver2);
#else
		vDSP_zvabs(&_A_COMPLEX_SPLIT, 1, realArray, 2, nOver2);
#endif
	
	// Scale as stated in the guide
	VSCSFloat scale = 0.5;
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_vsmulD(realArray, 2, &scale, realArray, 2, nOver2);
#else
		vDSP_vsmul(realArray, 2, &scale, realArray, 2, nOver2);
#endif
	
	// use vDSP_zvphasD to get the phase of the split complex, , and put them in a_array+1 with stride 2
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_zvphasD(&_A_COMPLEX_SPLIT, 1, realArray+1, 2, nOver2);
#else
		vDSP_zvabs(&_A_COMPLEX_SPLIT, 1, realArray+1, 2, nOver2);
#endif
	
	/*---------------------------------------------------------------------------*/
	
	
}

void VSCFilterUtils::cleanFFTAllocations(void) {
	
	if (_setupReal) {
#ifdef VSCS_USE_DOUBLE_PRECISION
		vDSP_destroy_fftsetupD (_setupReal);
#else
		vDSP_destroy_fftsetup (_setupReal);
#endif
		_setupReal = NULL;
	}
	
	if (_A_COMPLEX_SPLIT.realp) {
		free (_A_COMPLEX_SPLIT.realp);
		_A_COMPLEX_SPLIT.realp = NULL;
	}
	
	if (_A_COMPLEX_SPLIT.imagp) {
		free (_A_COMPLEX_SPLIT.imagp);
		_A_COMPLEX_SPLIT.imagp = NULL;
	}
	
	_log2FFTCapacity = 0;
	
}

void VSCFilterUtils::makeFFTAllocations(uint32_t log2FFTCapacity) {
	
	cleanFFTAllocations();
	
	uint32_t log2n = log2FFTCapacity;
    uint32_t n = 1 << log2FFTCapacity; // do a bit shift, less expensive and pretty :)
	uint32_t nOver2 = n/2;
	
	/* 
	 *	Allocate memory for the input operands and check its availability,
     *	use the vector version to get 16-byte alignment. 
	 */
    _A_COMPLEX_SPLIT.realp = (VSCSFloat *) malloc(nOver2 * sizeof(VSCSFloat));
    _A_COMPLEX_SPLIT.imagp = (VSCSFloat *) malloc(nOver2 * sizeof(VSCSFloat));
	
	if (_A_COMPLEX_SPLIT.realp == NULL || _A_COMPLEX_SPLIT.imagp == NULL) {
		// cleanup / throw
        return;
    }
	
#ifdef VSCS_USE_DOUBLE_PRECISION
		_setupReal = vDSP_create_fftsetupD(log2n, FFT_RADIX2); 
#else
		_setupReal = vDSP_create_fftsetup(log2n, FFT_RADIX2); 
#endif

	
	_log2FFTCapacity = log2FFTCapacity;
	
}
