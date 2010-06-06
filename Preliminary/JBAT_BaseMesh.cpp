/*
 *  VBOMesh.cpp
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

#include "JBAT_BaseMesh.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>






///////////////////////////////////////////////////////////
// Constructor, does what constructors do... set everything to zero or NULL
JBAT_BaseMesh::JBAT_BaseMesh(void) {
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;
    
	
    nMaxIndexes = DEFAULT_MAX_VERTEX_COUNT;
	nMaxVerts = DEFAULT_MAX_VERTEX_COUNT;
    nNumIndexes = 0;
    nNumVerts = 0;
}

JBAT_BaseMesh::JBAT_BaseMesh(std::string const & fileName) {
	
	pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;
    
	
    nMaxIndexes = DEFAULT_MAX_VERTEX_COUNT;
	nMaxVerts = DEFAULT_MAX_VERTEX_COUNT;
    nNumIndexes = 0;
    nNumVerts = 0;
	
	readVTKFile(fileName);
}

JBAT_BaseMesh::JBAT_BaseMesh(const JBAT_BaseMesh& mesh) {
	
	nMaxIndexes = mesh.getMaxVertexCount();
	nMaxIndexes = mesh.getMaxIndexCount();
	nNumIndexes = mesh.getIndexCount();
	nNumVerts = mesh.getVertexCount();
	
	pIndexes = new GLushort[nNumIndexes];
    pVerts = new M3DVector3f[nNumVerts];
    pNorms = new M3DVector3f[nNumVerts];
	pColors = new M3DVector4f[nNumVerts];
    pTexCoords = new M3DVector2f[nNumVerts];
	
	// we should have access to the private members of mesh
	
	int i;
	
	for (i = 0; i < nNumIndexes; i++) {
		pIndexes[i] = mesh.pIndexes[i];
	}
	
	for (i = 0; i < nNumVerts; i++) {
		m3dCopyVector3(pVerts[i], mesh.pVerts[i]);
		m3dCopyVector3(pNorms[i], mesh.pNorms[i]);
		m3dCopyVector4(pColors[i], mesh.pColors[i]);
		m3dCopyVector3(pTexCoords[i], mesh.pTexCoords[i]);
	}
	
}
    
////////////////////////////////////////////////////////////
// Free any dynamically allocated memory. For those C programmers
// coming to C++, it is perfectly valid to delete a NULL pointer.
JBAT_BaseMesh::~JBAT_BaseMesh(void) {
	
	
   
	std::cout << "In JBAT_BaseMesh destructor";
	
	// delete on NULL should be fine
    
	
	delete [] pIndexes;
	delete [] pVerts;
	delete [] pNorms;
	delete [] pColors;
	delete [] pTexCoords;

	
	pIndexes = NULL;
	pVerts = NULL;
	pNorms = NULL;
	pColors = NULL;
	pTexCoords = NULL;
    
}
    
////////////////////////////////////////////////////////////
// Start assembling a mesh. You need to specify a maximum amount
// of indexes that you expect. The EndMesh will clean up any uneeded
// memory. This is far better than shreading your heap with STL containers...
// At least that's my humble opinion.
void JBAT_BaseMesh::initialize(GLuint nMaxVerts) 
{
    // Just in case this gets called more than once...
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
	delete [] pColors;
    delete [] pTexCoords;
    
    nMaxIndexes = nMaxVerts;
    nNumIndexes = 0;
    nNumVerts = 0;
    
    // Allocate new blocks
    pIndexes = new GLushort[nMaxIndexes];
    pVerts = new M3DVector3f[nMaxVerts];
    pNorms = new M3DVector3f[nMaxVerts];
	pColors = new M3DVector4f[nMaxVerts];
    pTexCoords = new M3DVector2f[nMaxVerts];
	
}

/**
 *    Read a VTK file
 */

