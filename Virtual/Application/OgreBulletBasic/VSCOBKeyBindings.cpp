
#include "VSCOBKeyBindings.h"

#include <boost/pointer_cast.hpp>

void VSC::OB::KeyBound::setOgreKeyBindings(VSC::OB::KeyBindings::SPtr keyBindings)
{
    BaseBindings::SPtr baseKeyBinding = boost::dynamic_pointer_cast<BaseBindings>(keyBindings);
    this->setBindings(baseKeyBinding);
}

VSC::OB::KeyBindings::SPtr VSC::OB::KeyBound::getOgreKeyBindings(void)
{
    BaseBindings::SPtr baseKeyBinding = this->getBindings();
    return boost::dynamic_pointer_cast<VSC::OB::KeyBindings>(baseKeyBinding);
}



