#include "MyTools.h"
#include "Matrix.h"
#include <iostream>
#include "Novice.h"

using namespace std;

/// 
/// ツール関数 ここから
/// 

/// マウスカーソルの座標を取得する関数
Vector3 MyTools::GetMousePosition(const float& kWindowWidth, const float& kWindowHeight, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix)
{
	int x = 0, y = 0;
	Novice::GetMousePosition(&x, &y);

	// クリップ座標に変換
	float clipX = (2.0f * float(x)) / kWindowWidth - 1.0f;
	float clipY = 1.0f - (2.0f * float(y)) / kWindowHeight;
	float clipZ = 0.0f; // スクリーン座標からは深度情報が得られないので適当な値を指定します

	// クリップ座標をカメラ座標に逆変換
	Vector3 eyeCoords = Matrix::Transform(Vector3(clipX, clipY, clipZ), Matrix::Inverse(projectionMatrix));

	// カメラ座標をワールド座標に逆変換
	Vector3 worldCoords = Matrix::Transform(eyeCoords, Matrix::Inverse(viewMatrix));

	return worldCoords;
}

/// 範囲内の値を返す関数
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

/// 三角形の存在する平面情報を求める関数
MyTools::Plane MyTools::TriangleToPlane(const Triangle& triangle)
{
	// ベクトルv1,v2を求める
	Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);

	// 法線nを算出
	Vector3 n = Normalize(Cross(v1, v2));

	// 距離を求める
	float d = Dot(triangle.vertices[0], n);

	return Plane{ n, d };
}

/// 球と球の衝突判定を返す関数
bool MyTools::IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	// 2つの球の中心点間の距離を求める
	float distance = Length(Subtract(sphere2.center, sphere1.center));

	// 半径の合計よりも短ければ衝突
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}

	return false;
}

/// 球と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 平面と球の中心点との距離
	float k = Dot(plane.normal, sphere.center) - plane.distance;
	// 絶対値
	if (k < 0) { k *= -1.f; };

	if (k <= sphere.radius)
	{
		return true;
	}

	return false;
}

/// 直線と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Line& line, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=並行であるので、衝突しているはずがない
	if (dot == 0)
	{
		return false;
	}

	// tを求める
	//float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	// tの値と線の種類によって衝突しているかを判断する
	return true;
}

/// 半直線と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Ray& ray, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);

	// 垂直=並行であるので、衝突しているはずがない
	if (dot == 0)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;

	// tの値と線の種類によって衝突しているかを判断する
	if (t > 0.f)
	{
		return true;
	}
	return false;
}

/// 線分と平面の衝突判定を返す関数
bool MyTools::IsCollision(const Segment& segment, const Plane& plane)
{
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直=並行であるので、衝突しているはずがない
	if (dot == 0)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tの値と線の種類によって衝突しているかを判断する
	if (t >= 0.f && t <= 1.f)
	{
		return true;
	}
	return false;
}

/// 三角形と直線の衝突判定を返す関数
bool MyTools::IsCollision(const Triangle& triangle, const Line& line)
{
	// 三角形の存在する平面を求める
	Plane plane = TriangleToPlane(triangle);

	// 線と平面との衝突判定を行う
	if (IsCollision(line, plane))
	{
		// 衝突点を求める
		float dot = Dot(plane.normal, line.diff);
		float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
		Vector3 p = Add(line.origin, Multiply(t, line.diff));

		// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));

		// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f)
		{
			return true;
		}
		return false;
	}
	return false;
}

/// 三角形と半直線の衝突判定を返す関数
bool MyTools::IsCollision(const Triangle& triangle, const Ray& ray)
{
	// 三角形の存在する平面を求める
	Plane plane = TriangleToPlane(triangle);

	// 線と平面との衝突判定を行う
	if (IsCollision(ray, plane))
	{
		// 衝突点を求める
		float dot = Dot(plane.normal, ray.diff);
		float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;
		Vector3 p = Add(ray.origin, Multiply(t, ray.diff));

		// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));

		// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f)
		{
			return true;
		}
		return false;
	}
	return false;
}

/// 三角形と線分の衝突判定を返す関数
bool MyTools::IsCollision(const Triangle& triangle, const Segment& segment)
{
	// 三角形の存在する平面を求める
	Plane plane = TriangleToPlane(triangle);

	// 線と平面との衝突判定を行う
	if (IsCollision(segment, plane))
	{
		// 衝突点を求める
		float dot = Dot(plane.normal, segment.diff);
		float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
		Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

		// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
		Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]), Subtract(p, triangle.vertices[1]));
		Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), Subtract(p, triangle.vertices[2]));
		Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]), Subtract(p, triangle.vertices[0]));

		// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f)
		{
			return true;
		}
		return false;
	}
	return false;
}

/// AABB同士の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && 
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && 
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true;
	}
	return false;
}

/// 
/// ツール関数 ここまで
/// 

///
/// 2次元ベクトル ここから
///

/// 2次元ベクトルの内積を返す関数
float MyTools::Dot(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

/// 2次元ベクトルのクロス積(外積)を返す関数
float MyTools::Cross(const Vector2& v1, const Vector2& v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

/// 2次元ベクトルの長さ(ノルム)を返す関数
float MyTools::Length(float x, float y)
{
	return sqrtf(x * x + y * y);
}

/// 2次元ベクトルの正規化した値を返す関数
Vector2 MyTools::Normalize(float x, float y)
{
	float length = Length(x, y);
	if (length != 0) {
		x = x / length;
		y = y / length;
	}
	return Vector2{ x,y };
}

/// 2次元ベクトルの方向を求める関数
Vector2 MyTools::Direction(float x, float y)
{
	return Normalize(x, y);
}

///
/// 2次元ベクトル ここまで
/// 

///
/// 3次元ベクトル ここから
/// 

/// 3次元ベクトルの加算を返す関数
Vector3 MyTools::Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// 3次元ベクトルの減算を返す関数
Vector3 MyTools::Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/// 3次元ベクトルのスカラー倍を返す関数
Vector3 MyTools::Multiply(float scalar, const Vector3& v)
{
	return Vector3{ scalar * v.x, scalar * v.y, scalar * v.z };
}

/// 3次元ベクトルの内積を返す関数
float MyTools::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// 3次元ベクトルのクロス積(外積)を返す関数
Vector3 MyTools::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

/// 3次元ベクトル長さ(ノルム)を返す関数
float MyTools::Length(const Vector3& v)
{
	return sqrtf(Dot(v, v));
}

/// 3次元ベクトルの正規化した値を返す関数
Vector3 MyTools::Normalize(const Vector3& v)
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

/// 正射影ベクトル(ベクトル射影)を返す関数
Vector3 MyTools::Project(const Vector3& v1, const Vector3& v2)
{
	return Multiply(Dot(v1, Normalize(v2)), Normalize(v2));
}

/// 最近接点を返す関数
Vector3 MyTools::ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 ans;

	ans = Add(segment.origin, Project(Subtract(point, segment.origin), segment.diff));

	return ans;
}

/// 垂直なベクトルを求める関数
Vector3 MyTools::Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y, vector.x, 0.0f };
	}

	return { 0.0f, -vector.z, vector.y };
}

/// 
/// 3次元ベクトル ここまで
/// 

///
/// 描画関数 ここから
/// 

/// 2次元ベクトルの表示
void MyTools::VectorScreenPrintf(int x, int y, const Vector2& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%s", label);
}

/// 3次元ベクトルの表示
void MyTools::VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

/// 
/// 描画関数 ここまで
/// 

