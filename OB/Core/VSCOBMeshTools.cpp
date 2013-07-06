
#include "VSCOBMeshTools.h"
#include "VSCException.h"
#include "VSCRandom.h"

#include <Ogre/Ogre.h>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/set.hpp>
#include <boost/foreach.hpp>


using namespace boost::assign;

namespace VSC {
    
    namespace OB {
        
        /*
         *  Vertex Color Processors. Abstract class VertexColorProcessor does the boring work of finding the verteces,
         *  getting access to the color values and then the concrete sublasses overide the process vertex function
         *  which gets called for each vertex
         */
        
        class VertexColorProcessor
        {
        public:
            void processMesh(Ogre::Mesh* mesh, Ogre::VertexElementSemantic semantic);
        protected:
            virtual void processVertex(Ogre::SubMesh* submesh, const Ogre::VertexElement* element, unsigned char* vertex) = 0;
        };
        
        class RandomizeVertexColorProcessor : public VertexColorProcessor
        {
        protected:
            virtual void processVertex(Ogre::SubMesh* submesh, const Ogre::VertexElement* element, unsigned char* vertex);
        };
        
        class ConstantVertexColorProcessor : public VertexColorProcessor
        {
        public:
            ConstantVertexColorProcessor(const Ogre::RGBA& vertexColor) : mVertexColor(vertexColor) {}
        protected:
            virtual void processVertex(Ogre::SubMesh* submesh, const Ogre::VertexElement* element, unsigned char* vertex);
        private:
            Ogre::RGBA mVertexColor;
        };
        
    }
    
}

void VSC::OB::VertexColorProcessor::processMesh(Ogre::Mesh* mesh, Ogre::VertexElementSemantic semantic)
{
    if (semantic != Ogre::VES_SPECULAR && semantic != Ogre::VES_DIFFUSE)
    {
        throw VSCBadParameterException("Argument semantic should be Ogre::VES_SPECULAR or Ogre::VES_DIFFUSE");
    }
    
    // http://www.ogre3d.org/tikiwiki/tiki-index.php?page=RetrieveVertexData
    
    bool done_shared = false;
    
    done_shared = false;
    
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        if (!submesh) throw VSCBadStateException("No Submesh");
        
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
        if (!vertex_data) throw VSCBadStateException("No VertexData");
        
        if(submesh->useSharedVertices && !done_shared)
        {
            done_shared = true;
        }
        else if (submesh->useSharedVertices && done_shared)
        {
            continue;
        }
        
        const Ogre::VertexElement* element = vertex_data->vertexDeclaration->findElementBySemantic(semantic);
        if (!element) throw VSCBadStateException("No VertexElement");
        
        Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(element->getSource());
        if (!vbuf.get()) throw VSCBadStateException("No HardwareVertexBuffer");
        
        unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_NORMAL));
        
        for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
        {
            this->processVertex(submesh, element, vertex);
        }
        
        vbuf->unlock();
    }
}

void VSC::OB::RandomizeVertexColorProcessor::processVertex(Ogre::SubMesh* submesh, const Ogre::VertexElement* element, unsigned char* vertex)
{
    Ogre::RGBA* color;
    element->baseVertexPointerToElement(vertex, &color);
    *color = GenerateRandomNumber<Ogre::RGBA>(0x0000, 0xffff);
}

void VSC::OB::ConstantVertexColorProcessor::processVertex(Ogre::SubMesh* submesh, const Ogre::VertexElement* element, unsigned char* vertex)
{
    Ogre::RGBA* color;
    element->baseVertexPointerToElement(vertex, &color);
    *color = mVertexColor;
}

//MARK: - Vertex and Index Counts

size_t VSC::OB::OgreMeshVertexCount(const Ogre::Mesh* const mesh)
{
    bool added_shared = false;
    size_t vertex_count = 0;
    
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
    }
    
    return vertex_count;
}

size_t VSC::OB::OgreMeshIndexCount(const Ogre::Mesh* const mesh)
{
    size_t index_count = 0;
    
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);
        index_count += submesh->indexData->indexCount;
    }
    
    return index_count;
}

void VSC::OB::OgreMeshRandomizeVertexColors(Ogre::Mesh* mesh, Ogre::VertexElementSemantic semantic)
{
    RandomizeVertexColorProcessor processor;
    processor.processMesh(mesh, semantic);
}

void VSC::OB::OgreMeshConstantVertexColors(Ogre::Mesh* mesh, Ogre::RGBA color, Ogre::VertexElementSemantic semantic)
{
    ConstantVertexColorProcessor processor(color);
    processor.processMesh(mesh, semantic);
}


