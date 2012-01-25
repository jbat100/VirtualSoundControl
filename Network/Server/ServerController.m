
#import "ServerController.h"
#import "AsyncSocket.h"
#import "MTMessageBroker.h"
#import "MTMessage.h"


@interface ServerController ()

@property (readwrite, retain) MTMessageBroker *messageBroker;

-(void)startService;
-(void)stopService;

@end


@implementation ServerController

@synthesize listeningSocket;
@synthesize connectionSocket;
@synthesize messageBroker;

-(void)awakeFromNib {    
    [self startService];
}

-(void)startService {
    // Start listening socket
    NSError *error;
    self.listeningSocket = [[[AsyncSocket alloc]initWithDelegate:self] autorelease];
    if ( ![self.listeningSocket acceptOnPort:0 error:&error] ) {
        NSLog(@"Failed to create listening socket");
        return;
    }
    
    // Advertise service with bonjour
    NSString *serviceName = [NSString stringWithFormat:@"Cocoa for Scientists on %@", [[NSProcessInfo processInfo] hostName]];
    netService = [[NSNetService alloc] initWithDomain:@"" type:@"_cocoaforscientists._tcp." name:serviceName port:self.listeningSocket.localPort];
    netService.delegate = self;
    [netService publish];
}

-(void)stopService {
    self.listeningSocket = nil;
    self.connectionSocket = nil;
    self.messageBroker.delegate = nil;
    self.messageBroker = nil;
    [netService stop]; 
    [netService release];    
    [super dealloc];
}

-(void)dealloc {
    [self stopService];
    [super dealloc];
}

#pragma mark Button Callbacks

-(IBAction) send:(id)sender {
	NSData *data = [sendingTextView.string dataUsingEncoding:NSUTF8StringEncoding];
    MTMessage *newMessage = [[[MTMessage alloc] init] autorelease];
    newMessage.tag = 100;
    newMessage.dataContent = data;
    [self.messageBroker sendMessage:newMessage];
}

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

#pragma mark MTMessageBroker Delegate Methods
-(void)messageBroker:(MTMessageBroker *)server didReceiveMessage:(MTMessage *)message {
    if ( message.tag == 100 ) {
        receivingTextView.string = [[[NSString alloc] initWithData:message.dataContent encoding:NSUTF8StringEncoding] autorelease];
    }
}

#pragma mark Net Service Delegate Methods
-(void)netService:(NSNetService *)aNetService didNotPublish:(NSDictionary *)dict {
    NSLog(@"Failed to publish: %@", dict);
}

@end
