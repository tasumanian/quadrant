#include "transform.h"

Mat4 Transform::ToMatrix() const
{
    Mat4 translation = Mat4::Translation(x, y, z);
    Mat4 rotation = Mat4::RotationY(rotationY);

    return translation * rotation;
}