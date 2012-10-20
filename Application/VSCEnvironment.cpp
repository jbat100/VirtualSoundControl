
#include "VSCGlobalApplication.h"
#include "VSCEnvironment.h"

VSC::Environment::Environment(GlobalApplication::SPtr application)
{
    mGlobalApplication = GlobalApplication::WPtr(application);
}


