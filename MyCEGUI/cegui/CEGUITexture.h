#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Abstract base class specifying the required interface for Texture objects.

    Texture objects are created via the Renderer.  The actual inner workings of
    any Texture object are dependant upon the Renderer (and underlying API) in
    use.  This base class defines the minimal set of functions that is required
    for the rest of the system to work.  Texture objects are only created
    through the Renderer object's texture creation functions.
*/
class Texture
{
public:
    /*!
    \brief
        Enumerated type containing the supported pixel formats that can be
        passed to loadFromMemory
    */
    enum PixelFormat
    {
        //! Each pixel is 3 bytes. RGB in that order.
        PF_RGB,
        //! Each pixel is 4 bytes. RGBA in that order.
        PF_RGBA
    };

    /*!
    \brief
        Returns the current pixel size of the texture.

    \return
        Reference to a Size object that describes the size of the texture in
        pixels.
    */
    virtual const Size& getSize() const = 0;

    /*!
    \brief
        Returns the original pixel size of the data loaded into the texture.

    \return
        reference to a Size object that describes the original size, in pixels,
        of the data loaded into the texture.
    */
    virtual const Size& getOriginalDataSize() const = 0;

    /*!
    \brief
        Returns pixel to texel scale values that should be used for converting
        pixel values to texture co-ords.

    \return
        Reference to a Vector2 object that describes the scaling values required
        to accurately map pixel positions to texture co-ordinates.
    */
    virtual const Vector2& getTexelScaling() const = 0;

    /*!
    \brief
        Loads the specified image file into the texture.  The texture is resized
        as required to hold the image.

    \param filename
        The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when
        loading the image file.

    \return
        Nothing.
    */
    virtual void loadFromFile(const String& filename,
                              const String& resourceGroup) = 0;

    /*!
    \brief
        Loads (copies) an image in memory into the texture.  The texture is
        resized as required to hold the image.

    \param buffer
        Pointer to the buffer containing the image data.

    \param buffer_size
        Size of the buffer (in pixels as specified by \a pixelFormat)

    \param pixel_format
        PixelFormat value describing the format contained in \a buffPtr.

    \return
        Nothing.
    */
    virtual void loadFromMemory(const void* buffer,
                                const Size& buffer_size,
                                PixelFormat pixel_format) = 0;

    /*!
    \brief
        Save / dump the content of the texture to a memory buffer.  The dumped
        pixel format is always RGBA (4 bytes per pixel).

    \param buffer
        Pointer to the buffer that is to receive the image data.  You must make
        sure that this buffer is large enough to hold the dumped texture data,
        the required pixel dimensions can be established by calling getSize.
    */
    virtual void saveToMemory(void* buffer) = 0;

    /*!
    \brief
        Destructor for Texture base class.
    */
    virtual ~Texture() {}
};

}