#include "Core/mat4.h"
#include <cmath>

Mat4 Mat4::Indentitiy() //単位行列の生成
{
	Mat4 result = {}; //0初期化

	result.m[0] = 1.0f;
	result.m[5] = 1.0f;
	result.m[10] = 1.0f;
	result.m[15] = 1.0f;
	
	return result;
}

Mat4 Mat4::Translation(float x, float y, float z)
{
	Mat4 result = Indentitiy();

	//GLに渡す前提の行列
	result.m[12] = x;
	result.m[13] = y;
	result.m[14] = z;

	return result;
}
Mat4 Mat4::RotationY(float angle) //Y軸回転行列の生成
{
	Mat4 result = Indentitiy();

	float c = std::cos(angle);
	float s = std::sin(angle);

	result.m[0] = c;
	result.m[2] = s;

	result.m[8] = -s;
	result.m[10] = c;

	return result;
}
Mat4 Mat4::RotationX(float angle)
{
	Mat4 result = Indentitiy();

	float c = std::cos(angle);
	float s = std::sin(angle);

	result.m[5] = c;
	result.m[6] = -s;

	result.m[9] = s;
	result.m[10] = c;

	return result;
}
Mat4 Mat4::Perspective(float fovY, float aspect, float nearZ, float farZ)
{
	Mat4 result = {};

	//視野角の設定
	float f = 1.0f / std::tan(fovY * 0.5f);

	//横長補正
	result.m[0] = f / aspect;
	
	result.m[5] = f;
	result.m[10] = (farZ + nearZ) / (nearZ - farZ);
	result.m[11] = -1.0f;
	result.m[14] = (2.0f * farZ * nearZ) / (nearZ - farZ);

	return result;
}
Mat4 Mat4::operator*(const Mat4& other) const
{
	Mat4 result = {};

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			for (int i = 0; i < 4; i++)
			{
				result.m[row * 4 + col] +=
					m[row * 4 + i] *
					other.m[i * 4 + col];
			}
		}
	}

	return result;
}