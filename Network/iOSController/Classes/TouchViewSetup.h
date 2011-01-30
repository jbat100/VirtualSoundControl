//
//  TouchViewSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 30/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface TouchViewSetup : NSObject {
	
	// A name for the setup (preset or user)
	NSString* name;
	
	// A desription of each touch view (views should be created and added to a superview in sequence)
	NSMutableArray* touchViewDescriptions;
	
}

@property (nonatomic, retain) NSString* name;
@property (nonatomic, retain) NSMutableArray* touchViewDescriptions;

@end
