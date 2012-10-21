
#include "VSCOBResourceManager.h"

void VSC::OB::ResourceManager::setupResources(void)
{
    Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
    
    /*-------------------------------------------------------------------------------------*/
    
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    //cf.load(resourcePath + "resources.cfg");
    cf.load(mResourceFilePath);
    
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    
    Ogre::String secName, typeName, archName;
    
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            try {
                rsm->addResourceLocation(archName, typeName, secName);
            }
            catch (Ogre::Exception& e)
            {
                std::cout << "Exception : " << e.getFullDescription() << std::endl;
            }
        }
    }
    
    /*-------------------------------------------------------------------------------------*/
    
	Ogre::StringVector groups = rsm->getResourceGroups();        

	const Ogre::String resName("OgreBullet");
	{
		if (std::find(groups.begin(), groups.end(), resName) == groups.end())
		{

			rsm->createResourceGroup(resName);
            Ogre::String baseName;
            
            baseName = "/Library/VirtualSoundControl/Ogrebullet/Media";
            
            rsm->addResourceLocation(baseName, "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/textures", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/overlays", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/materials", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/models", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/gui", "FileSystem", resName);
		}
	}
    
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    
    this->internalSetupResources();
}


void VSC::OB::ResourceManager::loadResources(void)
{
    Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
    
    rsm->initialiseAllResourceGroups(); // is this necessary given what follows ?
    
	Ogre::StringVector groups = rsm->getResourceGroups();      
	for (Ogre::StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
    
    this->internalLoadResources();
}


