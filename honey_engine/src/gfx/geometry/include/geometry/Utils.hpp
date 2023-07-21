#pragma once

namespace geometry
{
class Utils
{
public:
    static void perspective(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
};

} // namespace geometry