void JBAT_BaseMesh::readVTKFile(std::string const & vtk_file)  {
	
	/* We're using C-style streams here because I believe they're still faster */
	FILE * f = fopen(vtk_file.c_str(), "r");
	if (!f)
		throw std::runtime_error(std::string("Could not open " + vtk_file + " for reading."));
	
	static const int bs = 256;
	char b[bs];
	
	fgets(b, bs, f);
	if (strcmp(b, "# vtk DataFile Version 3.0\n") && strcmp(b, "# vtk DataFile Version 2.0\n"))
		throw std::runtime_error("Bad VTK header.");
	
	fgets(b, bs, f);
	
	fgets(b, bs, f);
	if (strcmp(b, "ASCII\n"))
		throw std::runtime_error("VTK file must be ASCII.");
	
	fgets(b, bs, f);
	if (strcmp(b, "DATASET POLYDATA\n") && strcmp(b, "DATASET UNSTRUCTURED_GRID\n"))
		throw std::runtime_error("Could not find POLYDATA or UNSTRUCTURED_GRID in VTK file.");
	
	int num_points;
	fscanf(f, "%s %d %s", b, &num_points, b);
	
	// Reinitialize arrays with right size
	if (pVerts) {
		delete [] pVerts;
	}
	if (pNorms) {
		delete [] pNorms;
	}
	if (pTexCoords) {
		delete [] pTexCoords;
	}
	
    nNumVerts = (GLuint)num_points;
    pVerts = new M3DVector3f[num_points];
    pNorms = new M3DVector3f[num_points];
	pColors = new M3DVector4f[num_points];
    pTexCoords = new M3DVector2f[num_points];
	
	// Read point coordinates 
	for (int i = 0; i < num_points; i++)
	{
		fscanf(f, "%f %f %f", &pVerts[i][0], &pVerts[i][1], &pVerts[i][2]);
		
		// While were at it, set the colors to white 
		pColors[i][0] = 1.0;
		pColors[i][1] = 1.0;
		pColors[i][2] = 1.0;
		pColors[i][3] = 1.0;
	}
	
	// Find polys
	bool foundPolys = false;
	while (!feof (f) && !foundPolys)
	{
		int num;
		int vals;
		fscanf(f, "%s %d %d", b, &num, &vals);
		// Break if end of file
		if (feof (f))
			break;
		// Ignore lines
		if (strcmp(b, "LINES") == 0)
		{
			float d;
			for (int i = 0; i < vals; i++)
				fscanf(f, "%f", &d);
		}
		// If found polygons or cells tag, import
		if (strcmp(b, "POLYGONS") == 0 || strcmp(b, "CELLS") == 0)
		{
			nNumIndexes = (GLuint)num*3;
			// Reinitialize pIndexes with right size
			if (pIndexes) {
				delete [] pIndexes;
			}
			pIndexes = new GLushort[nNumIndexes];
			// Read polygons/cells
			for (int i = 0; i < num; i++)
			{
				int nodes;
				fscanf(f, "%d", &nodes);
				// only accept tris
				if (nodes == 3) {
					int nodes[3];
					fscanf(f, "%d %d %d", &nodes[0], &nodes[1], &nodes[2]);
					pIndexes[i*3] = (GLushort)nodes[0];
					pIndexes[(i*3)+1] = (GLushort)nodes[1];
					pIndexes[(i*3)+2] = (GLushort)nodes[2];
					
				}
				// If not a tri, ignore and print message
				else {
					std::cout << "Got polygon with bad number of nodes: " << nodes << ", expected 3" << std::endl;
					fgets(b, bs, f); // read rest of line
				}
			}
			foundPolys = true;
		}
	}
	
	// Find point data
	bool foundPointData = false;
	while (!feof (f) && !foundPointData) {
		fscanf(f, "%s", b);
		if (strcmp(b, "POINT_DATA") == 0) {
			foundPointData = true;
			break;
		}
	}
	fscanf(f, "%d", &num_points); // num is number of point data (normally should be the same as number of points)
	// Find point nornals
	bool foundPointNormals = false;
	while (!feof (f) && !foundPointNormals) {
		fscanf(f, "%s", b);
		if (strcmp(b, "NORMALS") == 0) {
			foundPointNormals = true;
			fgets(b, bs, f); // read rest of line
			break;
		}
	}
	// Read point normals
	for (int i = 0; i < num_points; i++)
	{
		fscanf(f, "%f %f %f", &pNorms[i][0], &pNorms[i][1], &pNorms[i][2]);
	}
	
	fclose (f);
}


