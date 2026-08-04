#pragma once

struct Mat4
{
	float m[16]; //4*4行列

	static Mat4 Indentitiy();
	static Mat4 Translation(float x, float y, float z);
	static Mat4 RotationY(float angle);
	static Mat4 RotationX(float angle);
	static Mat4 Perspective(float fovY, float aspect, float nearZ, float farZ);
	Mat4 operator*(const Mat4& other) const;
};