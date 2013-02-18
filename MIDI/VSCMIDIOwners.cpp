
#include "VSCMIDIOwners.h"

#include "VSCMIDIOutputManager.h"

using namespace VSC;

MIDI::OutputOwner::OutputOwner()
{
    mMIDIOutput = MIDI::Output::SPtr();
    
    MIDI::OutputManager::SPtr manager = MIDI::OutputManager::singletonManager();
    BOOST_ASSERT(manager);
    if (!manager) return;
    
    MIDI::Output::SPtr output = manager->getFirstOpenedOutput();
    BOOST_ASSERT(output);
    
    mMIDIOutput = output;
}