/*
 *  BufferedMesh.cpp
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 14/03/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#include "JBAT_BufferedMesh.h"
#include <iostream>

JBAT_BufferedMesh::JBAT_BufferedMesh() : JBAT_BaseMesh() {
	buffersAreCreated = false; 
	setToDefault();
}

JBAT_BufferedMesh::JBAT_BufferedMesh(std::string const & fileName) : JBAT_BaseMesh(fileName) {
	buffersAreCreated = false; 
	setToDefault();
}

JBAT_BufferedMesh::JBAT_BufferedMesh(const JBAT_BufferedMesh& mesh) : JBAT_BaseMesh(mesh) {
	buffersAreCreated = false;
	glUsage = mesh.glUsage;
}


JBAT_BufferedMesh::~JBAT_BufferedMesh(void) { 
	std::cout << "\nIn JBAT_BufferedMesh destructor";
	// Delete buffer objects
	glDeleteBuffers(4, bufferObjects);
}

void JBAT_BufferedMesh::setToDefault() {
	std::cout << "\nSet glUsage to static draw";
	glUsage = GL_STATIC_DRAW;
}

void JBAT_BufferedMesh::setGlUsage(GLenum u) {
	
	if (glUsage != u) {
		glUsage = u;
		buffersAreCreated = false;
	}

}
GLenum JBAT_BufferedMesh::getGlUsage() {
	return glUsage;
}

void JBAT_BufferedMesh::addTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3], M3DVector4f vColors[3]) {
	JBAT_BaseMesh::addTriangle(verts, vNorms, vTexCoords, vColors);
}

void JBAT_BufferedMesh::readVTKFile(std::string const & vtk_file) {
	JBAT_BaseMesh::readVTKFile(vtk_file);
	std::cout << "\nImported vtk file, got " << nNumVerts << " vertices and " << getCellCount() << " polys";
	endMesh();
}

void JBAT_BufferedMesh::readOBJFile(std::string const & obj_file) {
	JBAT_BaseMesh::readVTKFile(obj_file);
	std::cout << "\nImported obj file, got " << nNumVerts << " vertices and " << getCellCount() << " polys";
	endMesh();
}


//////////////////////////////////////////////////////////////////
// Compact the data. This is a nice utility, but you should really
// save the results of the indexing for future use if the model data
// is static (doesn't change).
void JBAT_BufferedMesh::endMesh() {
	
	//std::cout << "\nGenerating buffers with " << nNumVerts << " vertices and " << getCellCount() << " polys";
	
	// Create the buffer objects
    glGenBuffers(4, bufferObjects);
    
    // Copy data to video memory
    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pVerts, glUsage);
    
    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pNorms, glUsage);
    
    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*2, pTexCoords, glUsage);
    
    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*nNumIndexes, pIndexes, glUsage);
    
 

//    // Free older, larger arrays
//    delete [] pIndexes;
//    delete [] pVerts;
//    delete [] pNorms;
//    delete [] pTexCoords;
//	
//    // Reasign pointers so they are marked as unused
//    pIndexes = NULL;
//    pVerts = NULL;
//    pNorms = NULL;
//    pTexCoords = NULL;
 
	buffersAreCreated = true;
	
}

///////////////////////////////////////////////////////////////////////////
// Scale of the vertices. The only way to do this is to map the VBO back
// into client memory, then back again
void JBAT_BufferedMesh::scale(GLfloat fScaleValue) {
	
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    M3DVector3f *pVertexData = (M3DVector3f *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    
    if(pVertexData != NULL)
	{
        for(int i = 0; i < nNumVerts; i++)
            m3dScaleVector3(pVertexData[i], fScaleValue);
		
        glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
}

//////////////////////////////////////////////////////////////////////////
// Draw - make sure you call glEnableClientState for these arrays
void JBAT_BufferedMesh::draw() {
	
	if (buffersAreCreated) {
		
		//std::cout << "\nDrawing buffers";
		
		// Here's where the data is now
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
		glVertexPointer(3, GL_FLOAT, 0,  0);
		
		// Normal data
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
		glNormalPointer(GL_FLOAT, 0, 0);
		
		// Texture coordinates
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		// Colors
		//glColorPointer()
		
		// Indexes
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
		glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, 0);
	
	}
	
}


std::ostream & operator<<(std::ostream& s, JBAT_BufferedMesh & mesh) {
	s << "\nJBAT_BufferedMesh with " << mesh.nNumVerts << " vertices and " << mesh.getCellCount() << " polys";
	return s;
}



void JBAT_SetupOpenGLClientStates(JBAT_OpenGLClientStates clientsStatesSetup) {

	/*
	 glEnableClientState can be called with the following arguments 
	
	GL_COLOR_ARRAY,
	GL_EDGE_FLAG_ARRAY,
	GL_FOG_COORD_ARRAY,
	GL_INDEX_ARRAY,
	GL_NORMAL_ARRAY,
	GL_SECONDARY_COLOR_ARRAY,
	GL_TEXTURE_COORD_ARRAY, and
	GL_VERTEX_ARRAY
	 
	 */ 
	
	if (clientsStatesSetup == JBAT_OpenGLClientStatesDefault) {
	
		std::cout << "\nEnabling client states";
		//glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		
	}
	
	else if (clientsStatesSetup == JBAT_OpenGLClientStatesOff) {
		
		std::cout << "\nDisabling client states";
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
	}

	
}

