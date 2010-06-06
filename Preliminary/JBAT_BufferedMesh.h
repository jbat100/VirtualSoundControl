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


#define VERTEX_DATA     0
#define NORMAL_DATA     1
#define TEXTURE_DATA    2
#define INDEX_DATA      3


class JBAT_BufferedMesh : public JBAT_BaseMesh {
	
public:
	
	JBAT_BufferedMesh() : JBAT_BaseMesh() {buffersAreCreated = false;}
	JBAT_BufferedMesh(std::string const & fileName) : JBAT_BaseMesh(fileName) {buffersAreCreated = false;}
	JBAT_BufferedMesh(const JBAT_BufferedMesh& mesh) : JBAT_BaseMesh(mesh) {buffersAreCreated = false;}
	~JBAT_BufferedMesh();
	
	void bufferedScale(GLfloat fScaleValue);
	void bufferedDraw(void);
	
protected:
	
	GLuint bufferObjects[4];
	bool buffersAreCreated;

};

typedef enum _JBAT_OpenGLClientStates {
	JBAT_OpenGLClientStatesDefault,
	JBAT_OpenGLClientStatesOff
} JBAT_OpenGLClientStates;

extern void JBAT_SetUpOpenGLClientStates(JBAT_OpenGLClientStates clientsStatesSetup);

#endif