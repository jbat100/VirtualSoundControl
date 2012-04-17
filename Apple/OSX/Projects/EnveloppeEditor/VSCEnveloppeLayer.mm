//
//  VSCEnveloppeLayer.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCEnveloppeLayer.h"

#import "CGColor+VSCAdditions.h"
#import <boost/shared_ptr.hpp>

@interface VSCEnveloppeLayer ()

-(void) drawEnveloppe:(VSCEnveloppePtr)enveloppe withDisplaySetup:(VSCEnveloppeGUIConfigPtr)displaySetup inContext:(CGContextRef)ctx;

@end


@implementation VSCEnveloppeLayer

-(id) init {
    
    if ((self = [super init])) {
        _defaultDisplaySetup = VSCEnveloppeGUIConfigPtr(new VSCEnveloppeGUIConfig());
    }
    
    return self;
    
}

-(VSCEnveloppeGUIConfigPtr) getDisplaySetupForEnveloppe:(VSCEnveloppePtr)enveloppe {
    
    // Look for display setup in the map
    VSCEnveloppeGUIConfigMap::iterator setupIt = _enveloppeDisplaySetupMap.find(enveloppe);
    if (setupIt != _enveloppeDisplaySetupMap.end()) {
        return setupIt->second;
    }
    // if not in map, return the default
    return  _defaultDisplaySetup;
}
        


- (void)drawInContext:(CGContextRef)ctx {
    
    drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
    
    /*
     *  First draw non editable enveloppes
     */
    for (VSCEnveloppe::List::const_iterator it = _enveloppeList.begin(); it != _enveloppeList.end(); ++it) {
        VSCEnveloppePtr enveloppe = (*it);
        if ([self.editor enveloppeIsEditable:enveloppe] == NO) {
            VSCEnveloppeGUIConfigPtr displaySetup = [self getDisplaySetupForEnveloppe:enveloppe];
            [self drawEnveloppe:enveloppe withDisplaySetup:displaySetup inContext:ctx];
        }
    }
    
    /*
     *  Then draw editable enveloppes and selection rects
     */
    CGFloat selectionRectWidth = (CGFloat)[self.editor enveloppeEditorGUIConfig]->getSelectionRectLineWidth();
    const VSCColour selectionRectColour = [self.editor enveloppeEditorGUIConfig]->getSelectionRectColour();
    CGColorRef cgSelectionRectColour = CGColorCreateFromVSCColour(selectionRectColour);
    
    for (VSCEnveloppe::List::const_iterator it = _enveloppeList.begin(); it != _enveloppeList.end(); ++it) {
        VSCEnveloppePtr enveloppe = (*it);
        if ([self.editor enveloppeIsEditable:enveloppe] == YES) {
            VSCEnveloppeGUIConfigPtr displaySetup = [self getDisplaySetupForEnveloppe:enveloppe];
            [self drawEnveloppe:enveloppe withDisplaySetup:displaySetup inContext:ctx];
        }
        drawRectOutline(ctx, [self.editor currentSelectionRectForEnveloppe:enveloppe], selectionRectWidth, cgSelectionRectColour);
    }
    
}

-(void) drawEnveloppe:(VSCEnveloppePtr)enveloppe withDisplaySetup:(VSCEnveloppeGUIConfigPtr)displaySetup inContext:(CGContextRef)ctx {
    
    VSCEnveloppeEditorGUIConfigPtr editorConfig = [self.editor enveloppeEditorGUIConfig];
    
    CGFloat radius = (CGFloat)(displaySetup->getControlPointRadius());
    CGFloat lineWidth = (CGFloat)(displaySetup->getLineWidth());
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSCColour(displaySetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSCColour(displaySetup->getControlPointUnselectedColour());
	CGColorRef cgLineSelectedColorRef = CGColorCreateFromVSCColour(displaySetup->getLineSelectedColour());
    CGColorRef cgLineUnselectedColorRef = CGColorCreateFromVSCColour(displaySetup->getLineUnselectedColour());
    
    VSCEnveloppe::ConstPointIterator nextIt;
	VSCEnveloppe::ConstPointIterator endIt = enveloppe->getPointEndIterator();
    
    /*
     *  Draw enveloppe lines
     */
    
    BOOL isEditable = [self.editor enveloppeIsEditable:enveloppe];
    if (isEditable == NO) {
        CGContextSetStrokeColorWithColor(ctx, cgLineUnselectedColorRef);
    }
    
    CGContextSetLineWidth(ctx, lineWidth);
    
	for (VSCEnveloppe::ConstPointIterator it = enveloppe->getPointBeginIterator(); it !=endIt; ++it) {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != endIt) {
            
            VSCEnveloppePointPtr currentPoint = *it;
			VSCEnveloppePointPtr nextPoint = *nextIt;
            
            // if the enveloppe is not editable, we don't even need to check whether points are selected or not, we stay in unselected color
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
			
			//NSPoint point1 = NSMakePointFromPoint(editorConfig->pointForEnveloppeCoordinate(boost::static_pointer_cast<VSCEnveloppeCoordinate>(currentPoint)));
			NSPoint point1 = NSMakePointFromPoint(editorConfig->pointForEnveloppeCoordinate(currentPoint));
            NSPoint point2 = NSMakePointFromPoint(editorConfig->pointForEnveloppeCoordinate(nextPoint));
			
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
     *  Draw enveloppe points
     */
    
    if (isEditable == NO) {
        CGContextSetFillColorWithColor(ctx, cgUnselectedColourRef); 
    }
    
    CGContextSetLineWidth(ctx, 1);
    
    
	for (VSCEnveloppe::ConstPointIterator it = enveloppe->getPointBeginIterator(); it !=endIt; it++) {
		
		VSCEnveloppePointPtr currentPoint = *it;
		
		// draw control circle for point
		
		NSPoint p = [self pointForEnveloppePoint:currentPoint];
		
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
