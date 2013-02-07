
#include "VSCIMEvent.h"
#include "VSCIMMapping.h"

#include <boost/assign/std/vector.hpp>
#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;

//MARK: - Mappings

bool Event::checkRequiredMappings(void)
{
    BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
    {
        BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
        {
            if (!this->getMapping(trigger, target)) return false;
        }
        return true;
    }
}

Mapping_SPtr Event::getMapping(const Trigger trigger, const Target target)
{
    TargetMappingMap& targetMappingMap = mMappingMap[trigger];
    return targetMappingMap[target];
}

void Event::clearMappings(void)
{
    mMappingMap.clear();
}

void Event::generateMappings(void)
{
    BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
    {
        TargetMappingMap& targetMappingMap = mMappingMap[trigger];
        BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
        {
            targetMappingMap[target] = Mapping::SPtr(new Mapping);
        }
    }
}


//MARK: - Mapping Targets

void Event::addRequiredMappingTarget(Target target)
{
    Targets::iterator it = std::find(mRequiredMappingTargets.begin(), mRequiredMappingTargets.end(), target);
    if (it == mRequiredMappingTargets.end()) mRequiredMappingTargets.push_back(target);
}

void Event::clearRequiredMappingTargets(void)
{
    mRequiredMappingTargets.clear();
}


