////////////////////////////////////////////////////////////
constexpr Transform::Transform() = default;


////////////////////////////////////////////////////////////
constexpr Transform::Transform(const float diag)
    : m_matrix{diag, 0.f,  0.f,  0.f,
               0.f,  diag, 0.f,  0.f,
               0.f,  0.f,  diag, 0.f,
               0.f,  0.f,  0.f,  diag}
{
}


////////////////////////////////////////////////////////////
constexpr Transform::Transform(const float* matrix)
    : m_matrix{matrix[0], matrix[1],  matrix[2],  matrix[3],
               matrix[4], matrix[5],  matrix[6],  matrix[7],
               matrix[8], matrix[9],  matrix[10],  matrix[11],
               matrix[12], matrix[13],  matrix[14],  matrix[15]}
{
}


////////////////////////////////////////////////////////////
constexpr Transform::Transform(float a00, float a01, float a02,
                               float a10, float a11, float a12,
                               float a20, float a21, float a22)
    : m_matrix{a00, a10, 0.f, a20,
               a01, a11, 0.f, a21,
               0.f, 0.f, 1.f, 0.f,
               a02, a12, 0.f, a22}
{
}


////////////////////////////////////////////////////////////
constexpr Transform::Transform(float a00, float a01, float a02, float a03,
                               float a10, float a11, float a12, float a13,
                               float a20, float a21, float a22, float a23,
                               float a30, float a31, float a32, float a33)
    : m_matrix{a00, a10, a20, a30,
               a01, a11, a21, a31,
               a02, a12, a22, a32,
               a03, a13, a23, a33}
{
}


