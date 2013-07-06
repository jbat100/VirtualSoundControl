
#ifndef _VSC_OB_DEBUG_H_
#define _VSC_OB_DEBUG_H_

#include <Ogre/Ogre.h>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        /*
         *  Ogre Entity Debug
         */
        
        std::string OgreEntityDescription(Ogre::Entity* entity);
        std::string OgreEntityVertexDeclarationDescription(Ogre::Entity* entity);
        
        /*
         *  Ogre Mesh Debug 
         */
        
        bool OgreMeshVertices(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vertices);
        bool OgreMeshNormals(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& normals);
        bool OgreMeshIndeces(const Ogre::Mesh* const mesh, std::vector<unsigned long>& indices);
        bool OgreMeshDiffuseColors(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors);
        bool OgreMeshSpecularColors(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors);
        bool OgreMeshTextureCoordinates(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector2>& coordinates);
        
        /*
         *  Streams
         */
        
        std::ostream& operator<< (std::ostream &out, Ogre::Pass &cPoint);
        std::ostream& operator<< (std::ostream &out, Ogre::TextureUnitState &state);
        
    }
}

#endif //_VSC_OB_DEBUG_H_

