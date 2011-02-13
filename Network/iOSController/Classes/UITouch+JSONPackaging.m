//
//  UITouch+JSONPackaging.m
//  iOSController
//
//  Created by Jonathan Thorpe on 27/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "UITouch+JSONPackaging.h"
#import "NetworkDefinitions.h"

@implementation UITouch (JSONPackaging)

-(void) fillJSONDictionary:(NSMutableDictionary*)dictionary {
	
	[self fillJSONDictionary:dictionary centered:YES normalised:YES inverted:YES];
	
}

-(void) fillJSONDictionary:(NSMutableDictionary*)dictionary 
				  centered:(BOOL)centered 
				normalised:(BOOL)normalised 
				  inverted:(BOOL)inverted {
	
	CGPoint l = [self locationInView:[self view]];
	CGPoint pl = [self previousLocationInView:[self view]];
	
	if (centered) {
		CGFloat widthOffset = [self view].frame.size.width / 2.0;
		l.x -= widthOffset;
		pl.x -= widthOffset;
		CGFloat heightOffset = [self view].frame.size.height / 2.0;
		l.y -= heightOffset;
		pl.y -= heightOffset;
	}
	
	if (normalised) {
		l.x /= [self view].frame.size.width;
		pl.x /= [self view].frame.size.width;
		l.y /= [self view].frame.size.height;
		pl.y /= [self view].frame.size.height;
	}
	
	if (inverted) {
		l.y *= -1.0;
		pl.y *= -1.0;
	}
	
	else {
		l.x *= -1.0;
		pl.x *= -1.0;
	}
	
	[dictionary setObject:[NSString stringWithFormat:@"%x", self] forKey:
	 [NSString stringWithCString:JSON_TOUCH_ID_KEY encoding:NSUTF8StringEncoding]];
	
	[dictionary setObject:[NSString stringWithFormat:@"%.4f", l.x] forKey:
	 [NSString stringWithCString:JSON_TOUCH_X_KEY encoding:NSUTF8StringEncoding]];
	[dictionary setObject:[NSString stringWithFormat:@"%.4f", l.y] forKey:
	 [NSString stringWithCString:JSON_TOUCH_Y_KEY encoding:NSUTF8StringEncoding]];
	
	[dictionary setObject:[NSString stringWithFormat:@"%.4f", l.x - pl.x] forKey:
	 [NSString stringWithCString:JSON_TOUCH_DX_KEY encoding:NSUTF8StringEncoding]];
	[dictionary setObject:[NSString stringWithFormat:@"%.4f", l.y - pl.y] forKey:
	 [NSString stringWithCString:JSON_TOUCH_DY_KEY encoding:NSUTF8StringEncoding]];
	
}


@end
