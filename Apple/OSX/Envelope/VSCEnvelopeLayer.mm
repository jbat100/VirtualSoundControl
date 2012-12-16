//
//  VSC::EnvelopeLayer.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCEnvelopeLayer.h"
#import "CGColor+VSCAdditions.h"
#import "NS+VSCGeomOperations.h"

#include <boost/shared_ptr.hpp>
#include <boost/assert.hpp>

@interface VSC::EnvelopeLayer ()
{
    @private
    VSC::Envelope::List _envelopeList;
}

-(void) drawEnvelope:(VSC::Envelope::SPtr)envelope withDisplaySetup:(VSC::EnvelopeGUIConfig::SPtr)displaySetup inContext:(CGContextRef)ctx;

@end


@implementation VSCEnvelopeLayer

-(id) init {
    
    if ((self = [super init]))
    {
        _defaultDisplaySetup = VSC::EnvelopeGUIConfig::SPtr(new VSC::EnvelopeGUIConfig());
    }
    
    return self;
    
}

#pragma mark - C++ Setters/Getters

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope {
    
    VSC::Envelope::List::iterator it = std::find(_envelopeList.begin(), _envelopeList.end(), envelope);
    
    if (it == _envelopeList.end())
    {
        _envelopeList.push_back(envelope);
    }
    
}

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope atIndex:(NSUInteger)index {
    NSAssert(0, @"Not Implemented");
}

-(void) removeEnvelope:(VSC::Envelope::SPtr)envelope {
    _envelopeList.remove(envelope);
}

-(VSC::EnvelopeGUIConfig::SPtr) getDefaultDisplaySetup {
    return _defaultDisplaySetup;
}

-(void) setDefaultDisplaySetup:(VSC::EnvelopeGUIConfig::SPtr)setup {
    _defaultDisplaySetup = setup;
}


-(void) setDisplaySetup:(VSC::EnvelopeGUIConfig::SPtr)setup forEnvelope:(VSC::Envelope::SPtr)envelope {
    _envelopeDisplaySetupMap[envelope] = setup;
}

-(VSC::EnvelopeGUIConfig::SPtr) getDisplaySetupForEnvelope:(VSC::Envelope::SPtr)envelope {
    
    // Look for display setup in the map
    VSC::EnvelopeGUIConfigMap::iterator setupIt = _envelopeDisplaySetupMap.find(envelope);
    if (setupIt != _envelopeDisplaySetupMap.end()) {
        return setupIt->second;
    }
    // if not in map, return the default
    return  _defaultDisplaySetup;
}
        
#pragma mark - Drawing

- (void)drawInContext:(CGContextRef)ctx {
    
    drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
    
    /*
     *  First draw non editable envelopes
     */
    for (VSC::Envelope::List::const_iterator it = _envelopeList.begin(); it != _envelopeList.end(); ++it) {
        VSC::Envelope::SPtr envelope = (*it);
        if ([self.editor envelopeIsEditable:envelope] == NO) {
            VSC::EnvelopeGUIConfig::SPtr displaySetup = [self getDisplaySetupForEnvelope:envelope];
            [self drawEnvelope:envelope withDisplaySetup:displaySetup inContext:ctx];
        }
    }
    
    /*
     *  Then draw editable envelopes and selection rects
     */
    CGFloat selectionRectWidth = (CGFloat)[self.editor envelopeEditorGUIConfig]->getSelectionRectLineWidth();
    const VSC::Color selectionRectColour = [self.editor envelopeEditorGUIConfig]->getSelectionRectColour();
    CGColorRef cgSelectionRectColour = CGColorCreateFromVSC::Color(selectionRectColour);
    
    for (VSC::Envelope::List::const_iterator it = _envelopeList.begin(); it != _envelopeList.end(); ++it) {
        VSC::Envelope::SPtr envelope = (*it);
        if ([self.editor envelopeIsEditable:envelope] == YES) {
            VSC::EnvelopeGUIConfig::SPtr displaySetup = [self getDisplaySetupForEnvelope:envelope];
            [self drawEnvelope:envelope withDisplaySetup:displaySetup inContext:ctx];
        }
        drawRectOutline(ctx, [self.editor currentSelectionRectForEnvelope:envelope], selectionRectWidth, cgSelectionRectColour);
    }
    
}

-(void) drawEnvelope:(VSC::Envelope::SPtr)envelope withDisplaySetup:(VSC::EnvelopeGUIConfig::SPtr)displaySetup inContext:(CGContextRef)ctx {
    
    VSC::EnvelopeEditorGUIConfig::SPtr editorConfig = [self.editor envelopeEditorGUIConfig];
    
    CGFloat radius = (CGFloat)(displaySetup->getControlPointRadius());
    CGFloat lineWidth = (CGFloat)(displaySetup->getLineWidth());
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSC::Color(displaySetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSC::Color(displaySetup->getControlPointUnselectedColour());
	CGColorRef cgLineSelectedColorRef = CGColorCreateFromVSC::Color(displaySetup->getLineSelectedColour());
    CGColorRef cgLineUnselectedColorRef = CGColorCreateFromVSC::Color(displaySetup->getLineUnselectedColour());
    
    VSC::Envelope::ConstPointIterator nextIt;
	VSC::Envelope::ConstPointIterator endIt = envelope->getPointEndIterator();
    
    /*
     *  Draw envelope lines
     */
    
    BOOL isEditable = [self.editor envelopeIsEditable:envelope];
    if (isEditable == NO) {
        CGContextSetStrokeColorWithColor(ctx, cgLineUnselectedColorRef);
    }
    
    CGContextSetLineWidth(ctx, lineWidth);
    
	for (VSC::Envelope::ConstPointIterator it = envelope->getPointBeginIterator(); it !=endIt; ++it) {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != endIt) {
            
            VSC::EnvelopePoint::SPtr currentPoint = *it;
			VSC::EnvelopePoint::SPtr nextPoint = *nextIt;
            
            // if the envelope is not editable, we don't even need to check whether points are selected or not, we stay in unselected color
            if (isEditable == YES) {
                // if both the current point and the next point are selected then draw line in selected color
                if ([self.editor pointIsSelected:currentPoint] && [self.editor pointIsSelected:nextPoint]) {
                    CGContextSetStrokeColorWithColor(ctx, cgLineSelectedColorRef);
                }
                else {
                    CGContextSetStrokeColorWithColor(ctx, cgLineUnselectedColorRef);
                }
            }
			
			// draw line between this point and next
			
			//NSPoint point1 = NSMakePointFromPoint(editorConfig->pointForEnvelopeCoordinate(boost::static_pointer_cast<VSC::EnvelopeCoordinate>(currentPoint)));
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
    
    if (isEditable == NO) {
        CGContextSetFillColorWithColor(ctx, cgUnselectedColourRef); 
    }
    
    CGContextSetLineWidth(ctx, 1);
    
    
	for (VSC::Envelope::ConstPointIterator it = envelope->getPointBeginIterator(); it !=endIt; it++) {
		
		VSC::EnvelopePoint::SPtr currentPoint = *it;
		
		// draw control circle for point
		
        VSC::Point vscp = [self.editor envelopeEditorGUIConfig]->pointForEnvelopeCoordinate(boost::dynamic_pointer_cast<VSC::EnvelopeCoordinate>(currentPoint));
		NSPoint p =  NSMakePointFromPoint(vscp);
		
        if (isEditable == YES) {
            if ([self.editor pointIsSelected:currentPoint]) {
                CGContextSetFillColorWithColor(ctx, cgSelectedColourRef);  
            }
            else {
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
