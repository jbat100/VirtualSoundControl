OGRE_SDK=/Users/jonathanthorpe/Dev/Libraries/OgreSDK
 
all:
	# create directories (if they don't exist)
	mkdir -p $(CONTENT_PATH)/Components
	mkdir -p $(CONTENT_PATH)/Plugins
	mkdir -p $(CONTENT_PATH)/Resources
	mkdir -p $(CONTENT_PATH)/Frameworks
 
	# copy components
	cp $(OGRE_SDK)/lib/libOgrePaging.dylib $(CONTENT_PATH)/Components
	cp $(OGRE_SDK)/lib/libOgreProperty.dylib $(CONTENT_PATH)/Components
	cp $(OGRE_SDK)/lib/libOgreRTShaderSystem.dylib $(CONTENT_PATH)/Components
	cp $(OGRE_SDK)/lib/libOgreTerrain.dylib $(CONTENT_PATH)/Components
 
	# copy plugins
	cp $(OGRE_SDK)/lib/Plugin_BSPSceneManager.dylib $(CONTENT_PATH)/Plugins
	cp $(OGRE_SDK)/lib/Plugin_CgProgramManager.dylib $(CONTENT_PATH)/Plugins
	cp $(OGRE_SDK)/lib/Plugin_OctreeSceneManager.dylib $(CONTENT_PATH)/Plugins
	cp $(OGRE_SDK)/lib/Plugin_OctreeZone.dylib $(CONTENT_PATH)/Plugins
	cp $(OGRE_SDK)/lib/Plugin_PCZSceneManager.dylib $(CONTENT_PATH)/Plugins
	cp $(OGRE_SDK)/lib/Plugin_ParticleFX.dylib $(CONTENT_PATH)/Plugins
	cp $(OGRE_SDK)/lib/RenderSystem_GL.dylib $(CONTENT_PATH)/Plugins
 
	# copy frameworks
	cp -R /Library/Frameworks/Cg.framework $(CONTENT_PATH)/Frameworks/
	cp -R $(OGRE_SDK)/lib/release/Ogre.framework $(CONTENT_PATH)/Frameworks/
 
	# copy resource files
	cp -R Resources/* $(CONTENT_PATH)/Resources/
 
	# change permission on all files
	chmod -R u+rwx $(CONTENT_PATH)/*
	chmod -R go+r $(CONTENT_PATH)/*
 
clean:
	rm -rf $(CONTENT_PATH)/Components
	rm -rf $(CONTENT_PATH)/Plugins
	rm -rf $(CONTENT_PATH)/Resources
	rm -rf $(CONTENT_PATH)/Frameworks