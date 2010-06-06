/*
 *  VBOMesh.h
 *  OpenGL SuperBible
 *
 *  Copyright 2007 Richard S. Wright Jr.. All rights reserved.
 *  This class allows you to simply add triangles as if this class were a 
 *  container. The AddTriangle() function searches the current list of triangles
 *  and determines if the vertex/normal/texcoord is a duplicate. If so, it addes
 *  an entry to the index array instead of the list of vertices.
 *  When finished, call EndMesh() to free up extra unneeded memory that is reserved
 *  as workspace when you call BeginMesh().
 *
 *  This class can easily be extended to contain other vertex attributes, and to 
 *  save itself and load itself from disk (thus forming the beginnings of a custom
 *  model file format).
 *
 *  Very similiar to CTriangleMesh, except the final arrays are stored in Vertex buffer objects
 *  Before anybody flames me, bear in mind this is not a book on object oriented programming, 
 *  and I'm trying to be nice to the C people ;-)
 *  Yes, I know all about inheritance, and it's all good....
 */

#ifndef _JBAT_BASE_MESH_
#define _JBAT_BASE_MESH_

#define DEFAULT_MAX_VERTEX_COUNT 10000
#define DEFAULT_MAX_INDEX_COUNT 10000


//#include "gltools.h"
#include "Math3d.h"

#include <string>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>


class JBAT_BaseMesh {
	
public:
	
	JBAT_BaseMesh(void);
	JBAT_BaseMesh(std::string const & fileName);
	JBAT_BaseMesh(const JBAT_BaseMesh& mesh);
	~JBAT_BaseMesh(void);
	
	// Use these three functions to add triangles
	void initialize(GLuint nMaxVerts);
	void addTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3], M3DVector4f vColors[3]);
	void readVTKFile(std::string const & vtk_file);
	void readOBJFile(std::string const & obj_file);
	
	// Draw - make sure you call glEnableClientState for these arrays
	void endMesh(void);
	void draw(void);
	
	// Useful for statistics
	inline GLuint getIndexCount(void) const { return nNumIndexes; }
	inline GLuint getCellCount(void) const { return nNumIndexes/3; }
	inline GLuint getVertexCount(void) const { return nNumVerts; }
	
	inline GLuint getMaxIndexCount(void) const { return nMaxIndexes; }
	inline GLuint getMaxCellCount(void) const { return nMaxIndexes/3; }
	inline GLuint getMaxVertexCount(void) const { return nMaxVerts; }
	
	// In place scale of the vertices
	void scale(GLfloat fScaleValue);
	
	// Getters
	float* getVertexCoordinates(int v) const;
	float* getVertexNormals(int v) const;
	float* getVertexTextureCoordinates(int v) const;
	float* getVertexColors(int v) const;
	int* getTriangleNodeIndexes(int p) const;
	
	// copiers
	void copyVertexCoordinates(int v, M3DVector3f coordinates) const;
	void copyVertexNormals(int v, M3DVector3f normals) const;
	void copyVertexTextureCoordinates(int v, M3DVector2f textureCoordinates) const;
	void copyVertexColors(int v, M3DVector3f colors) const;
	void copyTriangleNodeIndexes(int p, int& n1, int& n2, int& n3) const;
	
	void setVertexCoordinates(int v, M3DVector3f coordinates);
	void setVertexNormals(int v, M3DVector3f normals);
	void setVertexColor(int v, M3DVector4f color);
	void setVertexTextureCoordinates(int v, M3DVector2f textureCoordinates);
	void setTriangleNodeIndexes(int p, int n1, int n2, int n3);
	
	
	
	
protected:
	
	GLushort  *pIndexes;          // Array of indexes
	M3DVector3f *pVerts;        // Array of vertices
	M3DVector3f *pNorms;        // Array of normals
	M3DVector4f *pColors;		// Array of colors
	M3DVector2f *pTexCoords;    // Array of texture coordinates
	
	GLuint nMaxIndexes;         // Maximum workspace
	GLuint nMaxVerts; 
	GLuint nNumIndexes;         // Number of indexes currently used
	GLuint nNumVerts;           // Number of vertices actually used
	
	bool needsDisplay;
	
};

typedef enum _JBAT_OpenGLMeshType {
	JBAT_OpenGLMeshTypeDefault,
	JBAT_OpenGLMeshTypeWireframe,
	JBAT_OpenGLMeshTypeFull
} JBAT_OpenGLMeshType;

typedef enum _JBAT_OpenGLColorSetup {
	JBAT_OpenGLColorSetupDefault
} JBAT_OpenGLColorSetup;


extern void JBAT_SetUpOpenGLMaterial(JBAT_OpenGLMeshType meshType, JBAT_OpenGLColorSetup colorSetup);

#endif
