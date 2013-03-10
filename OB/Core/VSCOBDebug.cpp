
#include "VSCOBDebug.h"
#include "VSCOBMeshTools.h"

#include "VSCException.h"

#include <Ogre/Ogre.h>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/set.hpp>
#include <boost/foreach.hpp>

using namespace boost::assign;

namespace VSC {
    
    namespace OB {
        
        bool OgreMeshVertexBiInfo(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector2>& vectors, Ogre::VertexElementSemantic semantic);
        bool OgreMeshVertexTriInfo(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vectors, Ogre::VertexElementSemantic semantic);

        
        bool OgreMeshVertexColorInfo(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors, Ogre::VertexElementSemantic semantic);
        
        // old...
        void OgreMeshInformation(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vertices, std::vector<unsigned long>& indices);
        
        std::string OgreVertexElementSemanticToString(Ogre::VertexElementSemantic semantic);
        
    }
}

static const bool traceMesh = true;

//MARK: - String Conversions

std::string VSC::OB::OgreVertexElementSemanticToString(Ogre::VertexElementSemantic semantic)
{
    switch (semantic) {
            
        case Ogre::VES_POSITION:
            return "Position";
            
        case Ogre::VES_NORMAL:
            return "Normal";
            
        case Ogre::VES_DIFFUSE:
            return "Diffuse";
            
        case Ogre::VES_SPECULAR:
            return "Specular";
            
        case Ogre::VES_TEXTURE_COORDINATES:
            return "Texture coordinates";
            
        default:
            break;
    }
    
    return "Unknown Semantic";
}

//MARK: - Description

std::string VSC::OB::OgreEntityDescription(Ogre::Entity* entity)
{
    std::stringstream s;
    
    Ogre::MeshPtr meshPtr = entity->getMesh();
    Ogre::Mesh* mesh = meshPtr.get();
    
    size_t vertex_count = OgreMeshVertexCount(mesh);
    size_t index_count = OgreMeshIndexCount(mesh);
    
    std::vector<Ogre::Vector3> vertices;
    std::vector<unsigned long> indices;
    
    OgreMeshVertices(mesh, vertices);
    OgreMeshIndeces(mesh, indices);
    
    s << "Ogre::Entity : " << "\n";
    s << "---------------------------------------- " << vertex_count << " vertices ------------------------------- \n";
    for (int i = 0; i < vertex_count; i++)
    {
        s << vertices[i] << "\n";
    }
    s << "---------------------------------------- " << index_count << " indeces --------------------------------- \n";
    for (int i = 0; i < index_count; i++)
    {
        s << indices[i] << "  ";
    }
    s << "-------------------------------------------------------------------------------------------------------- \n";
    
    s << std::endl;
    
    return s.str();
    
}


//MARK: - Submeshes and Vertex Declarations

std::string VSC::OB::OgreEntityVertexDeclarationDescription(Ogre::Entity* entity)
{
    std::stringstream s;
    
    Ogre::MeshPtr meshPtr = entity->getMesh();
    Ogre::Mesh* mesh = meshPtr.get();
    
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        s << "Submesh " << i;
        
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
        
        if (submesh->useSharedVertices) std::cout << " (shared vertices)" << std::endl;
        
        Ogre::VertexDeclaration* declaration = vertex_data->vertexDeclaration;
        
        std::vector<Ogre::VertexElementSemantic> semantics;
        semantics += Ogre::VES_POSITION, Ogre::VES_NORMAL, Ogre::VES_DIFFUSE, Ogre::VES_SPECULAR, Ogre::VES_TEXTURE_COORDINATES;
        
        BOOST_FOREACH(Ogre::VertexElementSemantic semantic, semantics)
        {
            s << "Declaration for " << OgreVertexElementSemanticToString(semantic) << " : ";
            const Ogre::VertexElement* posElem = declaration->findElementBySemantic(semantic);
            if (posElem)
            {
                s << "FOUND with source " << posElem->getSource() << " index " << posElem->getIndex();
                s << " offset " << posElem->getOffset() << std::endl;
                //Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
            }
            else
            {
                s << "NOT FOUND" << std::endl;
            }
        }
    }
    
    return s.str();
}

//MARK: - Mesh Multi-Info (Probably obsolete...)

void VSC::OB::OgreMeshInformation(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vertices, std::vector<unsigned long>& indices)
{
    
    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
    
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;
    
    size_t vertex_count = OgreMeshVertexCount(mesh);
    size_t index_count = OgreMeshIndexCount(mesh);
    
    // Allocate space for the vertices and indices
    vertices.resize(vertex_count, Ogre::Vector3::ZERO);
    indices.resize(index_count, 0);
    
    added_shared = false;
    
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
        
        if(submesh->useSharedVertices)
        {
            added_shared = true;
            shared_offset = current_offset;
        }
        
        const Ogre::VertexElement* posElem =
        vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
        
        Ogre::HardwareVertexBufferSharedPtr vbuf =
        vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
        
        unsigned char* vertex =
        static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        
        // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
        //  as second argument. So make it float, to avoid trouble when Ogre::Real will
        //  be comiled/typedefed as double:
        
        //Ogre::Real* pReal;
        float* pReal;
        
        for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
            //vertices[current_offset + j] = (orient * (pt * scale)) + position;
            vertices[current_offset + j] = pt;
        }
        
        vbuf->unlock();
        next_offset += vertex_data->vertexCount;
        
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
        
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
        
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
        
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
        
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                static_cast<unsigned long>(offset);
            }
        }
        
        ibuf->unlock();
        current_offset = next_offset;
    }
}

//MARK: - Mesh Verteces

