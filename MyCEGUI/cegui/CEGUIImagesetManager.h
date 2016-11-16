#pragma once

#include "CEGUIBase.h"
#include "CEGUISingleton.h"
#include "CEGUINamedXMLResourceManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImageset_xmlHandler.h"
#include "CEGUIIteratorBase.h"


namespace CEGUI
{
/*!
\brief
    Class providing a shared library of Imageset objects to the system.

    The ImagesetManager is used to create, access, and destroy Imageset objects.
    The idea is that the ImagesetManager will function as a central repository
    for imagery used within the GUI system, and that such imagery can be
    accessed, via a unique name, by any interested party within the system.
*/
class ImagesetManager :
        public Singleton<ImagesetManager>,
        public NamedXMLResourceManager<Imageset, Imageset_xmlHandler>
{
public:
    //! Constructor for ImagesetManager objects
    ImagesetManager();

    //! Destructor for ImagesetManager objects
    ~ImagesetManager();

    /*!
    \brief
        Create a Imageset object with the given name and Texture

        The created Imageset will be of limited use, and will require one or
        more images to be defined for the set.

    \param name
        String object containing the unique name for the Imageset being created.

    \param texture
        Texture object to be associated with the Imageset

    \param action
        One of the XMLResourceExistsAction enumerated values indicating what
        action should be taken when an Imageset with the specified name
        already exists.

    \return
        Reference to the newly created Imageset object

    \exception AlreadyExistsException
        thrown if an Imageset named \a name is already present in the system.
    */
    Imageset& create(const String& name, Texture& texture,
                     XMLResourceExistsAction action = XREA_RETURN);

    /*!
    \brief
        Create an Imageset object from the specified image file.  The Imageset
        will initially have a single image defined named "full_image" which is
        an image that represents the entire area of the loaded image.

    \param name
        String object containing the unique name for the Imageset being created.

    \param filename
        String object holding the name of the image file to be loaded.

    \param resourceGroup
        Resource group identifier to be passed to the resource manager when
        loading the image file.

    \param action
        One of the XMLResourceExistsAction enumerated values indicating what
        action should be taken when an Imageset with the specified name
        already exists.

    \return
        Reference to the newly created Imageset object

    \exception AlreadyExistsException
        thrown if an Imageset named \a name is already present in the system.

    \exception FileIOException
        thrown if something goes wrong while reading the image file \a filename.
    */
    Imageset& createFromImageFile(const String& name, const String& filename,
                                const String& resourceGroup = "",
                                XMLResourceExistsAction action = XREA_RETURN);

    /*!
    \brief
        Notify the ImagesetManager that the display size may have changed.

    \param size
        Size object describing the display resolution
    */
    void notifyDisplaySizeChanged(const Size& size);

    /*!
    \brief
        Writes a full XML imageset for the specified Imageset to the given
        OutStream.

    \param imageset
        String holding the name of the Imageset to be written to the stream.

    \param out_stream
        OutStream (std::ostream based) object where data is to be sent.

    \return
        Nothing.
    */
    void writeImagesetToStream(const String& imageset,
                               OutStream& out_stream) const;

    //! ImagesetIterator type definition.
    typedef ConstBaseIterator<ObjectRegistry> ImagesetIterator;

    /*!
    \brief
        Return a ImagesetManager::ImagesetIterator object to iterate over the
        available Imageset objects.
    */
    ImagesetIterator getIterator() const;

    // ensure we see overloads from template base class
    using NamedXMLResourceManager<Imageset, Imageset_xmlHandler>::create;
};

}