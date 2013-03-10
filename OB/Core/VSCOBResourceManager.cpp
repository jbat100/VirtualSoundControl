
#include "VSCOBResourceManager.h"

VSC::OB::ResourceManager::ResourceManager(std::string resourceFilePath, std::string resourceRootPath) :
mResourceFilePath(resourceFilePath),
mResourceRootPath(resourceRootPath)
{

}

void VSC::OB::ResourceManager::setupResources(void)
{
    Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
    
    if (mTrace) std::cout << "VSC::OB::ResourceManager::setupResources with path " << mResourceFilePath << std::endl;
    
    // Load resource paths from config file
    
    Ogre::ConfigFile cf;
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
            archName = mResourceRootPath + i->second;
            try
            {
                rsm->addResourceLocation(archName, typeName, secName);
            }
            catch (Ogre::Exception& e)
            {
                std::cout << "Exception : " << e.getFullDescription() << std::endl;
            }
        }
    }
    
    this->internalSetupResources();
}


void VSC::OB::ResourceManager::loadResources(void)
{
    /*
     *  It looks like we need a window to be created before we can do this
     */
    
    Ogre::TextureManager* textureManager = Ogre::TextureManager::getSingletonPtr();
    if (mTrace) std::cout << "VSC::OB::ResourceManager::setupResources, texture manager is " << textureManager << std::endl;
    textureManager->setDefaultNumMipmaps(5);
    
    Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
    
    try
    {
        rsm->initialiseAllResourceGroups(); // is this necessary given what follows ?
    }
    catch (std::exception& e)
    {
        std::cerr << "VSC::OB::ResourceManager::loadResources " << e.what() << std::endl;
    }
    
    
	Ogre::StringVector groups = rsm->getResourceGroups();      
	for (Ogre::StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
    
    this->internalLoadResources();
}