void JBAT_BaseMesh::readOBJFile(std::string const & obj_file)  {
	
	initialize((GLuint)DEFAULT_MAX_VERTEX_COUNT);
	
	std::ifstream f(obj_file.c_str());
	int vCount = 0;
	int fCount = 0;
	
	while (1)
	{
		std::string t;
		f >> t;
		if (f.good() == false)
			break;
		
		if (t == "v")
		{
			f >> pVerts[vCount][0] >> pVerts[vCount][1] >> pVerts[vCount][2];
			vCount++;
		}
		else if (t == "f")
		{
			/* expects only triangles! */
			int nodes[3];
			f >> nodes[0] >> nodes[1] >> nodes[2];
			pIndexes[vCount*3] = (GLushort)nodes[0];
			pIndexes[(vCount*3)+1] = (GLushort)nodes[1];
			pIndexes[(vCount*3)+2] = (GLushort)nodes[2];
			fCount++;
		}
		else
		{
			std::cout << "Unknown type." << t << std::endl;
		}
	}
	
	// Resize container arrays 
	//pVerts = (M3DVector3f *) realloc(pVerts, sizeof(M3DVector3f)*vCount);
	//pIndexes = (GLushort *) realloc(pIndexes, sizeof(GLushort)*fCount*3);
	nNumVerts = (GLuint)vCount;
	nNumIndexes = (GLuint)fCount*3;
	endMesh();
}

  
/////////////////////////////////////////////////////////////////
// Add a triangle to the mesh. This searches the current list for identical
// (well, almost identical - these are floats you know...) verts. If one is found, it
// is added to the index array. If not, it is added to both the index array and the vertex
// array grows by one as well.
void JBAT_BaseMesh::addTriangle(M3DVector3f verts[3], M3DVector3f vNorms[3], M3DVector2f vTexCoords[3], M3DVector4f vColors[3])
{
    const  float e = 0.000001; // How small a difference to equate
	
    // First thing we do is make sure the normals are unit length!
    // It's almost always a good idea to work with pre-normalized normals
    m3dNormalizeVector(vNorms[0]);
    m3dNormalizeVector(vNorms[1]);
    m3dNormalizeVector(vNorms[2]);
	
	
    // Search for match - triangle consists of three verts
    for(GLuint iVertex = 0; iVertex < 3; iVertex++)
	{
        GLuint iMatch = 0;
        for(iMatch = 0; iMatch < nNumVerts; iMatch++)
		{
            // If the vertex positions are the same
            if(m3dCloseEnough(pVerts[iMatch][0], verts[iVertex][0], e) &&
               m3dCloseEnough(pVerts[iMatch][1], verts[iVertex][1], e) &&
               m3dCloseEnough(pVerts[iMatch][2], verts[iVertex][2], e) &&
			   
               // AND the Normal is the same...
               m3dCloseEnough(pNorms[iMatch][0], vNorms[iVertex][0], e) &&
               m3dCloseEnough(pNorms[iMatch][1], vNorms[iVertex][1], e) &&
               m3dCloseEnough(pNorms[iMatch][2], vNorms[iVertex][2], e) &&
			   
			   // AND the Color is the same...
               m3dCloseEnough(pColors[iMatch][0], vColors[iVertex][0], e) &&
               m3dCloseEnough(pColors[iMatch][1], vColors[iVertex][1], e) &&
               m3dCloseEnough(pColors[iMatch][2], vColors[iVertex][2], e) &&
			   
			   // And Texture is the same...
			   m3dCloseEnough(pTexCoords[iMatch][0], vTexCoords[iVertex][0], e) &&
			   m3dCloseEnough(pTexCoords[iMatch][1], vTexCoords[iVertex][1], e))
			{
                // Then add the index only
                pIndexes[nNumIndexes] = iMatch;
                nNumIndexes++;
                break;
			}
		}
		
        // No match for this vertex, add to end of list
        if(iMatch == nNumVerts)
		{
            memcpy(pVerts[nNumVerts], verts[iVertex], sizeof(M3DVector3f));
            memcpy(pNorms[nNumVerts], vNorms[iVertex], sizeof(M3DVector3f));
            memcpy(pTexCoords[nNumVerts], &vTexCoords[iVertex], sizeof(M3DVector2f));
            pIndexes[nNumIndexes] = nNumVerts;
            nNumIndexes++; 
            nNumVerts++;
		}   
	}
}
    

//////////////////////////////////////////////////////////////////
// Getters

float* JBAT_BaseMesh::getVertexCoordinates(int v) const {
	
	if (v < nNumVerts)
		return &pVerts[v][0];
	return NULL;
}

float* JBAT_BaseMesh::getVertexNormals(int v) const {
	if (v < nNumVerts)
		return &pNorms[v][0];
	return NULL;
}

