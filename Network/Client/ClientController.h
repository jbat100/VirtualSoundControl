
#import <Cocoa/Cocoa.h>

@class AsyncSocket;
@class MTMessageBroker;

@interface ClientController : NSObject <NSNetServiceDelegate, NSNetServiceBrowserDelegate> {
    BOOL isConnected;
    NSNetServiceBrowser *browser;
    NSNetService *connectedService;
    NSMutableArray *services;
    AsyncSocket *socket;
    MTMessageBroker *messageBroker;
    IBOutlet NSArrayController *servicesController;
    IBOutlet NSTextView *textView;
}

@property (readonly, retain) NSMutableArray *services;
@property (readonly, assign) BOOL isConnected;
@property (readwrite, retain) AsyncSocket *socket;

-(IBAction)search:(id)sender;
-(IBAction)connect:(id)sender;
-(IBAction)send:(id)sender;

@end
