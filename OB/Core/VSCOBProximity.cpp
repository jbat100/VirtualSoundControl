
#include "VSCOBProximity.h"

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBElement.h"
#include "VSCOBScene.h"


std::ostream& VSC::OB::operator << (std::ostream& stream, const Proximity& collision)
{
    stream << "Proximity between element: " << *collision.getFirstElement() << " and element: " << *collision.getSecondElement();
    stream << " state: " << (collision.mState == Proximity::StateActive ? "active" : "not active") << std::endl;
    
    return stream;
}
