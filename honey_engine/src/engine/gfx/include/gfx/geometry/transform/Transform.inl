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
constexpr Transform Transform::getInverse() const
{
    // clang-format off
    // Compute the determinant
    float det = m_matrix[0] * (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) -
                m_matrix[1] * (m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) +
                m_matrix[3] * (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]);
    // clang-format on

    // Compute the inverse if the determinant is not zero
    // (don't use an epsilon because the determinant may *really* be tiny)
    if (det != 0.f)
    {
        // clang-format off
        return Transform( (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) / det,
                         -(m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) / det,
                          (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]) / det,
                         -(m_matrix[15] * m_matrix[1] - m_matrix[3] * m_matrix[13]) / det,
                          (m_matrix[15] * m_matrix[0] - m_matrix[3] * m_matrix[12]) / det,
                         -(m_matrix[13] * m_matrix[0] - m_matrix[1] * m_matrix[12]) / det,
                          (m_matrix[7]  * m_matrix[1] - m_matrix[3] * m_matrix[5])  / det,
                         -(m_matrix[7]  * m_matrix[0] - m_matrix[3] * m_matrix[4])  / det,
                          (m_matrix[5]  * m_matrix[0] - m_matrix[1] * m_matrix[4])  / det);
        // clang-format on
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
    *this = Transform(a[0] * b[0]  + a[4] * b[1]  + a[12] * b[3],
                      a[0] * b[4]  + a[4] * b[5]  + a[12] * b[7],
                      a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
                      a[1] * b[0]  + a[5] * b[1]  + a[13] * b[3],
                      a[1] * b[4]  + a[5] * b[5]  + a[13] * b[7],
                      a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
                      a[3] * b[0]  + a[7] * b[1]  + a[15] * b[3],
                      a[3] * b[4]  + a[7] * b[5]  + a[15] * b[7],
                      a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);
    // clang-format on

    return *this;
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::translate(const geometry::Vector2Df& offset)
{
    // clang-format off
    Transform translation(1, 0, offset.x,
                          0, 1, offset.y,
                          0, 0, 1);
    // clang-format on

    return combine(translation);
}


////////////////////////////////////////////////////////////
constexpr Transform& Transform::translate(const geometry::Vector3Df& offset)
{
    // clang-format off
    Transform translation(1, 0, 0, offset.x,
                          0, 1, 0, offset.y,
                          0, 0, 1, offset.z,
                          0, 0, 0, 1);
    // clang-format on

    return combine(translation);
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
constexpr Transform& Transform::scale(const geometry::Vector2Df& factors, const geometry::Point2Df& center)
{
    // clang-format off
    Transform scaling(factors.x, 0,         center.x * (1 - factors.x),
                      0,         factors.y, center.y * (1 - factors.y),
                      0,         0,         1);
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
constexpr bool operator==(const Transform& left, const Transform& right)
{
    const float* a = left.getMatrix();
    const float* b = right.getMatrix();

    // clang-format off
    return ((a[0]  == b[0])  && (a[1]  == b[1])  && (a[3]  == b[3]) &&
            (a[4]  == b[4])  && (a[5]  == b[5])  && (a[7]  == b[7]) &&
            (a[12] == b[12]) && (a[13] == b[13]) && (a[15] == b[15]));
    // clang-format on
}


////////////////////////////////////////////////////////////
constexpr bool operator!=(const Transform& left, const Transform& right)
{
    return !(left == right);
}

inline constexpr Transform Transform::Identity;