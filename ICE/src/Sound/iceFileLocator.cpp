#include <Sound\iceFileLocator.h>
#include <OgreException.h>

iceFileLocator::iceFileLocator(){}
iceFileLocator::~iceFileLocator(){}

Ogre::Archive* iceFileLocator::Find(Ogre::String &filename){

	Ogre::ResourceGroupManager::ResourceGroup* grp = Ogre::ResourceGroupManager::getResourceGroup("General");
    if (!grp)
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Cannot locate a resource group called 'General'", "ResourceGroupManager::openResource");

	OGRE_LOCK_MUTEX(grp->OGRE_AUTO_MUTEX_NAME) // lock group mutex
	ResourceLocationIndex::iterator rit = grp->resourceIndexCaseSensitive.find(filename);
	if (rit != grp->resourceIndexCaseSensitive.end())
	{
		// Found in the index
		Ogre::Archive *fileArchive = rit->second;
		filename = fileArchive->getName() + "/" + filename;
		return fileArchive;
	}
    return NULL;
}
