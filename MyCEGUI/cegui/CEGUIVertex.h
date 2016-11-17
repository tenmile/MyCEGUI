#include "CEGUIVector.h"
#include "CEGUIcolour.h"

namespace CEGUI
{

//3D�ռ�Ķ���ṹ
struct Vertex
{
    //! Position of the vertex in 3D space.
    Vector3 position;
    //! Texture co-ords to be applied to the vertex.
    Vector2 tex_coords;
    //! colour to be applied to the vertex.
    colour  colour_val;
};

}