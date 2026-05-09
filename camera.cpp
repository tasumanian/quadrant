#include "camera.h"

Mat4 Camera::GetViewMatrix() const
{
	//カメラを動かすとき、世界自体を逆方向に動かす
    Mat4 translation =
        Mat4::Translation(-x, -y, -z);

    Mat4 yawRotation =
        Mat4::RotationY(-yaw);
    Mat4 pitchRotation =
        Mat4::RotationX(-pitch);

    //順番大事
    return pitchRotation *
            yawRotation * translation;
}