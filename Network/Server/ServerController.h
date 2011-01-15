
#import <Cocoa/Cocoa.h>

@class AsyncSocket;
@class MTMessageBroker;

@interface ServerController : NSObject <NSNetServiceDelegate> {
    IBOutlet NSTextView *receivingTextView;
	IBOutlet NSTextView *sendingTextView;
    NSNetService *netService;
    AsyncSocket *listeningSocket;
    AsyncSocket *connectionSocket;
    MTMessageBroker *messageBroker;
}

@property (readwrite, retain) AsyncSocket *listeningSocket;
@property (readwrite, retain) AsyncSocket *connectionSocket;

-(IBAction) send:(id)sender;

@end
