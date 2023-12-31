#include "gfx/draw/IShape.hpp"

namespace he
{
namespace gfx
{
namespace draw
{
////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
IShape<POINT, VECTOR, VERTEX>::IShape(const std::string& name) : m_context(name)
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
IShape<POINT, VECTOR, VERTEX>::IShape(const IShape<POINT, VECTOR, VERTEX>& copy)
    : m_vertexArray{copy.m_vertexArray}
    , m_context{copy.m_context} // TODO: warning - during copy, unique string will be not unique. maybe name should be: hash+name ?
    , m_vertexArrayNeedUpdate{copy.m_vertexArrayNeedUpdate}
    , TransformableTmpl(copy)
{
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
IShape<POINT, VECTOR, VERTEX>::~IShape() = default;


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void IShape<POINT, VECTOR, VERTEX>::setColor(const Color& color)
{
    if (color != m_context.color)
    {
        m_context.color = color;
    
        for (std::size_t i = 0; i < m_vertexArray.size(); ++i)
        {
            m_vertexArray[i].color = color;
        }
    }
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const Color IShape<POINT, VECTOR, VERTEX>::getColor() const
{
    return m_context.color;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool IShape<POINT, VECTOR, VERTEX>::setPosition(const POINT& position)
{
    m_vertexArrayNeedUpdate = TransformableTmpl::setPosition(position);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool IShape<POINT, VECTOR, VERTEX>::setOrigin(const POINT& point)
{
    m_vertexArrayNeedUpdate = TransformableTmpl::setOrigin(point);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool IShape<POINT, VECTOR, VERTEX>::setRotation(const float rotationZ)
{
    m_vertexArrayNeedUpdate = TransformableTmpl::setRotation(rotationZ);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
bool IShape<POINT, VECTOR, VERTEX>::setRotations(const float rotationX, const float rotationY, const float rotationZ, geometry::transform::AxisOrder axisOrder)
{
    m_vertexArrayNeedUpdate = TransformableTmpl::setRotations(rotationX, rotationY, rotationZ, axisOrder);
    return m_vertexArrayNeedUpdate;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const POINT& IShape<POINT, VECTOR, VERTEX>::getPosition() const
{
    return TransformableTmpl::getPosition();
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const std::string IShape<POINT, VECTOR, VERTEX>::getName() const
{
    return m_context.name;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void IShape<POINT, VECTOR, VERTEX>::setLayerName(const std::string& name)
{
    m_context.layerName = name;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const std::string IShape<POINT, VECTOR, VERTEX>::getLayerName() const
{
    return m_context.layerName;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void IShape<POINT, VECTOR, VERTEX>::setName(const std::string& name)
{
    m_context.name = name;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX> // TODO : do usunięcia lub do protected, ponieważ korzystam tylko z m_vertexArray (poza tym nie trzeba dawać na zewnątrz) 
const VERTEX& IShape<POINT, VECTOR, VERTEX>::getVertexArray() const
{
    return m_vertexArray;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const POINT& IShape<POINT, VECTOR, VERTEX>::getOrigin() const
{
    return TransformableTmpl::getOrigin();
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const geometry::Angle& IShape<POINT, VECTOR, VERTEX>::getRotation() const
{
    return TransformableTmpl::getRotation();
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
void IShape<POINT, VECTOR, VERTEX>::setOriginPosition(const OriginPosition& originPosition)
{
    switch (originPosition)
    {
    case OriginPosition::any:
        break;
    case OriginPosition::leftDown:
        setOrigin({0.0, 0.0});
        break;
    case OriginPosition::center:
        setOriginInCenter();
        break;
    default:
        break;
    }

    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
template<>
void IShape2dFor3d::setOriginPosition(const OriginPosition& originPosition)
{
    switch (originPosition)
    {
    case OriginPosition::any:
        break;
    case OriginPosition::leftDown:
        setOrigin({0.0, 0.0, 0.0});
        break;
    case OriginPosition::center:
        setOriginInCenter();
        break;
    default:
        break;
    }

    m_vertexArrayNeedUpdate = true;
}


////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
OriginPosition IShape<POINT, VECTOR, VERTEX>::getOriginPosition() const
{
    return m_context.originPosition;
}



////////////////////////////////////////////////////////////
template<typename POINT, typename VECTOR, typename VERTEX>
const he::gfx::geometry::transform::Transform& IShape<POINT, VECTOR, VERTEX>::getTransform() const
{
    return TransformableTmpl::getTransform();
}

     
template class IShape<geometry::Point2Df, geometry::Vector2Df, VertexArray2d>;
template class IShape<geometry::Point3Df, geometry::Vector3Df, VertexArray3d>;

} // namespace draw
} // namespace gfx
} // namespace he