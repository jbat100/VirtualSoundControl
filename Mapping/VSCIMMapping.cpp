
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

bool Mapping::setMappingType(MappingType mappingType)
{
    bool allowed = this->checkMappingType(mappingType);
    BOOST_ASSERT(allowed);
    if (allowed)
    {
        mMappingType = mappingType;
        return true;
    }
    return false;
}

bool Mapping::checkMappingType(const MappingType mappingType)
{
    return mAllowedMappingTypeSet.find(mappingType) != mAllowedMappingTypeSet.end();
}

void Mapping::allowMappingType(const MappingType mappingType)
{
    mAllowedMappingTypeSet.insert(mappingType);
}

void Mapping::setSetupForSetupType(Setup::SPtr setup, SetupType setupType)
{
    
}
