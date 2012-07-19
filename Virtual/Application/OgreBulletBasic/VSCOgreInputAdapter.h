
#ifndef _VSC_OGRE_BULLET_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_INPUT_ADAPTER_H_

#include <set>

class VSCOgreInputListener;

class VSCOgreInputAdapter
{
    
public:
    
    VSCOgreInputAdapter(void);
    
    /*
     *  Add/Remove input listeners
     */
    void addInputListener(VSCOgreInputListener* listener);
    void removeInputListener(VSCOgreInputListener* listener);
    const std::set<VSCOgreInputListener*>& getInputListeners(void) {return mInputListeners;}
    
    bool areCoordinatesNormalized(void) {return mNormalizedCoordinates;}
    void normalizeCoordinates(bool norm) {mNormalizedCoordinates = norm;}
    
private:

    std::set<VSCOgreInputListener*>     mInputListeners;
    bool                                mNormalizedCoordinates;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_ADAPTER_H_

