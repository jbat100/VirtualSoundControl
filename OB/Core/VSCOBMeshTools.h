
#ifndef _VSC_OB_MESH_TOOLS_H_
#define _VSC_OB_MESH_TOOLS_H_

#include <Ogre/Ogre.h>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        /*
         *  Vertex and Index count 
         */
        
        size_t OgreMeshVertexCount(const Ogre::Mesh* const mesh);
        size_t OgreMeshIndexCount(const Ogre::Mesh* const mesh);
        
        /*
         *  Color Tools
         */
        
        // semantic can be Ogre::VES_DIFFUSE or Ogre::VES_SPECULAR
        
        void OgreMeshRandomizeVertexColors(Ogre::Mesh* mesh, Ogre::VertexElementSemantic semantic = Ogre::VES_DIFFUSE);
        void OgreMeshConstantVertexColors(Ogre::Mesh* mesh, Ogre::RGBA color, Ogre::VertexElementSemantic semantic = Ogre::VES_DIFFUSE);
    
    }
}

#endif //_VSC_OB_MESH_TOOLS_H_

