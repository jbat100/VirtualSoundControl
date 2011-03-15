//
//  Enveloppe.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface Enveloppe : NSObject <NSCoding> {
	
	NSMutableArray* controlPoints;

}

@property (nonatomic, retain) NSMutableArray* controlPoints;

@end
