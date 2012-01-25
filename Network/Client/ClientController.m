
#import "ClientController.h"
#import "MTMessageBroker.h"
#import "MTMessage.h"
#import "AsyncSocket.h"


@interface ClientController ()

@property (readwrite, retain) NSNetServiceBrowser *browser;
@property (readwrite, retain) NSMutableArray *services;
@property (readwrite, assign) BOOL isConnected;
@property (readwrite, retain) NSNetService *connectedService;
@property (readwrite, retain) MTMessageBroker *messageBroker;

@end


@implementation ClientController

@synthesize browser;
@synthesize services;
@synthesize isConnected;
@synthesize connectedService;
@synthesize socket;
@synthesize messageBroker;

-(void)awakeFromNib {
    services = [NSMutableArray new];
    self.browser = [[NSNetServiceBrowser new] autorelease];
    self.browser.delegate = self;
    self.isConnected = NO;
}

-(void)dealloc {
    self.connectedService = nil;
    self.browser = nil;
    self.socket = nil;
    self.messageBroker.delegate = nil;
    self.messageBroker = nil;
    [services release];
    [super dealloc];
}

-(IBAction)search:(id)sender {
    [self.browser searchForServicesOfType:@"_cocoaforscientists._tcp." inDomain:@""];
}

-(IBAction)connect:(id)sender {
    NSNetService *remoteService = servicesController.selectedObjects.lastObject;
    remoteService.delegate = self;
    [remoteService resolveWithTimeout:30];
}

-(IBAction)send:(id)sender {
    NSData *data = [textView.string dataUsingEncoding:NSUTF8StringEncoding];
    MTMessage *newMessage = [[[MTMessage alloc] init] autorelease];
    newMessage.tag = 100;
    newMessage.dataContent = data;
    [self.messageBroker sendMessage:newMessage];
}

#pragma mark MTMessageBroker Delegate Methods
-(void)messageBroker:(MTMessageBroker *)server didReceiveMessage:(MTMessage *)message {
    if ( message.tag == 100 ) {
        NSString* receivedString = [[[NSString alloc] initWithData:message.dataContent encoding:NSUTF8StringEncoding] autorelease];
		NSLog(@"Got string: \"%@\"", receivedString);
    }
}

#pragma mark AsyncSocket Delegate Methods
-(void)onSocketDidDisconnect:(AsyncSocket *)sock {
    NSLog(@"Socket disconnected");
}

-(BOOL)onSocketWillConnect:(AsyncSocket *)sock {
    if ( messageBroker == nil ) {
        [sock retain];
        return YES;
    }
    return NO;
}

-(void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port {      
    MTMessageBroker *newBroker = [[[MTMessageBroker alloc] initWithAsyncSocket:socket] autorelease];
    [sock release];
    newBroker.delegate = self;
    self.messageBroker = newBroker;
    self.isConnected = YES;
}

#pragma mark Net Service Browser Delegate Methods
-(void)netServiceBrowser:(NSNetServiceBrowser *)aBrowser didFindService:(NSNetService *)aService moreComing:(BOOL)more {
    [servicesController addObject:aService];
}

-(void)netServiceBrowser:(NSNetServiceBrowser *)aBrowser didRemoveService:(NSNetService *)aService moreComing:(BOOL)more {
    [servicesController removeObject:aService];
    if ( aService == self.connectedService ) self.isConnected = NO;
}

-(void)netServiceDidResolveAddress:(NSNetService *)service {
    NSError *error;
    self.connectedService = service;
    self.socket = [[[AsyncSocket alloc] initWithDelegate:self] autorelease];
    [self.socket connectToAddress:service.addresses.lastObject error:&error];
}

-(void)netService:(NSNetService *)service didNotResolve:(NSDictionary *)errorDict {
    NSLog(@"Could not resolve: %@", errorDict);
}

@end
