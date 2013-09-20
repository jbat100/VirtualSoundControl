//
//  VSCOSXASIOTester.h
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCNetworkASIOTest.h"

@interface VSCOSXASIOTester : NSObject
{
    VSC::Network::ASIOTest* asioTest;
}

-(IBAction) startServer:(id)sender;

-(IBAction) stopServer:(id)sender;

@end
