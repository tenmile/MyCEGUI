#pragma once

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIColourRect.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImage.h"
#include "CEGUIIteratorBase.h"
#include "CEGUIXMLSerializer.h" 

#include <map>

namespace CEGUI
{

/*!
\brief
	Offers functions to define, access, and draw, a set of image components on a single graphical surface or Texture.

	Imageset objects are a means by which a single graphical image (file, Texture, etc), can be split into a number
	of 'components' which can later be accessed via name.  The components of an Imageset can queried for
	various details, and sent to the Renderer object for drawing.
*/
class Imageset
{
	typedef	std::map<String, Image, String::FastLessCompare>	ImageRegistry;

public:
	/*!
	\brief
		Construct a new Imageset object.  Object will initially have no Images defined

	\param texture
		Texture object that holds the imagery for the Imageset being created.
	*/
	Imageset(const String& name, Texture& texture);

	/*!
	\brief
        Construct a new Imageset using the specified image file and imageset name.  The created
        imageset will, by default, have a single Image defined named "full_image" which represents
        the entire area of the loaded image file.

    \note
        Under certain renderers it may be required that the source image dimensions be some
        power of 2, if this condition is not met then stretching and other undesired side-effects
        may be experienced.  To be safe from such effects it is generally recommended that all
        images that you load have dimensions that are some power of 2.

    \param name
        String object holding the name to be assigned to the created imageset.

    \param filename
        String object holding the filename of the image that is to be loaded.  The image should be
        of some format that is supported by the Renderer that is in use.

    \param resourceGroup
        Resource group identifier to be passed to the resource manager, which may specify a group
        from which the image file is to be loaded.

    \exception FileIOException thrown if something goes wrong while loading the image.
    */
    Imageset(const String& name, const String& filename, const String& resourceGroup);
	//不能被继承
	~Imageset(void);


	typedef	ConstBaseIterator<ImageRegistry>	ImageIterator;	//!< Iterator type for this collection

	/*************************************************************************
		Public interface
	*************************************************************************/
	Texture*	getTexture(void) const						{return d_texture;}
	const String&	getName(void) const						{return d_name;}
	uint      getImageCount(void) const               {return (uint)d_images.size();}
	bool		isImageDefined(const String& name) const	{return d_images.find(name) != d_images.end();}
	//返回副本
	//exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	const Image&	getImage(const String& name) const;
	void	undefineImage(const String& name);
	void	undefineAllImages(void);
	//exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	Size	getImageSize(const String& name) const			{return getImage(name).getSize();}
	//exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	float	getImageWidth(const String& name) const			{return getImage(name).getWidth();}
	//exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	float	getImageHeight(const String& name) const		{return getImage(name).getHeight();}


	/*!
	\brief
		return the rendering offsets applied to the named image.

	\param name
		String object holding the name of the Image.

	\return
		Point object that holds the rendering offsets for the requested Image.

	\exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	*/
	Point	getImageOffset(const String& name) const		{return getImage(name).getOffsets();}


	/*!
	\brief
		return the x rendering offset for the named image.

	\param name
		String object holding the name of the Image.

	\return
		float value equal to the x rendering offset applied when drawing the requested Image.

	\exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	*/
	float	getImageOffsetX(const String& name) const		{return getImage(name).getOffsetX();}


	/*!
	\brief
		return the y rendering offset for the named image.

	\param name
		String object holding the name of the Image.

	\return
		float value equal to the y rendering offset applied when drawing the requested Image.

	\exception UnknownObjectException	thrown if no Image named \a name is defined for the Imageset
	*/
	float	getImageOffsetY(const String& name) const		{return getImage(name).getOffsetY();}

	
	/*!
	\brief
		Define a new Image for this Imageset

	\param name
		String object holding the name that will be assigned to the new Image, which must be unique within the Imageset.

	\param position
		Point object describing the pixel location of the Image on the image file / texture associated with this Imageset.

	\param size
		Size object describing the dimensions of the Image, in pixels.

	\param render_offset
		Point object describing the offsets, in pixels, that are to be applied to the Image when it is drawn.

	\return
		Nothing

	\exception AlreadyExistsException	thrown if an Image named \a name is already defined for this Imageset
	*/
	void	defineImage(const String& name, const Point& position, const Size& size, const Point& render_offset)
	{
		defineImage(name, Rect(position.d_x, position.d_y, position.d_x + size.d_width, position.d_y + size.d_height), render_offset);
	}


	/*!
	\brief
		Define a new Image for this Imageset

	\param name
		String object holding the name that will be assigned to the new Image, which must be unique within the Imageset.

	\param image_rect
		Rect object describing the area on the image file / texture associated with this Imageset that will be used for the Image.

	\param render_offset
		Point object describing the offsets, in pixels, that are to be applied to the Image when it is drawn.

	\return
		Nothing

	\exception AlreadyExistsException	thrown if an Image named \a name is already defined for this Imageset
	*/
	void	defineImage(const String& name, const Rect& image_rect, const Point& render_offset);


