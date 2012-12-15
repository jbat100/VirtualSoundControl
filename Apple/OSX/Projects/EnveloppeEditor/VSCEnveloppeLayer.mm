//
//  VSC::EnveloppeLayer.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCEnveloppeLayer.h"

#import "CGColor+VSCAdditions.h"
#import "NS+VSCGeomOperations.h"
#import <boost/shared_ptr.hpp>

@interface VSC::EnveloppeLayer ()
{
    @private
    VSC::Enveloppe::List _enveloppeList;
}

-(void) drawEnveloppe:(VSC::Enveloppe::SPtr)enveloppe withDisplaySetup:(VSC::EnveloppeGUIConfig::SPtr)displaySetup inContext:(CGContextRef)ctx;

@end


@implementation VSCEnveloppeLayer

@synthesize editor = _editor;

-(id) init {
    
    if ((self = [super init])) {
        _defaultDisplaySetup = VSC::EnveloppeGUIConfig::SPtr(new VSC::EnveloppeGUIConfig());
    }
    
    return self;
    
}

#pragma mark - C++ Setters/Getters

-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
    
    VSC::Enveloppe::List::iterator it = std::find(_enveloppeList.begin(), _enveloppeList.end(), enveloppe);
    
    if (it == _enveloppeList.end()) {
        _enveloppeList.push_back(enveloppe);
    }
    
}

-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe atIndex:(NSUInteger)index {
    NSAssert(0, @"Not Implemented");
}

-(void) removeEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
    _enveloppeList.remove(enveloppe);
}

-(VSC::EnveloppeGUIConfig::SPtr) getDefaultDisplaySetup {
    return _defaultDisplaySetup;
}

-(void) setDefaultDisplaySetup:(VSC::EnveloppeGUIConfig::SPtr)setup {
    _defaultDisplaySetup = setup;
}


-(void) setDisplaySetup:(VSC::EnveloppeGUIConfig::SPtr)setup forEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
    _enveloppeDisplaySetupMap[enveloppe] = setup;
}

-(VSC::EnveloppeGUIConfig::SPtr) getDisplaySetupForEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
    
    // Look for display setup in the map
    VSC::EnveloppeGUIConfigMap::iterator setupIt = _enveloppeDisplaySetupMap.find(enveloppe);
    if (setupIt != _enveloppeDisplaySetupMap.end()) {
        return setupIt->second;
    }
    // if not in map, return the default
    return  _defaultDisplaySetup;
}
        
#pragma mark - Drawing

- (void)drawInContext:(CGContextRef)ctx {
    
    drawRectFill(ctx, self.bounds, CGColorCreateFromRGBNSColor([NSColor lightGrayColor]));
    
    /*
     *  First draw non editable enveloppes
     */
    for (VSC::Enveloppe::List::const_iterator it = _enveloppeList.begin(); it != _enveloppeList.end(); ++it) {
        VSC::Enveloppe::SPtr enveloppe = (*it);
        if ([self.editor enveloppeIsEditable:enveloppe] == NO) {
            VSC::EnveloppeGUIConfig::SPtr displaySetup = [self getDisplaySetupForEnveloppe:enveloppe];
            [self drawEnveloppe:enveloppe withDisplaySetup:displaySetup inContext:ctx];
        }
    }
    
    /*
     *  Then draw editable enveloppes and selection rects
     */
    CGFloat selectionRectWidth = (CGFloat)[self.editor enveloppeEditorGUIConfig]->getSelectionRectLineWidth();
    const VSC::Color selectionRectColour = [self.editor enveloppeEditorGUIConfig]->getSelectionRectColour();
    CGColorRef cgSelectionRectColour = CGColorCreateFromVSC::Color(selectionRectColour);
    
    for (VSC::Enveloppe::List::const_iterator it = _enveloppeList.begin(); it != _enveloppeList.end(); ++it) {
        VSC::Enveloppe::SPtr enveloppe = (*it);
        if ([self.editor enveloppeIsEditable:enveloppe] == YES) {
            VSC::EnveloppeGUIConfig::SPtr displaySetup = [self getDisplaySetupForEnveloppe:enveloppe];
            [self drawEnveloppe:enveloppe withDisplaySetup:displaySetup inContext:ctx];
        }
        drawRectOutline(ctx, [self.editor currentSelectionRectForEnveloppe:enveloppe], selectionRectWidth, cgSelectionRectColour);
    }
    
}

-(void) drawEnveloppe:(VSC::Enveloppe::SPtr)enveloppe withDisplaySetup:(VSC::EnveloppeGUIConfig::SPtr)displaySetup inContext:(CGContextRef)ctx {
    
    VSC::EnveloppeEditorGUIConfig::SPtr editorConfig = [self.editor enveloppeEditorGUIConfig];
    
    CGFloat radius = (CGFloat)(displaySetup->getControlPointRadius());
    CGFloat lineWidth = (CGFloat)(displaySetup->getLineWidth());
	CGColorRef cgSelectedColourRef = CGColorCreateFromVSC::Color(displaySetup->getControlPointSelectedColour());
	CGColorRef cgUnselectedColourRef = CGColorCreateFromVSC::Color(displaySetup->getControlPointUnselectedColour());
	CGColorRef cgLineSelectedColorRef = CGColorCreateFromVSC::Color(displaySetup->getLineSelectedColour());
    CGColorRef cgLineUnselectedColorRef = CGColorCreateFromVSC::Color(displaySetup->getLineUnselectedColour());
    
    VSC::Enveloppe::ConstPointIterator nextIt;
	VSC::Enveloppe::ConstPointIterator endIt = enveloppe->getPointEndIterator();
    
    /*
     *  Draw enveloppe lines
     */
    
    BOOL isEditable = [self.editor enveloppeIsEditable:enveloppe];
    if (isEditable == NO) {
        CGContextSetStrokeColorWithColor(ctx, cgLineUnselectedColorRef);
    }
    
    CGContextSetLineWidth(ctx, lineWidth);
    
	for (VSC::Enveloppe::ConstPointIterator it = enveloppe->getPointBeginIterator(); it !=endIt; ++it) {
		
		nextIt = it;
		nextIt++;
		
		if (nextIt != endIt) {
            
            VSC::EnveloppePoint::SPtr currentPoint = *it;
			VSC::EnveloppePoint::SPtr nextPoint = *nextIt;
            
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
			
			//NSPoint point1 = NSMakePointFromPoint(editorConfig->pointForEnveloppeCoordinate(boost::static_pointer_cast<VSC::EnveloppeCoordinate>(currentPoint)));
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
    
    
	for (VSC::Enveloppe::ConstPointIterator it = enveloppe->getPointBeginIterator(); it !=endIt; it++) {
		
		VSC::EnveloppePoint::SPtr currentPoint = *it;
		
		// draw control circle for point
		
        VSC::Point vscp = [self.editor enveloppeEditorGUIConfig]->pointForEnveloppeCoordinate(boost::dynamic_pointer_cast<VSC::EnveloppeCoordinate>(currentPoint));
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
