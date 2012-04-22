//
//  VSCCollissionReceiver.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSCI_COLLISION_MAPPING_H_
#define _VSCI_COLLISION_MAPPING_H_

#include "VSCSound.h"
#include "VSCICollision.h"

#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <boost/shared_ptr.hpp>
#include <map>

/**-----------------------------------------------------------------------------------
 *  Velocity Mapping Abstract Base Class
 */

class VSCICollisionVelocityMapper;
typedef boost::shared_ptr<VSCICollisionVelocityMapper> VSCICollisionVelocityMapperPtr;

class VSCICollisionVelocityMapper {
public:
    virtual VSCSFloat noteVelocityForCollision(const VSCICollision& collision) = 0;
};

/**
 *  Velocity Mapping based on collision velocity
 */

class VSCICollisionVelocityMapperToSpeed : public VSCICollisionVelocityMapper {
public:
    VSCICollisionVelocityMapperToSpeed();
    VSCICollisionVelocityMapperToSpeed(VSCSFloat mult);
    
    VSCSFloat noteVelocityForCollision(const VSCICollision& collision);
    
    VSCSFloat getMultiplier(void) const;
    void setMultiplier(const VSCSFloat multiplier);
    
private:
    VSCSFloat _multiplier;
};


/**-----------------------------------------------------------------------------------
 *  Channel Contribution Mapping Abstract Base Class
 */

class VSCICollisionChannelContributionMapper;
typedef boost::shared_ptr<VSCICollisionChannelContributionMapper> VSCICollisionChannelContributionMapperPtr;

class VSCICollisionChannelContributionMapper {
public:
    typedef std::map<unsigned int, VSCSFloat>   ContributionMap;
    typedef std::pair<unsigned int, VSCSFloat>  ContributionPair;
    virtual const ContributionMap channelContributionForCollision(const VSCICollision& collision);
};

class VSCICollisionChannelContributionMapperToColor : public VSCICollisionChannelContributionMapper {
public:
    VSCICollisionChannelContributionMapperToColor();
    VSCICollisionChannelContributionMapperToColor(unsigned int rChannel, unsigned int gChannel, unsigned int bChannel);
    const ContributionMap channelContributionForCollision(const VSCICollision& collision);
    void setRChannel(const unsigned int channel);
    unsigned int getRChannel(void) const;
    void setGChannel(const unsigned int channel);
    unsigned int getGChannel(void) const;
    void setBChannel(const unsigned int channel);
    unsigned int getBChannel(void) const;
private:
    unsigned int _rChannel;
    unsigned int _gChannel;
    unsigned int _bChannel;
};

#endif
