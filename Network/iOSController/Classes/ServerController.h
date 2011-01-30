
#import <UIKit/UIKit.h>

@class AsyncSocket;
@class MTMessageBroker;

@interface ServerController : NSObject <NSNetServiceDelegate> {
	
    NSNetService *netService;
    MTMessageBroker *messageBroker;
	
	AsyncSocket *listeningSocket;
    AsyncSocket *connectionSocket;
	
}

@property (retain) AsyncSocket *listeningSocket;
@property (retain) AsyncSocket *connectionSocket;
@property (retain) MTMessageBroker *messageBroker;

+(ServerController*) instance;

-(void)startService;
-(void)stopService;

-(void) sendData:(NSData*)data;

@end
