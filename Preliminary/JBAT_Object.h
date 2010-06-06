/*
 *  JBAT_Object.h
 *  MeshFrames3
 *
 *  Created by Jonathan Thorpe on 14/05/2010.
 *  Copyright 2010 JBAT. All rights reserved.
 *
 */

#ifndef _JBAT_OBJECT_
#define _JBAT_OBJECT_


#include "JBAT_BufferedMesh.h"
#include "GLFrame.h"
#include "Math3d.h"

#include <iostream>
#include <string>
#include <list>


class JBAT_Object : public JBAT_BufferedMesh {
	
public:
	
	JBAT_Object();
	JBAT_Object(std::string const & objectName);
	JBAT_Object(const JBAT_Object& object);
	~JBAT_Object(void);
	
	
	void setToDefault();
	
	// CHILD OBJECTS
	void addChildObject(JBAT_Object* childObject, bool duplicate = false);
	void removeChildObject(JBAT_Object* childObject);
	void removeChildObjectAtIndex(int index);
	void removeAllChildObjects();
	JBAT_Object* getChildObjectAtIndex(int index);
	void setChildObjectAtIndex(int index, JBAT_Object*childObject, bool duplicate = false);
	void getChildObjects(std::list<JBAT_Object*>& childObject) const;
	void addChildObjects(const std::list<JBAT_Object*>& childObjects, bool duplicate = false);
	
	void setName(const std::string n);
	std::string getName();
	void setFrame(const GLFrame* f);
	GLFrame* getFrame(); 
	void setSpecularColor(const float r, const float g, const float b, const float a);
	float* getSpecularColor();
	void setShininess(float s);
	float getShininess();
	
	// OPENGL
	void setupOpenGL();
	
	
	friend std::ostream & operator<<(std::ostream& s, JBAT_Object const & object);
	
protected:
	
	GLFrame *frame;
	bool on;
	
	GLfloat shininess;
	bool shininessNeedsUpdate;
	M3DVector3f specularColor;
	bool specularColorNeedsUpdate;
	
	std::list<JBAT_Object*> childObjects;
	
	std::string name;
	std::list<std::string> meshPaths;  // contains paths to mesh files in deceasing order of resolution 
	
	
};

typedef enum _JBAT_OpenGLMaterialSetup {
	JBAT_OpenGLMaterialSetupDefault
} JBAT_OpenGLMaterialSetup;

void JBAT_SetUpOpenGLMaterial(JBAT_OpenGLMaterialSetup materialSetup);

#endif