//
//  VSCOSXASIOTester.h
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCNetwork.h"

@interface VSCOSXASIOTester : NSObject

@property (nonatomic, assign) unsigned int requestedPort;

@property (nonatomic, strong) IBOutlet NSTextField* portTextField;

/**
 Server stuff
 */

@property (nonatomic, strong) NSString* serverMessageString;

@property (nonatomic, strong) IBOutlet NSTextField* serverStatusTextField;
@property (nonatomic, strong) IBOutlet NSTextField* serverMessageTextField;
@property (nonatomic, strong) IBOutlet NSButton* serverActionButton;
@property (nonatomic, strong) IBOutlet NSButton* serverSendButton;

-(IBAction) startServer:(id)sender;
-(IBAction) stopServer:(id)sender;
-(IBAction) sendServerMessage:(id)sender;

-(VSC::Network::TCPServer_SPtr) server;

/**
 Client stuff
 */

@property (nonatomic, strong) NSString* clientMessageString;

@property (nonatomic, strong) IBOutlet NSTextField* clientStatusTextField;
@property (nonatomic, strong) IBOutlet NSTextField* clientMessageTextField;
@property (nonatomic, strong) IBOutlet NSButton* clientActionButton;
@property (nonatomic, strong) IBOutlet NSButton* clientSendButton;

-(IBAction) startClient:(id)sender;
-(IBAction) stopClient:(id)sender;
-(IBAction) sendClientMessage:(id)sender;

-(VSC::Network::TCPClient_SPtr) client;

@end
