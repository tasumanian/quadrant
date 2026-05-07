#include "camera.h"

Mat4 Camera::GetViewMatrix() const
{
	//カメラを動かすとき、世界自体を逆方向に動かす
	return Mat4::Translation(-x, -y, -z); 
}