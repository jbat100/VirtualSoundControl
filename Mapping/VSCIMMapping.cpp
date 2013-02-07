
#include "VSCIMMapping.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;

Mapping::Mapping() : mOffset(0.0), mScaleFactor(1.0), mMappingType(MappingTypeNone)
{
    this->allowMappingType(MappingTypeConstant);
}

Float Mapping::applyOffsetAndScaleFactor(Float rawValue)
{
    return mOffset + (rawValue * mScaleFactor);
}

void Mapping::setMappingType(MappingType mappingType)
{
    switch (mappingType)
    {
        case MappingTypeConstant:
            mImplementation = Implementation::SPtr(new ImplementationConstant);
            break;
            
        case MappingTypeCollisionVelocity:
            mImplementation = Implementation::SPtr(new ImplementationCollisionVelocity);
            break;
            
        case MappingTypeCollisionDistance:
            mImplementation = Implementation::SPtr(new ImplementationCollisionDistance);
            break;
            
        default:
            BOOST_ASSERT_MSG("MappingType is not handled");
            mImplementation = Implementation::SPtr(new ImplementationConstant);
            mMappingType = MappingTypeConstant;
            return;
    }
    
    mMappingType = mappingType;

}

Float Mapping::mappedValue(Trigger trigger, TriggerPayload::SPtr payload)
{
    Float rawValue = 0.0;
    
    BOOST_ASSERT(mImplementation);
    if (mImplementation)
    {
        rawValue = mImplementation->mappedValue(trigger, payload);
    }
    
    return this->applyOffsetAndScaleFactor(rawValue);
}
