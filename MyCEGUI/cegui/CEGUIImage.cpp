#include "CEGUIImage.h"
#include "CEGUIExceptions.h"
#include "CEGUITexture.h"
#include "CEGUIImageset.h"
#include "CEGUIRenderer.h"
#include "CEGUIPropertyHelper.h"
#include <cmath>
#include <iostream>


namespace CEGUI
{

/*************************************************************************
	Constructor
*************************************************************************/
Image::Image(const Imageset* owner, const String& name, const Rect& area, const Point& render_offset, float horzScaling, float vertScaling) :
	d_owner(owner),
	d_area(area),
	d_offset(render_offset),
	d_name(name)
{
	if (!d_owner)
	{
		CEGUI_THROW(NullObjectException("Image::Image - Imageset pointer passed to Image constructor must be valid."));
	}

	// setup initial image scaling
	setHorzScaling(horzScaling);
	setVertScaling(vertScaling);

	// TODO: if we ever store texture co-ordinates, they should be calculated here.
}

/*************************************************************************
	Copy constructor
*************************************************************************/
Image::Image(const Image& image) :
	d_owner(image.d_owner),
	d_area(image.d_area),
	d_offset(image.d_offset),
	d_scaledWidth(image.d_scaledWidth),
	d_scaledHeight(image.d_scaledHeight),
	d_scaledOffset(image.d_scaledOffset),
	d_name(image.d_name)
{
}

/*************************************************************************
	destructor
*************************************************************************/
Image::~Image(void)
{
}


/*************************************************************************
	set the horizontal scaling factor to be applied to this Image
*************************************************************************/
void Image::setHorzScaling(float factor)
{
	d_scaledWidth		= PixelAligned(d_area.getWidth() * factor);
	d_scaledOffset.d_x	= PixelAligned(d_offset.d_x * factor);
}


/*************************************************************************
	set the vertical scaling factor to be applied to this Image
*************************************************************************/
void Image::setVertScaling(float factor)
{
	d_scaledHeight		= PixelAligned(d_area.getHeight() * factor);
	d_scaledOffset.d_y	= PixelAligned(d_offset.d_y * factor);
}


/*************************************************************************
    Clip and then queue the image to be rendered.
*************************************************************************/
void Image::draw(GeometryBuffer& buffer, const Rect& dest_rect,
    const Rect* clip_rect, const ColourRect& colours,
    QuadSplitMode quad_split_mode) const
{
    Rect dest(dest_rect);

    // apply rendering offset to the destination Rect
    dest.offset(d_scaledOffset);

    // draw
    d_owner->draw(buffer, d_area, dest, clip_rect, colours, quad_split_mode);
}


/*************************************************************************
	String object containing the name of this Image	
*************************************************************************/
const String& Image::getName(void) const
{
	return d_name;
}


/*************************************************************************
	Return the name of the Imageset that contains this Image	
*************************************************************************/
const String& Image::getImagesetName(void) const
{
	return d_owner->getName();
}

/*************************************************************************
	Return Rect describing the source texture area used by this Image.
*************************************************************************/
const Rect& Image::getSourceTextureArea(void) const
{
    return d_area;
}


/*************************************************************************
    Output XML <Image ... > element for this image
*************************************************************************/
void Image::writeXMLToStream(XMLSerializer& xml_stream) const
{
    xml_stream.openTag("Image")
        .attribute("Name", d_name)
        .attribute("XPos", PropertyHelper::uintToString(static_cast<uint>(d_area.d_left)))
        .attribute("YPos", PropertyHelper::uintToString(static_cast<uint>(d_area.d_top)))
        .attribute("Width", PropertyHelper::uintToString(static_cast<uint>(d_area.getWidth())))
        .attribute("Height", PropertyHelper::uintToString(static_cast<uint>(d_area.getHeight())));

    if (d_offset.d_x != 0.0f)
        xml_stream.attribute("XOffset", PropertyHelper::intToString(static_cast<int>(d_offset.d_x)));

    if (d_offset.d_y != 0.0f)
        xml_stream.attribute("YOffset", PropertyHelper::intToString(static_cast<int>(d_offset.d_x)));

    xml_stream.closeTag();
}


}