////////////////////////////////////////////////////////////
constexpr const float* Transform::getMatrix() const
{
    return m_matrix;
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getTranslateMatrix(const geometry::Point2Df& point) const
{
    return Transform(
         1.0f, 0.0f, 0.0f, point.x,
         0.0f, 1.0f, 0.0f, point.y,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getTranslateMatrix(const geometry::Point3Df& point) const
{
    return Transform(
         1.0f, 0.0f, 0.0f, point.x,
         0.0f, 1.0f, 0.0f, point.y,
         0.0f, 0.0f, 1.0f, point.z,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getRotateXMatrix(const geometry::Angle angle) const
{
    const float cosx = std::cos(angle.asRadians());
    const float sinx = std::sin(angle.asRadians());

    return Transform(
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, cosx, -sinx, 0.0f,
         0.0f, sinx, cosx, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );;
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getRotateYMatrix(const geometry::Angle angle) const
{
    const float cosy = std::cos(angle.asRadians());
    const float siny = std::sin(angle.asRadians());

    return Transform(
         cosy, 0.0f, siny, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f,
         -siny, 0.0f, cosy, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getRotateZMatrix(const geometry::Angle angle) const
{
    const float cosz = std::cos(angle.asRadians());
    const float sinz = std::sin(angle.asRadians());

    return Transform(
         cosz, sinz, 0.0f, 0.0f,
         -sinz, cosz, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getScaleMatrix(const geometry::Point2Df& point) const
{
    return Transform(
         point.x, 0.0f, 0.0f, 0.0f,
         0.0f, point.y, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getScaleMatrix(const geometry::Point3Df& point) const
{
    return Transform(
         point.x, 0.0f, 0.0f, 0.0f,
         0.0f, point.y, 0.0f, 0.0f,
         0.0f, 0.0f, point.z, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getOriginMatrix(const geometry::Point2Df& point) const
{
    return Transform(
         1.0f, 0.0f, 0.0f, -point.x,
         0.0f, 1.0f, 0.0f, -point.y,
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr const Transform Transform::getOriginMatrix(const geometry::Point3Df& point) const
{
    return Transform(
         1.0f, 0.0f, 0.0f, -point.x,
         0.0f, 1.0f, 0.0f, -point.y,
         0.0f, 0.0f, 1.0f, -point.z,
         0.0f, 0.0f, 0.0f, 1.0f
    );
}


////////////////////////////////////////////////////////////
constexpr Transform Transform::getInverse() const
{
    const float det = getDeterminant();

    if (det != 0.f)
    {
        return Transform( 
                        (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) / det,
                         -(m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) / det,
                          (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]) / det,
                         -(m_matrix[15] * m_matrix[1] - m_matrix[3] * m_matrix[13]) / det,
                          (m_matrix[15] * m_matrix[0] - m_matrix[3] * m_matrix[12]) / det,
                         -(m_matrix[13] * m_matrix[0] - m_matrix[1] * m_matrix[12]) / det,
                          (m_matrix[7]  * m_matrix[1] - m_matrix[3] * m_matrix[5])  / det,
                         -(m_matrix[7]  * m_matrix[0] - m_matrix[3] * m_matrix[4])  / det,
                          (m_matrix[5]  * m_matrix[0] - m_matrix[1] * m_matrix[4])  / det
                        );
    }
    else
    {
        return Identity;
    }
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::combine(const Transform& transform)
{
    const float* a = m_matrix;
    const float* b = transform.m_matrix;

    // clang-format off
    *this = Transform(a[0] * b[0]  + a[4] * b[1]  + a[8] * b [2]  + a[12] * b[3],
                      a[0] * b[4]  + a[4] * b[5]  + a[8] * b [6]  + a[12] * b[7],
                      a[0] * b[8]  + a[4] * b[9]  + a[8] * b [10] + a[12] * b[11],
                      a[0] * b[12] + a[4] * b[13] + a[8] * b [14] + a[12] * b[15],
                      a[1] * b[0]  + a[5] * b[1]  + a[9] * b [2]  + a[13] * b[3],
                      a[1] * b[4]  + a[5] * b[5]  + a[9] * b [6]  + a[13] * b[7],
                      a[1] * b[8]  + a[5] * b[9]  + a[9] * b [10] + a[13] * b[11],
                      a[1] * b[12] + a[5] * b[13] + a[9] * b [14] + a[13] * b[15],
                      a[2] * b[0]  + a[6] * b[1]  + a[10] * b [2] + a[14] * b[3],
                      a[2] * b[4]  + a[6] * b[5]  + a[10] * b [6] + a[14] * b[7],
                      a[2] * b[8]  + a[6] * b[9]  + a[10] * b [10]+ a[14] * b[11],
                      a[2] * b[12] + a[6] * b[13] + a[10] * b [14]+ a[14] * b[15],
                      a[3] * b[0]  + a[7] * b[1]  + a[11] * b [2] + a[15] * b[3],
                      a[3] * b[4]  + a[7] * b[5]  + a[11] * b [6] + a[15] * b[7],
                      a[3] * b[8]  + a[7] * b[9]  + a[11] * b [10]+ a[15] * b[11],
                      a[3] * b[12] + a[7] * b[13] + a[11] * b [14]+ a[15] * b[15]);
    // clang-format on

    return *this;
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::translate(const geometry::Vector2Df& offset)
{
    return combine(getTranslateMatrix(geometry::Point2Df(offset.x, offset.y)));
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::translate(const geometry::Vector3Df& offset)
{
    return combine(getTranslateMatrix(geometry::Point3Df(offset.x, offset.y, offset.z)));

}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::scale(const geometry::Vector2Df& factors)
{
    // clang-format off
    Transform scaling(factors.x, 0,         0,
                      0,         factors.y, 0,
                      0,         0,         1);
    // clang-format on

    return combine(scaling);
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::scale(const geometry::Vector3Df& factors)
{
    // clang-format off
    Transform scaling(factors.x, 0,         0,          0,
                      0,         factors.y, 0,          0,
                      0,         0,         factors.z,  0,
                      0,         0,         0,          1.0f);
    // clang-format on

    return combine(scaling);
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::scale(const geometry::Vector2Df& factors, const geometry::Point2Df& center)
{
    // clang-format off
    Transform scaling(factors.x, 0,         center.x * (1 - factors.x),
                      0,         factors.y, center.y * (1 - factors.y),
                      0,         0,         1.0f);
    // clang-format on

    return combine(scaling);
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::scale(const geometry::Vector3Df& factors, const geometry::Point3Df& center)
{
    const float offsetX = (1.0f - factors.x) * center.x;
    const float offsetY = (1.0f - factors.y) * center.y;
    const float offsetZ = (1.0f - factors.z) * center.z;
    // clang-format off
    Transform scaling(  factors.x,  0,          0,          offsetX,
                        0,          factors.y,  0,          offsetY,
                        0,          0,          factors.z,  offsetZ,
                        0,          0,          0,          1);
    // clang-format on

    return combine(scaling);
}


////////////////////////////////////////////////////////////
constexpr geometry::Point2Df Transform::transformPoint(const geometry::Point2Df& point) const
{
    return geometry::Point2Df({m_matrix[0] * point.x + m_matrix[4] * point.y + m_matrix[12],
                               m_matrix[1] * point.x + m_matrix[5] * point.y + m_matrix[13]});
}


////////////////////////////////////////////////////////////
constexpr geometry::Point3Df Transform::transformPoint(const geometry::Point3Df& point) const
{
    geometry::Point3Df transformedPoint(
    m_matrix[0] * point.x + m_matrix[4] * point.y + m_matrix[8] * point.z + m_matrix[12],
    m_matrix[1] * point.x + m_matrix[5] * point.y + m_matrix[9] * point.z + m_matrix[13],
    m_matrix[2] * point.x + m_matrix[6] * point.y + m_matrix[10]* point.z + m_matrix[14]
    );

    const float w = m_matrix[3] * point.x + m_matrix[7] * point.y + m_matrix[11] * point.z + m_matrix[15];

    if (w != 1 and w != 0) 
    {
        transformedPoint.x /= w; 
        transformedPoint.y /= w; 
        transformedPoint.z /= w; 
    }

    return transformedPoint;
}


////////////////////////////////////////////////////////////
constexpr Transform operator*(const Transform& left, const Transform& right)
{
    return Transform(left).combine(right);
}


////////////////////////////////////////////////////////////
constexpr Transform& operator*=(Transform& left, const Transform& right)
{
    return left.combine(right);
}


////////////////////////////////////////////////////////////
constexpr geometry::Point2Df operator*(const Transform& left, const geometry::Point2Df& right)
{
    return left.transformPoint(right);
}


////////////////////////////////////////////////////////////
constexpr geometry::Point3Df operator*(const Transform& left, const geometry::Point3Df& right)
{
    return left.transformPoint(right);
}


////////////////////////////////////////////////////////////
constexpr bool operator==(const Transform& left, const Transform& right)
{
    const float* a = left.getMatrix();
    const float* b = right.getMatrix();

    // clang-format off
    return ((a[0]  == b[0])  && (a[1]  == b[1])  && (a[3]  == b[3]) &&
            (a[4]  == b[4])  && (a[5]  == b[5])  && (a[7]  == b[7]) &&
            (a[8]  == b[8])  && (a[9]  == b[9])  && (a[11]  == b[11]) &&
            (a[12] == b[12]) && (a[13] == b[13]) && (a[15] == b[15]));
    // clang-format on
}


////////////////////////////////////////////////////////////
constexpr bool operator!=(const Transform& left, const Transform& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
constexpr float Transform::getDeterminant() const
{
    const float det = m_matrix[0] * (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13])
                    - m_matrix[1] * (m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12])
                    + m_matrix[2] * (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12])
                    - m_matrix[3] * (m_matrix[13] * m_matrix[5] - m_matrix[4] * m_matrix[14]);

    return det;
}

inline constexpr Transform Transform::Identity;