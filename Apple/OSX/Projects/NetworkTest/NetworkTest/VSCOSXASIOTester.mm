//
//  VSCOSXASIOTester.m
//  NetworkTest
//
//  Created by Jonathan Thorpe on 9/18/13.
//  Copyright (c) 2013 Jonathan Thorpe. All rights reserved.
//

#import "VSCOSXASIOTester.h"
#import "NSString+VSCAdditions.h"

#include <boost/asio.hpp>
#include "VSCNetwork.h"
#include "VSCNetworkTCPServer.h"
#include "VSCNetworkTCPClient.h"


using namespace VSC::Network;
using namespace boost::asio;


static NSString* const VSCLocalHost = @"127.0.0.1";

@interface VSCOSXASIOTester ()
{
    TCPServer_SPtr _server;
    TCPClient_SPtr _client;
    TCPResolver_SPtr _resolver;
}

@end

@implementation VSCOSXASIOTester

-(id)init
{
    self = [super init];
    if (self)
    {
        
        
    }
    return self;
}

-(IBAction) startServer:(id)sender
{
    assert(!_server);
    if (!_server)
    {
        _server = TCPServer_SPtr(new TCPServer(sharedIOService(), self.requestedPort));
    }
}

-(IBAction) stopServer:(id)sender
{
    assert(_server);
    if (_server)
    {
        _server.reset();
    }
}

-(IBAction) sendServerMessage:(id)sender
{
    assert(_server);
    NSLog(@"%s : \"%@\"", __PRETTY_FUNCTION__, self.serverMessageString);
    _server->write([self.serverMessageString stdString]);
    NSLog(@"%s finished", __PRETTY_FUNCTION__);
}

-(IBAction) startClient:(id)sender
{
    assert(!_client);
    if (!_client)
    {
        _client = TCPClient_SPtr(new TCPClient(sharedIOService()));
    }
    
    assert(!_resolver);
    if (!_resolver)
    {
        _resolver = TCPResolver_SPtr(new TCPResolver(sharedIOService()));
    }
    
    const char* host = [VSCLocalHost cStringUsingEncoding:NSUTF8StringEncoding];
    const char* port = [[[NSNumber numberWithUnsignedInt:self.requestedPort] stringValue] cStringUsingEncoding:NSUTF8StringEncoding];
    
    NSLog(@"%s starting client... (%s, %s)", __PRETTY_FUNCTION__, host, port);
    
    _client->start(_resolver->resolve(TCPQuery(host, port)));
    
    NSLog(@"%s started client", __PRETTY_FUNCTION__);
}

-(IBAction) stopClient:(id)sender
{
    assert(_client);
    if (_client)
    {
        _client->stop();
        _client.reset();
        _resolver.reset();
    }
}

-(VSC::Network::TCPServer_SPtr) server
{
    return _server;
}

-(VSC::Network::TCPClient_SPtr) client
{
    return _client;
}

@end
