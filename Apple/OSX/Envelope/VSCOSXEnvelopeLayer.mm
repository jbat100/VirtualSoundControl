//
//  VSCOSXEnvelopeLayer.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXEnvelopeLayer.h"
#import "CGColor+VSCAdditions.h"
#import "NS+VSCGeomOperations.h"

#include "VSCEnvelope.h"
#include "VSCEnvelopeGUI.h"

#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>

using namespace VSC;

@interface VSCOSXEnvelopeLayer ()
{
    @private
    Envelopes _envelopes;
}

-(void) drawEnvelope:(Envelope_SPtr)envelope withGUIConfig:(EnvelopeGUIConfig_SPtr)displaySetup inContext:(CGContextRef)ctx;

-(EnvelopeGUIConfig_SPtr) guiConfigForEnvelope:(Envelope_SPtr)envelope;

@end


@implementation VSCOSXEnvelopeLayer

-(id) init {
    
    if ((self = [super init]))
    {
        _defaultEnvelopeGUIConfig = EnvelopeGUIConfig_SPtr(new EnvelopeGUIConfig());
        _defaultEnvelopeEditorGUIConfig = EnvelopeEditorGUIConfig_SPtr(new EnvelopeEditorGUIConfig());
    }
    
    return self;
    
}
        
#pragma mark - Drawing

-(EnvelopeGUIConfig_SPtr) guiConfigForEnvelope:(Envelope_SPtr)envelope
{
    EnvelopeGUIConfig_SPtr guiConfig;
    if ([self editor])
    {
        return [[self editor] GUIConfigForEnvelope:envelope];
    }
    else
    {
        return _defaultEnvelopeGUIConfig;
    }
}

- (void)drawInContext:(CGContextRef)ctx
{
    
    drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
    
    /*
     *  First draw non editable envelopes
     */
    
    for (Envelopes::const_iterator it = _envelopes.begin(); it != _envelopes.end(); ++it)
    {
        Envelope_SPtr envelope = (*it);
        if ([self.editor envelopeIsEditable:envelope] == NO)
        {
            [self drawEnvelope:envelope withGUIConfig:[self guiConfigForEnvelope:envelope] inContext:ctx];
        }
    }
    
    /*
     *  Then draw editable envelopes and selection rects
     */
    
    CGFloat selectionRectWidth = (CGFloat)[self.editor envelopeEditorGUIConfig]->getSelectionRectLineWidth();
    const Color selectionRectColour = [self.editor envelopeEditorGUIConfig]->getSelectionRectColour();
    CGColorRef cgSelectionRectColour = CGColorCreateFromVSCColor(selectionRectColour);
    
    for (Envelopes::const_iterator it = _envelopes.begin(); it != _envelopes.end(); ++it)
    {
        Envelope_SPtr envelope = (*it);
        if ([self.editor envelopeIsEditable:envelope] == YES)
        {
            [self drawEnvelope:envelope withGUIConfig:[self guiConfigForEnvelope:envelope] inContext:ctx];
        }
        drawRectOutline(ctx, [self.editor currentSelectionRectForEnvelope:envelope], selectionRectWidth, cgSelectionRectColour);
    }
    
}

