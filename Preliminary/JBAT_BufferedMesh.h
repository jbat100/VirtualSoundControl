/*
 *  BufferedMesh.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 14/03/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_BUFFERED_MESH_
#define _JBAT_BUFFERED_MESH_


#include "JBAT_BaseMesh.h"
//#include "gltools.h"
#include "Math3d.h"
#include <string>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>


#define VERTEX_DATA     0
#define NORMAL_DATA     1
#define TEXTURE_DATA    2
#define INDEX_DATA      3


class JBAT_BufferedMesh : public JBAT_BaseMesh {
	
public:
	
	JBAT_BufferedMesh();
	JBAT_BufferedMesh(std::string const & fileName);
	JBAT_BufferedMesh(const JBAT_BufferedMesh& mesh);
	~JBAT_BufferedMesh();
	
	void setToDefault();
	
	void addTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3], M3DVector4f vColors[3]);
	void readVTKFile(std::string const & vtk_file);
	void readOBJFile(std::string const & obj_file);
	
	void endMesh();
	void scale(GLfloat fScaleValue);
	void draw();
	
	void setGlUsage(GLenum u);
	GLenum getGlUsage();
	
	friend std::ostream & operator<<(std::ostream& s, JBAT_BufferedMesh & mesh);
	
protected:
	
	GLuint bufferObjects[4];
	bool buffersAreCreated;
	
	GLenum glUsage; 
	// GL_STATIC_DRAW: rarely changes
	// GL_DYNAMIC_DRAW: occasionally changes
	// GL_STREAM_DRAW: changes all the time

};

typedef enum _JBAT_OpenGLClientStates {
	JBAT_OpenGLClientStatesDefault,
	JBAT_OpenGLClientStatesOff
} JBAT_OpenGLClientStates;

void JBAT_SetupOpenGLClientStates(JBAT_OpenGLClientStates clientsStatesSetup);

#endif