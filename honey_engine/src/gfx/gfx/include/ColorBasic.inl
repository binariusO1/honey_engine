
////////////////////////////////////////////////////////////
constexpr ColorBasic::ColorBasic() = default;


////////////////////////////////////////////////////////////
constexpr ColorBasic::ColorBasic(std::uint8_t red, std::uint8_t green, std::uint8_t blue) :
r(red),
g(green),
b(blue)
{
}


////////////////////////////////////////////////////////////
constexpr ColorBasic::ColorBasic(std::uint32_t color) :
r(static_cast<std::uint8_t>((color & 0xff000000) >> 24)),
g(static_cast<std::uint8_t>((color & 0x00ff0000) >> 16)),
b(static_cast<std::uint8_t>((color & 0x0000ff00) >> 8))
{
}


////////////////////////////////////////////////////////////
constexpr std::uint32_t ColorBasic::toInteger() const
{
    return static_cast<std::uint32_t>((r << 24) | (g << 16) | (b << 8));
}


////////////////////////////////////////////////////////////
constexpr bool operator==(const ColorBasic& left, const ColorBasic& right)
{
    return (left.r == right.r) && (left.g == right.g) && (left.b == right.b);
}


////////////////////////////////////////////////////////////
constexpr bool operator!=(const ColorBasic& left, const ColorBasic& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
constexpr ColorBasic operator+(const ColorBasic& left, const ColorBasic& right)
{
    const auto clampedAdd = [](std::uint8_t lhs, std::uint8_t rhs) -> std::uint8_t
    {
        const int intResult = static_cast<int>(lhs) + static_cast<int>(rhs);
        return static_cast<std::uint8_t>(intResult < 255 ? intResult : 255);
    };

    return ColorBasic(clampedAdd(left.r, right.r),
                 clampedAdd(left.g, right.g),
                 clampedAdd(left.b, right.b));
}


////////////////////////////////////////////////////////////
constexpr ColorBasic operator-(const ColorBasic& left, const ColorBasic& right)
{
    const auto clampedSub = [](std::uint8_t lhs, std::uint8_t rhs) -> std::uint8_t
    {
        const int intResult = static_cast<int>(lhs) - static_cast<int>(rhs);
        return static_cast<std::uint8_t>(intResult > 0 ? intResult : 0);
    };

    return ColorBasic(clampedSub(left.r, right.r),
                 clampedSub(left.g, right.g),
                 clampedSub(left.b, right.b));
}


////////////////////////////////////////////////////////////
constexpr ColorBasic operator*(const ColorBasic& left, const ColorBasic& right)
{
    const auto scaledMul = [](std::uint8_t lhs, std::uint8_t rhs) -> std::uint8_t
    {
        const auto uint16Result = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(lhs) * static_cast<std::uint16_t>(rhs));
        return static_cast<std::uint8_t>(uint16Result / 255u);
    };

    return ColorBasic(scaledMul(left.r, right.r),
                 scaledMul(left.g, right.g),
                 scaledMul(left.b, right.b));
}


////////////////////////////////////////////////////////////
constexpr ColorBasic& operator+=(ColorBasic& left, const ColorBasic& right)
{
    return left = left + right;
}


////////////////////////////////////////////////////////////
constexpr ColorBasic& operator-=(ColorBasic& left, const ColorBasic& right)
{
    return left = left - right;
}


////////////////////////////////////////////////////////////
constexpr ColorBasic& operator*=(ColorBasic& left, const ColorBasic& right)
{
    return left = left * right;
}
