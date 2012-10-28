

#include "VSCIMCollisionMapped.h"

VSC::IM::CollisionMapping::SPtr VSC::IM::CollisionMapped::getMappingForTarget(Target target)
{
    return mCollisionMappingMap[target];
}

void VSC::IM::CollisionMapped::setMappingForTarget(Target target, CollisionMapping::SPtr mapping)
{
    mCollisionMappingMap[target] = mapping;
}

