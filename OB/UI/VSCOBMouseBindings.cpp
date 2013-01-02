
#include "VSCOBMouseBindings.h"

#include <boost/pointer_cast.hpp>

void VSC::OB::KeyBound::setOBMouseBindings(VSC::OB::MouseBindings::SPtr bindings)
{
    BaseBindings::SPtr baseMouseBinding = boost::dynamic_pointer_cast<BaseBindings>(bindings);
    this->setBindings(baseMouseBinding);
}

VSC::OB::MouseBindings::SPtr VSC::OB::MouseBound::getOBMouseBindings(void)
{
    BaseBindings::SPtr baseMouseBinding = this->getBindings();
    return boost::dynamic_pointer_cast<VSC::OB::MouseBindings>(baseMouseBinding);
}