float* JBAT_BaseMesh::getVertexColors(int v) const {
	if (v < nNumVerts)
		return &pColors[v][0];
	return NULL;
}

float* JBAT_BaseMesh::getVertexTextureCoordinates(int v) const {
	if (v < nNumVerts)
		return &pTexCoords[v][0];
	return NULL;
}

int* JBAT_BaseMesh::getTriangleNodeIndexes(int p) const {
	if (p < nNumIndexes / 3)
		&pIndexes[p*3];
	return NULL;
}


//////////////////////////////////////////////////////////////////
// Copiers
void JBAT_BaseMesh::copyVertexCoordinates(int v, M3DVector3f coordinates) const {
	coordinates[0] = pVerts[v][0];
	coordinates[1] = pVerts[v][1];
	coordinates[2] = pVerts[v][2];
}

void JBAT_BaseMesh::copyVertexNormals(int v, M3DVector3f normals) const {
	normals[0] = pNorms[v][0];
	normals[1] = pNorms[v][1];
	normals[2] = pNorms[v][2];
}

void JBAT_BaseMesh::copyVertexColors(int v, M3DVector3f colors) const {
	colors[0] = pColors[v][0];
	colors[1] = pColors[v][1];
	colors[2] = pColors[v][2];
	colors[3] = pColors[v][3];
}

void JBAT_BaseMesh::copyVertexTextureCoordinates(int v, M3DVector2f textureCoordinates) const {
	textureCoordinates[0] = pTexCoords[v][0];
	textureCoordinates[1] = pTexCoords[v][1];
	textureCoordinates[2] = pTexCoords[v][2];
}
void JBAT_BaseMesh::copyTriangleNodeIndexes(int p, int& n1, int& n2, int& n3) const {
	n1 = pIndexes[p*3];
	n2 = pIndexes[(p*3)+1];
	n3 = pIndexes[(p*3)+2];
}


//////////////////////////////////////////////////////////////////
// Setters
void JBAT_BaseMesh::setVertexCoordinates(int v, M3DVector3f coordinates) {
	pVerts[v][0] = coordinates[0];
	pVerts[v][1] = coordinates[1];
	pVerts[v][2] = coordinates[2];
}
void JBAT_BaseMesh::setVertexNormals(int v, M3DVector3f normals) {
	pNorms[v][0] = normals[0];
	pNorms[v][1] = normals[1];
	pNorms[v][2] = normals[2];
}
void JBAT_BaseMesh::setVertexTextureCoordinates(int v, M3DVector2f textureCoordinates) {
	pTexCoords[v][0] = textureCoordinates[0];
	pTexCoords[v][1] = textureCoordinates[1];
	pTexCoords[v][2] = textureCoordinates[2];
}
void JBAT_BaseMesh::setTriangleNodeIndexes(int p, int n1, int n2, int n3) {
	pIndexes[p*3] = n1;
	pIndexes[(p*3)+1] = n2;
	pIndexes[(p*3)+2] = n3;
}


//////////////////////////////////////////////////////////////////
// Compact the data. This is a nice utility, but you should really
// save the results of the indexing for future use if the model data
// is static (doesn't change).
void JBAT_BaseMesh::endMesh(void) {
    
	// Allocate smaller arrays
    GLushort *pPackedIndexes = new GLushort[nNumIndexes];
    M3DVector3f *pPackedVerts = new M3DVector3f[nNumVerts];
    M3DVector3f *pPackedNorms = new M3DVector3f[nNumVerts];
    M3DVector2f *pPackedTex = new M3DVector2f[nNumVerts];
    
    // Copy data to smaller arrays
    memcpy(pPackedIndexes, pIndexes, sizeof(GLushort)*nNumIndexes);
    memcpy(pPackedVerts, pVerts, sizeof(M3DVector3f)*nNumVerts);
    memcpy(pPackedNorms, pNorms, sizeof(M3DVector3f)*nNumVerts);
    memcpy(pPackedTex, pTexCoords, sizeof(M3DVector2f)*nNumVerts);
    
    // Free older, larger arrays
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;
	
    // Reasign pointers
    pIndexes = pPackedIndexes;
    pVerts = pPackedVerts;
    pNorms = pPackedNorms;
    pTexCoords = pPackedTex;
}


