//
//  VSCNSLog.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

/*
 *  VSCLOGGER and variants use NSLogger
 */

#ifdef DEBUG
#define VSCLOGGER(level, ...)              LogMessageF(__FILE__,__LINE__,__FUNCTION__,@"general",level,__VA_ARGS__)
#define VSCLOGGER_UI(level, ...)           LogMessageF(__FILE__,__LINE__,__FUNCTION__,@"ui",level,__VA_ARGS__)
#define VSCLOGGER_NETWORK(level, ...)      LogMessageF(__FILE__,__LINE__,__FUNCTION__,@"graphics",level,__VA_ARGS__)   
#else
#define VSCLOGGER(...)                     do{}while(0)
#define VSCLOGGER_UI(...)                  do{}while(0)
#define VSCLOGGER_NETWORK(...)             do{}while(0)
#endif
