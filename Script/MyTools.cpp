#include "MyTools.h"
//#include "Novice.h"

using namespace std;

/// 
/// ツール関数 ここから
/// 

/// <summary>
/// 範囲内の値を返す関数
/// </summary>
float MyTools::Clamp(const float& num, const float& min, const float& max)
{
	if (num < min)
	{
		return min;
	}
	else if (num > max)
	{
		return max;
	}
	else
	{
		return num;
	}
}

/// 
/// ツール関数 ここまで
/// 

///
/// 2次元ベクトル ここから
///

/// <summary>
/// 2次元ベクトル内積
/// </suumary>
float MyTools::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

/// <summary>
/// 2次元ベクトルクロス積(外積)
/// </summary>
float MyTools::Cross(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

/// <summary>
/// 2次元ベクトル長さ(ノルム)関数
/// </summary>
float MyTools::Length(float x, float y)
{
	return sqrtf(x * x + y * y);
}

/// <summary>
/// 2次元ベクトル正規化関数
/// </summary>
MyTools::Vector2 MyTools::Normalize(float x, float y)
{
	float length = Length(x, y);
	if (length != 0) {
		x = x / length;
		y = y / length;
	}
	return Vector2{ x,y };
}

/// <summary>
/// 2次元ベクトル方向を求める関数
/// </summary>
MyTools::Vector2 MyTools::Direction(float x, float y)
{
	return Normalize(x, y);
}

///
/// 2次元ベクトル ここまで
/// 

///
/// 3次元ベクトル ここから
/// 

/// <summary>
/// 3次元ベクトル加算
/// </summary>
MyTools::Vector3 MyTools::Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// <summary>
/// 3次元ベクトル減算
/// </summary>
MyTools::Vector3 MyTools::Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/// <summary>
/// 3次元ベクトルスカラー倍
/// </summary>
MyTools::Vector3 MyTools::Multiply(float scalar, const Vector3& v)
{
	return Vector3{ scalar * v.x, scalar * v.y, scalar * v.z };
}

/// <summary>
/// 3次元ベクトル内積
/// </summary>
float MyTools::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// <summary>
/// 3次元ベクトルクロス積(外積)
/// </summary>
MyTools::Vector3 MyTools::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

/// <summary>
/// 3次元ベクトル長さ(ノルム)
/// </summary>
float MyTools::Length(const Vector3& v)
{
	return sqrtf(Dot(v, v));
}

/// <summary>
/// 3次元ベクトル正規化
/// </summary>
MyTools::Vector3 MyTools::Normalize(const Vector3& v)
{
	float length = Length(v);
	float x = v.x, y = v.y, z = v.z;
	if (length != 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}

	return Vector3{ x, y, z };
}

/// 
/// 3次元ベクトル ここまで
/// 

///
/// 描画関数 ここから
/// 

/// <summary>
/// 2次元ベクトルの表示
/// </summary>
//void MyTools::VectorScreenPrintf(int x, int y, const Vector2& vector, const char* label)
//{
//	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
//	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
//	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%s", label);
//}

/// <summary>
/// 3次元ベクトルの表示
/// </summary>
//void MyTools::VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
//{
//	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
//	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
//	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
//	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
//}

/// 
/// 描画関数 ここまで
/// 