bool VSC::OB::OgreMeshIndeces(const Ogre::Mesh* const mesh, std::vector<unsigned long>& indices)
{
    
    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
    
    size_t index_offset = 0;

    size_t index_count = OgreMeshIndexCount(mesh);
    
    // Allocate space for the vertices and indices
    indices.resize(index_count, 0);
    
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
        
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
        
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
        
        size_t offset = index_offset;
        
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
            }
        }
        
        ibuf->unlock();
    }
    
    return true;
}


bool VSC::OB::OgreMeshVertices(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vertices)
{
    return OgreMeshVertexTriInfo(mesh, vertices, Ogre::VES_POSITION);
}

bool VSC::OB::OgreMeshNormals(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& normals)
{
    return OgreMeshVertexTriInfo(mesh, normals, Ogre::VES_NORMAL);
}

bool VSC::OB::OgreMeshDiffuseColors(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors)
{
    return OgreMeshVertexColorInfo(mesh, colors, Ogre::VES_DIFFUSE);
}

bool VSC::OB::OgreMeshSpecularColors(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors)
{
    return OgreMeshVertexColorInfo(mesh, colors, Ogre::VES_SPECULAR);
}

bool VSC::OB::OgreMeshTextureCoordinates(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector2>& coordinates)
{
    return OgreMeshVertexBiInfo(mesh, coordinates, Ogre::VES_TEXTURE_COORDINATES);
}

//MARK: - Utils

bool VSC::OB::OgreMeshVertexBiInfo(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector2>& vectors, Ogre::VertexElementSemantic semantic)
{
    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
    
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    
    size_t count = OgreMeshVertexCount(mesh);
    
    // Allocate space for the vertices and indices
    vectors.resize(count, Ogre::Vector2::ZERO);
    
    added_shared = false;
    
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        if (!submesh) throw VSCBadStateException("No Submesh");
        
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
        if (!vertex_data) throw VSCBadStateException("No VertexData");
        
        if(submesh->useSharedVertices && !added_shared)
        {
            added_shared = true;
            shared_offset = current_offset;
        }
        
        const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(semantic);
        if (!posElem) throw VSCBadStateException("No VertexElement");
        
        Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
        if (!vbuf.get()) throw VSCBadStateException("No HardwareVertexBuffer");
        
        unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        
        float* pReal;
        
        for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            Ogre::Vector2 pt(pReal[0], pReal[1]);
            BOOST_ASSERT(current_offset + j < count);
            vectors[current_offset + j] = pt;
        }
        
        vbuf->unlock();
        next_offset += vertex_data->vertexCount;
        current_offset = next_offset;
    }
    
    return true;
}

bool VSC::OB::OgreMeshVertexTriInfo(const Ogre::Mesh* const mesh, std::vector<Ogre::Vector3>& vectors, Ogre::VertexElementSemantic semantic)
{
    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
    
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    
    size_t count = OgreMeshVertexCount(mesh);
    
    // Allocate space for the vertices and indices
    vectors.resize(count, Ogre::Vector3::ZERO);
    
    added_shared = false;
    
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        if (!submesh) throw VSCBadStateException("No Submesh");
        
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
        if (!vertex_data) throw VSCBadStateException("No VertexData");
        
        if(submesh->useSharedVertices && !added_shared)
        {
            added_shared = true;
            shared_offset = current_offset;
        }
        
        const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(semantic);
        if (!posElem) throw VSCBadStateException("No VertexElement");
        
        Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
        if (!vbuf.get()) throw VSCBadStateException("No HardwareVertexBuffer");
        
        unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        
        float* pReal;
        
        for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
        {
            posElem->baseVertexPointerToElement(vertex, &pReal);
            Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
            BOOST_ASSERT(current_offset + j < count);
            vectors[current_offset + j] = pt;
        }
        
        vbuf->unlock();
        next_offset += vertex_data->vertexCount;
        current_offset = next_offset;
    }
    
    return true;
}

bool VSC::OB::OgreMeshVertexColorInfo(const Ogre::Mesh* const mesh, std::vector<Ogre::RGBA>& colors, Ogre::VertexElementSemantic semantic)
{
    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
    
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    
    size_t count = OgreMeshVertexCount(mesh);
    
    // Allocate space for the vertices and indices
    colors.resize(count, 0);
    
    added_shared = false;
    
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        if (!submesh) throw VSCBadStateException("No Submesh");
        
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
        if (!vertex_data) throw VSCBadStateException("No VertexData");
        
        if(submesh->useSharedVertices && !added_shared)
        {
            added_shared = true;
            shared_offset = current_offset;
        }
        
        const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(semantic);
        if (!posElem) throw VSCBadStateException("No VertexElement");
        
        Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
        if (!vbuf.get()) throw VSCBadStateException("No HardwareVertexBuffer");
        
        unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        
        Ogre::VertexElementType format = Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType();
        
        switch (format)
        {
            case Ogre::VET_COLOUR_ARGB:
                std::cout << "VET_COLOUR_ARGB" << std::endl;
                break;
            case Ogre::VET_COLOUR_ABGR:
                std::cout << "VET_COLOUR_ABGR" << std::endl;
                break;
            default:
                OGRE_EXCEPT(0, "Unknown RenderSystem color format", "Batch::addSub()");
                break;
        }
        
        Ogre::RGBA* color;
        
        for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
        {
            posElem->baseVertexPointerToElement(vertex, &color);
            BOOST_ASSERT(current_offset + j < count);
            colors[current_offset + j] = *color;
        }
        
        vbuf->unlock();
        next_offset += vertex_data->vertexCount;
        current_offset = next_offset;
    }
    
    return true;
}