// Draw - make sure you call glEnableClientState for these arrays
void JBAT_BaseMesh::draw(void) {
	// Here's where the data is now
	glVertexPointer(3, GL_FLOAT,0, pVerts);
	glNormalPointer(GL_FLOAT, 0, pNorms);
	glTexCoordPointer(2, GL_FLOAT, 0, pTexCoords);
	
	// Draw them
	glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, pIndexes);
}





///////////////////////////////////////////////////////////////////////////
// Scale of the vertices.
void JBAT_BaseMesh::scale(GLfloat fScaleValue) {
	for(int i = 0; i < nNumVerts; i++)
		m3dScaleVector3(pVerts[i], fScaleValue);
}





extern void JBAT_SetUpOpenGLMeshSetup(JBAT_OpenGLMeshType meshType, JBAT_OpenGLColorSetup colorSetup) {
	
	/* 
	 
	 WIKIPEDIA:
	 
	 In computer graphics, back-face culling determines whether a polygon of a graphical object is visible. 
	 It is a step in the graphical pipeline that tests whether the points in the polygon appear in clockwise 
	 or counter-clockwise order when projected onto the screen. If the user has specified that front-facing 
	 polygons have a clockwise winding, if the polygon projected on the screen has a counter-clockwise winding 
	 it has been rotated to face away from the camera and will not be drawn. The process makes rendering 
	 objects quicker and more efficient by reducing the number of polygons for the program to draw. For 
	 example, in a city street scene, there is generally no need to draw the polygons on the sides of the 
	 buildings facing away from the camera; they are completely occluded by the sides facing the camera.
	 
	 RANDOM FORUMS:
	 
	 http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=275143
	 
	 GL_CULL_FACE is to be enabled for performance reasons, as it easily removes half of the triangles to draw, 
	 normally without visual artifacts if your geometry is watertight, and CCW.
	 GL_DEPTH_TEST is to be enabled to avoid ugly artifacts that depend on the angle of view and drawing order 
	 (otherwise ie. if back of the cube is drawn last, if will appear "above" the front of the cube).
	 So yes, both should be enabled.
	 
	 http://www.videotutorialsrock.com/opengl_tutorial/reference.php
	 
	 Suggests doing this, but I'm not quite sure why... Why not just disable face culling completely?
	 
	 glEnable(GL_CULL_FACE); //Enable backface culling
	 glCullFace(GL_FRONT);   //Cull front faces
	 drawConvexObject();     //Draw back faces of convex surface
	 glCullFace(GL_BACK);    //Cull back faces
	 drawConvexObject();     //Draw front faces of convex surface
	 
	 IMPORTANT:
	 
	 glDisable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work <--- IS IT?
	 glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	 glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	 glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	 
	 The shininess parameter.  A higher number indicates a more concentrated
	 shiny area, while a lower number indicates a larger shiny area.  The
	 shininess must be between 0 and 128.
	 
	 glMaterialf(GL_FRONT, GL_SHININESS, 15.0f);
	 
	 IMPORTANT:
	 
	 Cannot call transformation (matrices) between glBegin and glEnd (GL_TRIANGLES, GL_QUADS...)
	 
	 
	 glDisable(GL_TEXTURE_2D) once the texturing is done

	 
	 
	 http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=145662
	 
	 
	 Great, now you have the right colours, but no lighting.
	 Do the following to get the right colours AND lighting:-
	 glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	 glEnable(GL_COLOR_MATERIAL);
	 
	 Now your lighting will work with your colours correctly.
	 What is happening is this:-
	 colours are ignored when lighting is enabled, because lighting uses materials instead. 
	 When you enable GL_COLOR_MATERIAL you are telling openGL that every time a glColor3f 
	 (or equivelent entry in your colour array) is issued, change the specified material property 
	 to that colour instead (GL_DIFFUSE, GL_AMBIENT or a convenient way of combining the two by 
	 using the GL_AMBIENT_AND_DIFFUSE constant).
	 
	 
	 
	 */
	
	if (meshType == JBAT_OpenGLMeshTypeDefault) {
		
		// POLY RENDERING
		glEnable(GL_DEPTH_TEST); 
		glFrontFace(GL_CCW); 
		glEnable(GL_CULL_FACE);
		
		// COLOR
		glEnable(GL_COLOR_MATERIAL); //Enable color (specular component is defined in the JBAT_Object subclass)
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
		
	}
	
	
}



