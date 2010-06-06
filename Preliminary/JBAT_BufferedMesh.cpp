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


JBAT_BufferedMesh::~JBAT_BufferedMesh(void) { 
	
	std::cout << "In JBAT_BufferedMesh destructor";
	
	// Delete buffer objects
	glDeleteBuffers(4, bufferObjects);
}

//////////////////////////////////////////////////////////////////
// Compact the data. This is a nice utility, but you should really
// save the results of the indexing for future use if the model data
// is static (doesn't change).
/*void BufferedMesh::endMesh(void) {
 
 
	
    // Create the buffer objects
    glGenBuffers(4, bufferObjects);
    
    // Copy data to video memory
    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pVerts, GL_STATIC_DRAW);
    
    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pNorms, GL_STATIC_DRAW);
    
    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*2, pTexCoords, GL_STATIC_DRAW);
    
    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*nNumIndexes, &pIndexes, GL_STATIC_DRAW);
    
 

    // Free older, larger arrays
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
	
    // Reasign pointers so they are marked as unused
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;
 

 
 buffersAreCreated = true;
	
}
 */

///////////////////////////////////////////////////////////////////////////
// Scale of the vertices. The only way to do this is to map the VBO back
// into client memory, then back again
void JBAT_BufferedMesh::bufferedScale(GLfloat fScaleValue) {
	
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
void JBAT_BufferedMesh::bufferedDraw(void) {
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





extern void JBAT_SetupOpenGLClientStates(JBAT_OpenGLClientStates clientsStatesSetup) {

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
	
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		
	}
	
	else if (clientsStatesSetup == JBAT_OpenGLClientStatesOff) {
		
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
	}

	
}