-(void) drawEnvelope:(Envelope_SPtr)envelope withGUIConfig:(EnvelopeGUIConfig_SPtr)displaySetup inContext:(CGContextRef)ctx
{
    
    EnvelopeEditorGUIConfig_SPtr editorConfig = [self.editor envelopeEditorGUIConfig];
    
    CGFloat radius = (CGFloat)(displaySetup->getControlPointRadius());
    CGFloat lineWidth = (CGFloat)(displaySetup->getLineWidth());
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSCColor(displaySetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSCColor(displaySetup->getControlPointUnselectedColour());
	CGColorRef cgLineSelectedColorRef = CGColorCreateFromVSCColor(displaySetup->getLineSelectedColour());
    CGColorRef cgLineUnselectedColorRef = CGColorCreateFromVSCColor(displaySetup->getLineUnselectedColour());
    
    EnvelopePoints::const_iterator nextIt;
	EnvelopePoints::const_iterator endIt = envelope->getPoints().end();
    
    /*
     *  Draw envelope lines
     */
    
    BOOL isEditable = [self.editor envelopeIsEditable:envelope];
    
    if (isEditable == NO)
    {
        CGContextSetStrokeColorWithColor(ctx, cgLineUnselectedColorRef);
    }
    
    CGContextSetLineWidth(ctx, lineWidth);
    
    const EnvelopePointSet& selectedPoints = [self.editor selectedEnvelopePointsForEnvelope:envelope];
    
	for (EnvelopePoints::const_iterator it = envelope->getPoints().begin(); it != endIt; ++it)
    {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != endIt)
        {
            
            EnvelopePoint_SPtr currentPoint = *it;
			EnvelopePoint_SPtr nextPoint = *nextIt;
            
            // if the envelope is not editable, we don't even need to check whether points are selected or not, we stay in unselected color
            if (isEditable == YES)
            {
                // if both the current point and the next point are selected then draw line in selected color
                if ([self.editor selectedEnvelopePointsForEnvelope:currentPoint] &&
                    [self.editor selectedEnvelopePointsForEnvelope:nextPoint])
                {
                    CGContextSetStrokeColorWithColor(ctx, cgLineSelectedColorRef);
                }
                else
                {
                    CGContextSetStrokeColorWithColor(ctx, cgLineUnselectedColorRef);
                }
            }
			
			// draw line between this point and next
			
			//NSPoint point1 = NSMakePointFromPoint(editorConfig->pointForEnvelopeCoordinate(boost::static_pointer_cast<EnvelopeCoordinate>(currentPoint)));
			NSPoint point1 = NSMakePointFromPoint(editorConfig->pointForEnvelopeCoordinate(currentPoint));
            NSPoint point2 = NSMakePointFromPoint(editorConfig->pointForEnvelopeCoordinate(nextPoint));
			
			//NSDrawLog(@"Converted points to %@ and %@", NSStringFromPoint(point1), NSStringFromPoint(point2));
			
			// Background
			CGMutablePathRef linePath = CGPathCreateMutable(); 
			CGPathMoveToPoint(linePath, NULL, point1.x, point1.y);
			CGPathAddLineToPoint(linePath, NULL, point2.x, point2.y); 
			CGContextAddPath(ctx, linePath);
			CGContextStrokePath(ctx);
			CGPathRelease(linePath);
			
		}
		
	}
    
    /*
     *  Draw envelope points
     */
    
    if (isEditable == NO)
    {
        CGContextSetFillColorWithColor(ctx, cgUnselectedColourRef); 
    }
    
    CGContextSetLineWidth(ctx, 1);
    
    
	for (Envelope::ConstPointIterator it = envelope->getPointBeginIterator(); it !=endIt; it++)
    {
		
		EnvelopePoint_SPtr currentPoint = *it;
		
		// draw control circle for point
		
        Point vscp = [self.editor envelopeEditorGUIConfig]->pointForEnvelopeCoordinate(boost::dynamic_pointer_cast<EnvelopeCoordinate>(currentPoint));
		NSPoint p =  NSMakePointFromPoint(vscp);
		
        if (isEditable == YES)
        {
            if ([self.editor selectedEnvelopePointsForEnvelope:currentPoint])
            {
                CGContextSetFillColorWithColor(ctx, cgSelectedColourRef);  
            }
            else
            {
                CGContextSetFillColorWithColor(ctx, cgUnselectedColourRef);  
            }
        }
		
		CGMutablePathRef dotPath = CGPathCreateMutable(); 
		CGPathAddEllipseInRect(dotPath, NULL, CGRectMake(p.x - radius, p.y - radius, 2.0*radius, 2.0*radius));
		CGContextAddPath(ctx, dotPath);
		CGContextFillPath(ctx);
		CGPathRelease(dotPath);
		
	}
    
}


@end
