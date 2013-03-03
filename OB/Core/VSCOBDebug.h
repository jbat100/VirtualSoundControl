
#ifndef _VSC_OB_DEBUG_H_
#define _VSC_OB_DEBUG_H_

#include <Ogre/Ogre.h>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        std::string OgreEntityDescription(Ogre::Entity* entity);
        std::string OgreEntityVertexDeclarationDescription(Ogre::Entity* entity);
        
        /*
         *  Ogre Mesh Tools 
         */
        
        size_t OgreMeshVertexCount(const Ogre::Mesh* const mesh);
        size_t OgreMeshIndexCount(const Ogre::Mesh* const mesh);
        
        bool OgreMeshVertices(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vertices);
        bool OgreMeshNormals(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& normals);
        bool OgreMeshIndeces(const Ogre::Mesh* const mesh, std::vector<unsigned long>& indices);
        bool OgreMeshDiffuseColors(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors);
        bool OgreMeshSpecularColors(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors);
        bool OgreMeshTextureCoordinates(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector2>& coordinates);
        
    }
}

#endif //_VSC_OB_DEBUG_H_

