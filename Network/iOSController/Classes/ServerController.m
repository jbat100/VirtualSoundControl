
#import "ServerController.h"
#import "AsyncSocket.h"
#import "MTMessageBroker.h"
#import "MTMessage.h"



@implementation ServerController

@synthesize listeningSocket, connectionSocket, messageBroker;

static ServerController* sInstance = nil;

+(ServerController*) instance {
	
	@synchronized (self) {
		
		if (!sInstance) 
			sInstance = [[ServerController alloc] init];
		
	}
	
	return sInstance;
	
}

-(void)dealloc {
    [self stopService];
    [super dealloc];
}

#pragma mark -
#pragma mark Network Service Methods

-(void) startService {
    // Start listening socket
    NSError *error;
    self.listeningSocket = [[[AsyncSocket alloc]initWithDelegate:self] autorelease];
	// Accept on port 0 gives back available port
    if ( ![self.listeningSocket acceptOnPort:0 error:&error] ) {
        NSLog(@"Failed to create listening socket");
        return;
    }
    
    // Advertise service with bonjour
    NSString *serviceName = [NSString stringWithFormat:@"iOSController %@", [[NSProcessInfo processInfo] hostName]];
	// empty domain means local
    netService = [[NSNetService alloc] initWithDomain:@"" type:@"_ioscontroller._tcp." name:serviceName port:self.listeningSocket.localPort];
    netService.delegate = self;
    [netService publish];
}

-(void) stopService {
	
    self.listeningSocket = nil;
    self.connectionSocket = nil;
	
    self.messageBroker.delegate = nil;
    self.messageBroker = nil;
    
	[netService stop]; 
    [netService release];  
	netService = nil;

}

-(BOOL) isActive {
	return netService ? YES : NO;
}


#pragma mark -
#pragma mark Messaging Methods

-(void) sendData:(NSData*)data {
	
	if (messageBroker) {
		MTMessage *newMessage = [[[MTMessage alloc] init] autorelease];
		newMessage.tag = 100;
		newMessage.dataContent = data;
		[messageBroker sendMessage:newMessage];
	}
	else {
		NSLog(@"Cannot send data, network service is not active");
	}

}

-(void)messageBroker:(MTMessageBroker *)server didReceiveMessage:(MTMessage *)message {
    if ( message.tag == 100 ) {
        NSString* receivedMessage = [[[NSString alloc] initWithData:message.dataContent encoding:NSUTF8StringEncoding] autorelease];
		NSLog(@"Received message: %@", receivedMessage);
    }
}

#pragma mark -
#pragma mark Socket Callbacks

-(BOOL)onSocketWillConnect:(AsyncSocket *)sock {
    if ( self.connectionSocket == nil ) {
        self.connectionSocket = sock;
        return YES;
    }
    return NO;
}

-(void)onSocketDidDisconnect:(AsyncSocket *)sock {
    if ( sock == self.connectionSocket ) {
        self.connectionSocket = nil;
        self.messageBroker = nil;
    }
}

-(void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port {
    MTMessageBroker *newBroker = [[[MTMessageBroker alloc] initWithAsyncSocket:sock] autorelease];
    newBroker.delegate = self;
    self.messageBroker = newBroker;
}

#pragma mark -
#pragma mark Net Service Delegate Methods

-(void)netService:(NSNetService *)aNetService didNotPublish:(NSDictionary *)dict {
    NSLog(@"Failed to publish: %@", dict);
}

@end
