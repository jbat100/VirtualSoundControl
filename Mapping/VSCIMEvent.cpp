
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
    }
    return true;
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
    Triggers triggers = AllowedTriggers();
    std::cout << "There are " << triggers.size() << " triggers " << std::endl;
    
    BOOST_FOREACH(const Trigger& trigger, AllowedTriggers())
    {
        std::cout << "Generating mappings for trigger " << StringForTrigger(trigger) << std::endl;
        TargetMappingMap& targetMappingMap = mMappingMap[trigger];
        BOOST_FOREACH(const Target& target, mRequiredMappingTargets)
        {
            std::cout << "Generating mappings for target " << StringForTarget(target) << std::endl;
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


