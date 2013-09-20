//
//  VSCOSXASIOTester.m
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#import "VSCOSXASIOTester.h"

#include <boost/asio.hpp>

//using boost::asio;

@implementation VSCOSXASIOTester

-(IBAction) startServer:(id)sender
{
    if (!asioTest)
    {
        asioTest = new VSC::Network::ASIOTest;
    }
}

-(IBAction) stopServer:(id)sender
{
    if (asioTest)
    {
        delete asioTest;
        asioTest = 0;
    }
}

@end
