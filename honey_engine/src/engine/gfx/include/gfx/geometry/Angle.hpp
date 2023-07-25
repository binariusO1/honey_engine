#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
// #include <SFML/System/Export.hpp>

#include <cassert>


namespace he
{
namespace gfx
{
namespace geometry
{
////////////////////////////////////////////////////////////
/// \brief Represents an angle value.
///
////////////////////////////////////////////////////////////
class Angle
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Sets the angle value to zero.
    ///
    ////////////////////////////////////////////////////////////
    constexpr Angle();

    ////////////////////////////////////////////////////////////
    /// \brief Return the angle's value in degrees
    ///
    /// \return Angle in degrees
    ///
    /// \see asRadians
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr float asDegrees() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the angle's value in radians
    ///
    /// \return Angle in radians
    ///
    /// \see asDegrees
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr float asRadians() const;

    ////////////////////////////////////////////////////////////
    /// \brief Wrap to a range such that -180° <= angle < 180°
    ///
    /// Similar to a modulo operation, this returns a copy of the angle
    /// constrained to the range [-180°, 180°) == [-Pi, Pi).
    /// The resulting angle represents a rotation which is equivalent to *this.
    ///
    /// The name "signed" originates from the similarity to signed integers:
    /// <table>
    /// <tr>
    ///   <th></th>
    ///   <th>signed</th>
    ///   <th>unsigned</th>
    /// </tr>
    /// <tr>
    ///   <td>char</td>
    ///   <td>[-128, 128)</td>
    ///   <td>[0, 256)</td>
    /// </tr>
    /// <tr>
    ///   <td>Angle</td>
    ///   <td>[-180°, 180°)</td>
    ///   <td>[0°, 360°)</td>
    /// </tr>
    /// </table>
    ///
    /// \return Signed angle, wrapped to [-180°, 180°)
    ///
    /// \see wrapUnsigned
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr Angle wrapSigned() const;

    ////////////////////////////////////////////////////////////
    /// \brief Wrap to a range such that 0° <= angle < 360°
    ///
    /// Similar to a modulo operation, this returns a copy of the angle
    /// constrained to the range [0°, 360°) == [0, Tau) == [0, 2*Pi).
    /// The resulting angle represents a rotation which is equivalent to *this.
    ///
    /// The name "unsigned" originates from the similarity to unsigned integers:
    /// <table>
    /// <tr>
    ///   <th></th>
    ///   <th>signed</th>
    ///   <th>unsigned</th>
    /// </tr>
    /// <tr>
    ///   <td>char</td>
    ///   <td>[-128, 128)</td>
    ///   <td>[0, 256)</td>
    /// </tr>
    /// <tr>
    ///   <td>Angle</td>
    ///   <td>[-180°, 180°)</td>
    ///   <td>[0°, 360°)</td>
    /// </tr>
    /// </table>
    ///
    /// \return Unsigned angle, wrapped to [0°, 360°)
    ///
    /// \see wrapSigned
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] constexpr Angle wrapUnsigned() const;

    ////////////////////////////////////////////////////////////
    // Static member data
    ////////////////////////////////////////////////////////////
    // NOLINTNEXTLINE(readability-identifier-naming)
    static const Angle Zero; //!< Predefined 0 degree angle value

private:
    friend constexpr Angle degrees(float angle);
    friend constexpr Angle radians(float angle);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a number of degrees
    ///
    /// This function is internal. To construct angle values,
    /// use sf::radians or sf::degrees instead.
    ///
    /// \param degrees Angle in degrees
    ///
    ////////////////////////////////////////////////////////////
public:
    constexpr explicit Angle(float degrees);

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    float m_degrees{0}; //!< Angle value stored as degrees
};

////////////////////////////////////////////////////////////
/// \brief Construct an angle value from a number of degrees
///
/// \param angle Number of degrees
///
/// \return Angle value constructed from the number of degrees
///
/// \see radians
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle degrees(float angle);

////////////////////////////////////////////////////////////
/// \brief Construct an angle value from a number of radians
///
/// \param angle Number of radians
///
/// \return Angle value constructed from the number of radians
///
/// \see degrees
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle radians(float angle);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of == operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if both angle values are equal
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator==(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of != operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if both angle values are different
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator!=(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of < operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is less than \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator<(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of > operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is greater than \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator>(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of <= operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is less than or equal to \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator<=(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of >= operator to compare two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return True if \a left is greater than or equal to \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr bool operator>=(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of unary - operator to negate an angle value.
///
/// Represents a rotation in the opposite direction.
///
/// \param right Right operand (an angle)
///
/// \return Negative of the angle value
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator-(Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary + operator to add two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Sum of the two angle values
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator+(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary += operator to add/assign two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Sum of the two angle values
///
////////////////////////////////////////////////////////////
constexpr Angle& operator+=(Angle& left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary - operator to subtract two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Difference of the two angle values
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator-(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary -= operator to subtract/assign two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return Difference of the two angle values
///
////////////////////////////////////////////////////////////
constexpr Angle& operator-=(Angle& left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary * operator to scale an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator*(Angle left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary * operator to scale an angle value
///
/// \param left  Left operand (a number)
/// \param right Right operand (an angle)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator*(float left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary *= operator to scale/assign an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left multiplied by \a right
///
////////////////////////////////////////////////////////////
constexpr Angle& operator*=(Angle& left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary / operator to scale an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator/(Angle left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary /= operator to scale/assign an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (a number)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
constexpr Angle& operator/=(Angle& left, float right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary / operator to compute the ratio of two angle values
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return \a left divided by \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr float operator/(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary % operator to compute modulo of an angle value.
///
/// Right hand angle must be greater than zero.
///
/// Examples:
/// \code
/// sf::degrees(90) % sf::degrees(40)  // 10 degrees
/// sf::degrees(-90) % sf::degrees(40) // 30 degrees (not -10)
/// \endcode
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return \a left modulo \a right
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator%(Angle left, Angle right);

////////////////////////////////////////////////////////////
/// \relates Angle
/// \brief Overload of binary %= operator to compute/assign remainder of an angle value
///
/// \param left  Left operand (an angle)
/// \param right Right operand (an angle)
///
/// \return \a left modulo \a right
///
////////////////////////////////////////////////////////////
constexpr Angle& operator%=(Angle& left, Angle right);

namespace Literals
{

////////////////////////////////////////////////////////////
/// \relates sf::Angle
/// \brief User defined literal for angles in degrees, e.g. 10.5_deg
///
/// \param angle Angle in degrees
///
/// \return \a Angle
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator"" _deg(long double angle);

////////////////////////////////////////////////////////////
/// \relates sf::Angle
/// \brief User defined literal for angles in degrees, e.g. 90_deg
///
/// \param angle Angle in degrees
///
/// \return \a Angle
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator"" _deg(unsigned long long int angle);

////////////////////////////////////////////////////////////
/// \relates sf::Angle
/// \brief User defined literal for angles in radians, e.g. 0.1_rad
///
/// \param angle Angle in radians
///
/// \return \a Angle
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator"" _rad(long double angle);

////////////////////////////////////////////////////////////
/// \relates sf::Angle
/// \brief User defined literal for angles in radians, e.g. 2_rad
///
/// \param angle Angle in radians
///
/// \return \a Angle
///
////////////////////////////////////////////////////////////
[[nodiscard]] constexpr Angle operator"" _rad(unsigned long long int angle);

} // namespace Literals

#include "gfx/geometry/Angle.inl"

} // namespace geometry
} // namespace gfx
} // namespace he