
#ifndef _VSC_OB_DEBUG_H_
#define _VSC_OB_DEBUG_H_

#include <Ogre/Ogre.h>

namespace VSC {
    
    namespace OB {
        
        std::string OgreEntityDescription(Ogre::Entity* entity);
        std::string OgreEntityVertexDeclarationDescription(Ogre::Entity* entity);
        
        /*
         *  Ogre Mesh Tools 
         */
        
        size_t OgreMeshVertexCount(const Ogre::Mesh* const mesh);
        size_t OgreMeshIndexCount(const Ogre::Mesh* const mesh);
        
        // vertices and indeces will be array newed (it your responsibility to call delete[] on both of them)
        void OgreMeshVertices(const Ogre::Mesh* const mesh, Ogre::Vector3* &vertices);
        void OgreMeshNormals(const Ogre::Mesh* const mesh, Ogre::Vector3* &normals);
        void OgreMeshIndeces(const Ogre::Mesh* const mesh, unsigned long* &indices);
        void OgreMeshDiffuseColors(const Ogre::Mesh* const mesh, Ogre::RGBA* &colors);
        void OgreMeshSpecularColors(const Ogre::Mesh* const mesh, Ogre::RGBA* &colors);
        
    }
}

#endif //_VSC_OB_DEBUG_H_

