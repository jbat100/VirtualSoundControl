
#include "VSCIMMapping.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <Ogre/Ogre.h>
#include <boost/assert.hpp>



VSC::Float VSC::IM::Mapping::applyOffsetAndScaleFactor(Float rawValue)
{
    return mOffset + (rawValue * mScaleFactor);
}
