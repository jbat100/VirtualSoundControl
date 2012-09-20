
#ifndef _VSC_OGRE_BULLET_H_
#define _VSC_OGRE_BULLET_H_


namespace VSC {
    
    namespace OB {

        enum QueryMask
        {
            QueryMaskNone                   = 0
            QueryMaskAny					= 1<<0,
            QueryMaskRagdoll				= 1<<1,
            QueryMaskGeometry				= 1<<2,
            QueryMaskVehicle				= 1<<3,
            QueryMaskStaticGeometry         = 1<<4
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_H_

