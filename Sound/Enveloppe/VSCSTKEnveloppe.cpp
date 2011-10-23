/*
 *  VSCSTKEnveloppe.cpp
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 15/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#include "VSCSTKEnveloppe.h"
#include "VSCSound.h"

#include <math.h>
#include <iterator>

stk::StkFrames VSCSTKEnveloppe::table_;

void VSCSTKEnveloppe::fire(void) {
	
}

void VSCSTKEnveloppe::fireAfterInterval(VSCSFloat time) {
	
}

void VSCSTKEnveloppe::setCurrentTime(VSCSFloat currentTime) {
	
}

void VSCSTKEnveloppe::enveloppeChangedBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end) {
	updateTableBetweenEnveloppePoints(begin, end);
}

void VSCSTKEnveloppe::enveloppeChangedBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point) {
	updateTableBetweenEnveloppePointAndNext(point);
}

void VSCSTKEnveloppe::updateTable(void) {
	for (ConstEnvPntIter it = _points.begin(); it != _points.end(); it++) {
		updateTableBetweenEnveloppePointAndNext(*it);
	}
}

void VSCSTKEnveloppe::checkTableSize(void) {
	VSCSFloat dur = duration();
	int requiredNumberOfFrames = ceil(dur/Stk::sampleRate());
	if (table_.frames() != requiredNumberOfFrames) {
		table_.resize(requiredNumberOfFrames, 1);
	}
}

void VSCSTKEnveloppe::updateTableBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end) {
	ConstEnvPntIter beginIt = find(getPointBeginIterator(), getPointEndIterator(), begin);
	ConstEnvPntIter endIt = find(getPointBeginIterator(), getPointEndIterator(), end);
	assert(beginIt != getPointEndIterator());
	assert(endIt != getPointEndIterator());
	updateTableBetweenEnveloppePoints(beginIt, endIt);
}

void VSCSTKEnveloppe::updateTableBetweenEnveloppePoints(ConstEnvPntIter beginIt, ConstEnvPntIter endIt) {
	assert(distance(beginIt, endIt) > 0);
	if (distance(beginIt, endIt) > 0) {
		for (ConstEnvPntIter it = beginIt; it != endIt; it++) {
			updateTableBetweenEnveloppePointAndNext(it);
		}
	}
}

void VSCSTKEnveloppe::updateTableBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point) {
	ConstEnvPntIter pointIt = find(getPointBeginIterator(), getPointEndIterator(), point);
	assert(pointIt != getPointEndIterator());
	updateTableBetweenEnveloppePointAndNext(pointIt);
}

void VSCSTKEnveloppe::updateTableBetweenEnveloppePointAndNext(ConstEnvPntIter pointIt) {
	
	/*
	 *	Make sure we have exactly the right number of frames in tables_
	 */
	checkTableSize();
	
	/*
	 *	If the iterator is the end iterator then do nothing...
	 */
	EnvPntIter it = _points.end();
	if (pointIt == it) {
		return;
	}
	
	/*
	 *	If the iterator points to the last point the 
	 */
	it--;
	if (pointIt == it) {
		table_[table_.frames()-1] = (*pointIt)->getValue();
		return;
	}
	
	/*
	 *	startIt and endIt point the the two points between which the enveloppe is recalculated
	 */
	ConstEnvPntIter startIt = pointIt;
	ConstEnvPntIter endIt = startIt;
	endIt++;
	
	/*
	 *	Get the table indeces corresponding to the startIt and endIt iterators
	 *	and perform appropriate bounds checking in debug mode
	 */
	unsigned int startIndex = upperTableIndexForTime((*startIt)->getTime());
	assert(startIndex >= 0);
	assert(startIndex < table_.frames());
	unsigned int endIndex = lowerTableIndexForTime((*endIt)->getTime());
	assert(endIndex >= startIndex);
	assert(endIndex < table_.frames());
	
	/*
	 *	Fill the table linearly for now baby
	 */
	VSCSFloat startTime = (*startIt)->getTime();
	VSCSFloat endTime = (*endIt)->getTime();
	VSCSFloat startValue = (*startIt)->getValue();
	VSCSFloat endValue = (*endIt)->getValue();
	VSCSFloat timeRange = endTime - startTime;
	VSCSFloat valueRange = endValue - startValue;
	
	VSCSFloat startIndexTime = startIndex * Stk::sampleRate();
	VSCSFloat endIndexTime = endIndex * Stk::sampleRate();
	VSCSFloat startIndexValue = ((startIndexTime - startTime)*(valueRange/timeRange)) + startValue;
	VSCSFloat endIndexValue = ((endIndexTime - endTime)*valueRange) + endValue;
	VSCSFloat valueStep = (endIndexValue - startIndexValue) / (endIndex - startIndex);
	
	VSCSFloat val = startIndexValue;
	
	for (unsigned int tableIndex = startIndex; tableIndex <= endIndex; tableIndex++) {
		table_[tableIndex] = val;
		val += valueStep;
	}
}

unsigned int VSCSTKEnveloppe::upperTableIndexForTime(VSCSFloat time) {
	return ceil(time/Stk::sampleRate());
}

unsigned int VSCSTKEnveloppe::lowerTableIndexForTime(VSCSFloat time) {
	return floor(time/Stk::sampleRate());
}

void VSCSTKEnveloppe::sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate) {
	updateTable();
}