    /*!
    \brief
        Queues an area of the associated Texture the be drawn on the screen.
        Low-level routine to be used carefully!

    \param buffer
        GeometryBuffer object where the geometry for the area to be drawn will
        be queued.

    \param source_rect
        Rect object describing the area of the image file / texture that is to
        be queued for drawing

    \param dest_rect
        Rect describing the area of the screen that will be filled with the
        imagery from \a source_rect.

    \param clip_rect
        Rect object describing a 'clipping rectangle' that will be applied when
        drawing the requested imagery

    \param colours
        ColourRect object holding the ARGB colours to be applied to the four
        corners of the rendered imagery.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Rect& source_rect,
              const Rect& dest_rect, const Rect* clip_rect,
              const ColourRect& colours, QuadSplitMode quad_split_mode) const;

    /*!
    \brief
        Queues an area of the associated Texture the be drawn on the screen.
        Low-level routine to be used carefully!

    \param buffer
        GeometryBuffer object where the geometry for the area to be drawn will
        be queued.

    \param source_rect
        Rect object describing the area of the image file / texture that is to
        be queued for drawing.

    \param dest_rect
        Rect describing the area of the screen that will be filled with the
        imagery from \a source_rect.

    \param clip_rect
        Rect object describing a 'clipping rectangle' that will be applied when
        drawing the requested imagery.

    \param top_left_colour
        colour to be applied to the top left corner of the rendered imagery.

    \param top_right_colour
        colour to be applied to the top right corner of the rendered imagery.

    \param bottom_left_colour
        colour to be applied to the bottom left corner of the rendered imagery.

    \param bottom_right_colour
        colour to be applied to the bottom right corner of the rendered imagery.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Rect& source_rect,
              const Rect& dest_rect, const Rect* clip_rect,
              const colour& top_left_colour = 0xFFFFFFFF,
              const colour& top_right_colour = 0xFFFFFFFF,
              const colour& bottom_left_colour = 0xFFFFFFFF,
              const colour& bottom_right_colour = 0xFFFFFFFF,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const
    {
        draw(buffer, source_rect, dest_rect, clip_rect,
             ColourRect(top_left_colour, top_right_colour,
                        bottom_left_colour, bottom_right_colour),
             quad_split_mode);
    }

	bool	isAutoScaled(void) const		{return d_autoScale;}


	/*!
	\brief
		Return the native display size for this Imageset.  This is only relevant if the Imageset is being auto-scaled.

	\return
		Size object describing the native display size for this Imageset.
	*/
	Size	getNativeResolution(void) const	{return Size(d_nativeHorzRes, d_nativeVertRes);}

	void	setAutoScalingEnabled(bool setting);


	/*!
	\brief
		Set the native resolution for this Imageset

	\param size
		Size object describing the new native screen resolution for this Imageset.

	\return
		Nothing
	*/
	void	setNativeResolution(const Size& size);


    /*!
    \brief
        Notify the Imageset that the display size may have changed.

    \param size
        Size object describing the display resolution
    */
    void notifyDisplaySizeChanged(const Size& size);

	ImageIterator	getIterator(void) const;


    /*!
    \brief
        Writes an xml representation of this Imageset to \a out_stream.

    \param out_stream
        Stream where xml data should be output.

    \param indentLevel
        Current XML indentation level

    \return
        Nothing.
    */
    void writeXMLToStream(XMLSerializer& xml_stream) const;
	//static
    static void setDefaultResourceGroup(const String& resourceGroup)
        { d_defaultResourceGroup = resourceGroup; }
	//static
    static const String& getDefaultResourceGroup()
        { return d_defaultResourceGroup; }

protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Unloads all loaded data and leaves the Imageset in a clean (but un-usable) state.  This should be called for cleanup purposes only.
	*/
	void	unload(void);


	/*!
	\brief
		set the Texture object to be used by this Imageset.  Changing textures on an Imageset that is in use is not a good idea!

	\param texture
		Texture object to be used by the Imageset.  The old texture is NOT disposed of, that is the clients responsibility.

	\return
		Nothing

	\exception	NullObjectException		thrown if \a texture is NULL
	*/
	void	setTexture(Texture* texture);


	/*!
	\brief
		Sets the scaling factor for all Images that are a part of this Imageset.

	\return
		Nothing.
	*/
	void	updateImageScalingFactors(void);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	String			d_name;						//!< Holds the name of this imageset.
	ImageRegistry	d_images;					//!< Registry of Image objects for the images defined for this Imageset
	Texture*		d_texture;					//!< Texture object that handles imagery for this Imageset
    String          d_textureFilename;          //!< String holding the name of the texture filename (if any).

	// auto-scaling fields
	bool	d_autoScale;			//!< true when auto-scaling is enabled.
	float	d_horzScaling;			//!< current horizontal scaling factor.
	float	d_vertScaling;			//!< current vertical scaling factor.
	float	d_nativeHorzRes;		//!< native horizontal resolution for this Imageset.
	float	d_nativeVertRes;		//!< native vertical resolution for this Imageset.
    //static 所有的Imageset只有一个
	static String d_defaultResourceGroup;   //!< Default resource group specifically for Imagesets.
};

}