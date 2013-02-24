
#include "VSCOBProximityDetector.h"

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBProximity.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>


bool VSC::OB::ProximityDetector::checkListener(VSC::Listener::SPtr listener)
{
    ProximityDetector::Listener::SPtr l = boost::dynamic_pointer_cast<ProximityDetector::Listener>(listener);
    if (l)
    {
        return true;
    }
    return false;
}

void VSC::OB::ProximityDetector::startMonitoringProximity(Proximity::SPtr proximity)
{
    BOOST_ASSERT(proximity);
    if (proximity)
    {
        Proximities::iterator it = std::find(mProximities.begin(), mProximities.end(), proximity);
        BOOST_ASSERT(it == mProximities.end());
        if (it == mProximities.end())
        {
            mProximities.push_back(proximity);
        }
    }
}

void VSC::OB::ProximityDetector::stopMonitoringProximity(Proximity::SPtr proximity)
{
    BOOST_ASSERT(proximity);
    if (proximity)
    {
        Proximities::iterator it = std::find(mProximities.begin(), mProximities.end(), proximity);
        BOOST_ASSERT(it != mProximities.end());
        if (it != mProximities.end())
        {
            mProximities.erase(it);
        }
    }
}

const VSC::OB::Proximities VSC::OB::ProximityDetector::getProximitiesForElementPair(Element::SPtr first, Element::SPtr second)
{
    Proximities proximities;
    
    BOOST_FOREACH(Proximity::SPtr proximity, mProximities)
    {
        /*
         *  Test both permutations
         */
        
        if (proximity->getFirstElement() == first && proximity->getSecondElement() == second)
        {
            proximities.push_back(proximity);
        }
        else if (proximity->getFirstElement() == second && proximity->getSecondElement() == first)
        {
            proximities.push_back(proximity);
        }
    }
    
    return proximities;
}

void VSC::OB::ProximityDetector::monitorProximities(void)
{
    BOOST_FOREACH(Proximity::SPtr proximity, mProximities)
    {
        /*
         *  Get positions and compute distance, update listeners...
         */
    }
}






