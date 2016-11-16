#include "CEGUIImagesetManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"


namespace CEGUI
{
//----------------------------------------------------------------------------//
// singleton instance pointer
template<> ImagesetManager* Singleton<ImagesetManager>::ms_Singleton = 0;

//----------------------------------------------------------------------------//
ImagesetManager::ImagesetManager() :
NamedXMLResourceManager<Imageset, Imageset_xmlHandler>("Imageset")
{
	char addr_buff[32];
	sprintf(addr_buff, "(%p)", static_cast<void*>(this));
	Logger::getSingleton().logEvent(
		"CEGUI::ImagesetManager singleton created " + String(addr_buff));
}

//----------------------------------------------------------------------------//
ImagesetManager::~ImagesetManager()
{
	Logger::getSingleton().logEvent(
		"---- Begining cleanup of Imageset system ----");

	destroyAll();

	char addr_buff[32];
	sprintf(addr_buff, "(%p)", static_cast<void*>(this));
	Logger::getSingleton().logEvent(
		"CEGUI::ImagesetManager singleton destroyed " + String(addr_buff));
}

//----------------------------------------------------------------------------//
Imageset& ImagesetManager::create(const String& name, Texture& texture,
	XMLResourceExistsAction action)
{
	Logger::getSingleton().logEvent("Attempting to create Imageset '" + name +
		"' with texture only.");

	// create new object ahead of time
	Imageset* object = new Imageset(name, texture);
	// return appropriate object instance (deleting any not required)
	return doExistingObjectAction(name, object, action);;
}

//----------------------------------------------------------------------------//
Imageset& ImagesetManager::createFromImageFile(const String& name,
	const String& filename,
	const String& resourceGroup,
	XMLResourceExistsAction action)
{
	Logger::getSingleton().logEvent("Attempting to create Imageset '" + name +
		"' using image file '" + filename + "'.");

	// create new object ahead of time
	Imageset* object = new Imageset(name, filename, resourceGroup);
	// return appropriate object instance (deleting any not required)
	return doExistingObjectAction(name, object, action);;
}

//----------------------------------------------------------------------------//
void ImagesetManager::notifyDisplaySizeChanged(const Size& size)
{
	// notify all attached Imageset objects of the change in resolution
	ObjectRegistry::iterator pos = d_objects.begin(), end = d_objects.end();

	for (; pos != end; ++pos)
		pos->second->notifyDisplaySizeChanged(size);
}

//----------------------------------------------------------------------------//
ImagesetManager::ImagesetIterator ImagesetManager::getIterator(void) const
{
	return ImagesetIterator(d_objects.begin(), d_objects.end());
}

//----------------------------------------------------------------------------//
void ImagesetManager::writeImagesetToStream(const String& imageset,
	OutStream& out_stream) const
{
	// Create an XMLSerializer which make use of 4 space and UTF-8 encoding
	XMLSerializer xml(out_stream);
	get(imageset).writeXMLToStream(xml);
}

//----------------------------------------------------------------------------//

}