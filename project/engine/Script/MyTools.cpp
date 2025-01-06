#include "MyTools.h"
#include "Matrix.h"
#include <iostream>
#include <algorithm>

using namespace std;

/// 
/// ツール関数 ここから
/// 

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

float MyTools::Lerp(const float& num1, const float& num2, const float& t)
{
	return num1 * (1 - t) + num2 * t;
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
/// AABBと球の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));

	// 距離が半径より小さければ衝突
	if (distance <= sphere.radius)
	{
		// 衝突
		return true;
	}
	return false;
}

/// AABBと直線の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Line& line)
{
	// 近い方(tが小さい)
	float tNearX = min((aabb.min.x - line.origin.x) / line.diff.x, (aabb.max.x - line.origin.x) / line.diff.x);
	float tNearY = min((aabb.min.y - line.origin.y) / line.diff.y, (aabb.max.y - line.origin.y) / line.diff.y);
	float tNearZ = min((aabb.min.z - line.origin.z) / line.diff.z, (aabb.max.z - line.origin.z) / line.diff.z);

	// 遠い方(tが大きい)
	float tFarX = max((aabb.min.x - line.origin.x) / line.diff.x, (aabb.max.x - line.origin.x) / line.diff.x);
	float tFarY = max((aabb.min.y - line.origin.y) / line.diff.y, (aabb.max.y - line.origin.y) / line.diff.y);
	float tFarZ = max((aabb.min.z - line.origin.z) / line.diff.z, (aabb.max.z - line.origin.z) / line.diff.z);

	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		// 衝突
		return true;
	}
	return false;
}

/// AABBと半直線の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Ray& ray)
{
	// 近い方(tが小さい)
	float tNearX = min((aabb.min.x - ray.origin.x) / ray.diff.x, (aabb.max.x - ray.origin.x) / ray.diff.x);
	float tNearY = min((aabb.min.y - ray.origin.y) / ray.diff.y, (aabb.max.y - ray.origin.y) / ray.diff.y);
	float tNearZ = min((aabb.min.z - ray.origin.z) / ray.diff.z, (aabb.max.z - ray.origin.z) / ray.diff.z);

	// 遠い方(tが大きい)
	float tFarX = max((aabb.min.x - ray.origin.x) / ray.diff.x, (aabb.max.x - ray.origin.x) / ray.diff.x);
	float tFarY = max((aabb.min.y - ray.origin.y) / ray.diff.y, (aabb.max.y - ray.origin.y) / ray.diff.y);
	float tFarZ = max((aabb.min.z - ray.origin.z) / ray.diff.z, (aabb.max.z - ray.origin.z) / ray.diff.z);

	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		if (tmax >= 0.0f && tmax <= 1.0f)
		{
			// 衝突
			return true;
		}
	}
	return false;
}

/// AABBと線分の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Segment& segment)
{
	// 各衝突点の媒介変数
	float tXmin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tXmax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tYmin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tYmax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tZmin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tZmax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	// 近い方(tが小さい)
	float tNearX = min(tXmin, tXmax);
	float tNearY = min(tYmin, tYmax);
	float tNearZ = min(tZmin, tZmax);

	// 遠い方(tが大きい)
	float tFarX = max(tXmin, tXmax);
	float tFarY = max(tYmin, tYmax);
	float tFarZ = max(tZmin, tZmax);

	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		if ((tmin >= 0.0f && tmin <= 1.0f) || (tmax >= 0.0f && tmax <= 1.0f) || (tmin <= 0 && tmax >= 1))
		{
			// 衝突
			return true;
		}
	}
	return false;
}

/// AABBと点の衝突判定を返す関数
bool MyTools::IsCollision(const AABB& aabb, const Vector3& point)
{
	return (point.x >= aabb.min.x && point.x <= aabb.max.x &&
			point.y >= aabb.min.y && point.y <= aabb.max.y &&
			point.z >= aabb.min.z && point.z <= aabb.max.z);
}

/// OBBと球の衝突判定を返す関数
bool MyTools::IsCollision(const OBB& obb, const Sphere& sphere)
{
	// ワールド行列
	Matrix4x4 obbWorldMatrix = {
		obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0,
		obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0,
		obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0,
		obb.center.x, obb.center.y, obb.center.z, 1
	};
	// ワールド逆行列
	Matrix4x4 obbWorldMatrixInverse = Matrix::Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = Matrix::Transform(sphere.center, obbWorldMatrixInverse);
	AABB aabbOBBLocal = { .min = { -obb.size.x, -obb.size.y, -obb.size.z }, .max = obb.size };
	Sphere sphereOBBLocal = { centerInOBBLocalSpace, sphere.radius };

	// ローカル空間で衝突判定
	return IsCollision(aabbOBBLocal, sphereOBBLocal);
}

/// OBBと直線の衝突判定を返す関数
bool MyTools::IsCollision(const OBB& obb, const Line& line)
{
	// ワールド行列
	Matrix4x4 obbWorldMatrix = {
		obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0,
		obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0,
		obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0,
		obb.center.x, obb.center.y, obb.center.z, 1
	};
	// ワールド逆行列
	Matrix4x4 obbWorldMatrixInverse = Matrix::Inverse(obbWorldMatrix);


	Vector3 centerInOBBLocalLine = Matrix::Transform(line.origin, obbWorldMatrixInverse);
	AABB aabbOBBLocal = { .min = { -obb.size.x, -obb.size.y, -obb.size.z }, .max = obb.size };
	Line lineOBBLocal = { centerInOBBLocalLine, line.diff };

	// ローカル空間で衝突判定
	return IsCollision(aabbOBBLocal, lineOBBLocal);
}

/// OBBと半直線の衝突判定を返す関数
bool MyTools::IsCollision(const OBB& obb, const Ray& ray)
{
	// ワールド行列
	Matrix4x4 obbWorldMatrix = {
		obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0,
		obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0,
		obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0,
		obb.center.x, obb.center.y, obb.center.z, 1
	};
	// ワールド逆行列
	Matrix4x4 obbWorldMatrixInverse = Matrix::Inverse(obbWorldMatrix);


	Vector3 centerInOBBLocalRay = Matrix::Transform(ray.origin, obbWorldMatrixInverse);
	AABB aabbOBBLocal = { .min = { -obb.size.x, -obb.size.y, -obb.size.z }, .max = obb.size };
	Ray rayOBBLocal = { centerInOBBLocalRay, ray.diff };

	// ローカル空間で衝突判定
	return IsCollision(aabbOBBLocal, rayOBBLocal);
}

/// OBBと線分の衝突判定を返す関数
bool MyTools::IsCollision(const OBB& obb, const Segment& segment)
{
	// ワールド行列
	Matrix4x4 obbWorldMatrix = {
		obb.orientations[0].x, obb.orientations[0].y, obb.orientations[0].z, 0,
		obb.orientations[1].x, obb.orientations[1].y, obb.orientations[1].z, 0,
		obb.orientations[2].x, obb.orientations[2].y, obb.orientations[2].z, 0,
		obb.center.x, obb.center.y, obb.center.z, 1
	};
	// ワールド逆行列
	Matrix4x4 obbWorldMatrixInverse = Matrix::Inverse(obbWorldMatrix);


	Vector3 centerInOBBLocalSegment = Matrix::Transform(segment.origin, obbWorldMatrixInverse);
	AABB aabbOBBLocal = { .min = { -obb.size.x, -obb.size.y, -obb.size.z }, .max = obb.size };
	Segment segmentOBBLocal = { centerInOBBLocalSegment, segment.diff };

	// ローカル空間で衝突判定
	return IsCollision(aabbOBBLocal, segmentOBBLocal);
}

/// OBBとOBBの衝突判定を返す関数
bool MyTools::IsCollision(const OBB& obb1, const OBB& obb2)
{
	// AABBでの各頂点座標(ローカル座標)を求める
	Vector3 obb1Vertex[8] =
	{
		{ -obb1.size.x, obb1.size.y, -obb1.size.z },	// 左上前
		{ obb1.size.x, obb1.size.y, -obb1.size.z },		// 右上前
		{ -obb1.size.x, -obb1.size.y, -obb1.size.z },	// 左下前
		{ obb1.size.x, -obb1.size.y, -obb1.size.z },	// 右下前
		{ -obb1.size.x, obb1.size.y, obb1.size.z },		// 左上奥
		{ obb1.size.x, obb1.size.y, obb1.size.z },		// 右上奥
		{ -obb1.size.x, -obb1.size.y, obb1.size.z },	// 左下奥
		{ obb1.size.x, -obb1.size.y, obb1.size.z },		// 右下奥
	};
	Vector3 obb2Vertex[8] =
	{
		{ -obb2.size.x, obb2.size.y, -obb2.size.z },	// 左上前
		{ obb2.size.x, obb2.size.y, -obb2.size.z },		// 右上前
		{ -obb2.size.x, -obb2.size.y, -obb2.size.z },	// 左下前
		{ obb2.size.x, -obb2.size.y, -obb2.size.z },	// 右下前
		{ -obb2.size.x, obb2.size.y, obb2.size.z },		// 左上奥
		{ obb2.size.x, obb2.size.y, obb2.size.z },		// 右上奥
		{ -obb2.size.x, -obb2.size.y, obb2.size.z },	// 左下奥
		{ obb2.size.x, -obb2.size.y, obb2.size.z },		// 右下奥
	};

	// ワールド行列
	Matrix4x4 obb1WorldMatrix = {
		obb1.orientations[0].x, obb1.orientations[0].y, obb1.orientations[0].z, 0,
		obb1.orientations[1].x, obb1.orientations[1].y, obb1.orientations[1].z, 0,
		obb1.orientations[2].x, obb1.orientations[2].y, obb1.orientations[2].z, 0,
		obb1.center.x, obb1.center.y, obb1.center.z, 1
	};
	Matrix4x4 obb2WorldMatrix = {
		obb2.orientations[0].x, obb2.orientations[0].y, obb2.orientations[0].z, 0,
		obb2.orientations[1].x, obb2.orientations[1].y, obb2.orientations[1].z, 0,
		obb2.orientations[2].x, obb2.orientations[2].y, obb2.orientations[2].z, 0,
		obb2.center.x, obb2.center.y, obb2.center.z, 1
	};

	// 各頂点座標(ワールド座標)
	Vector3 obb1WorldVertex[8];
	Vector3 obb2WorldVertex[8];
	for (uint32_t i = 0; i < 8; i++)
	{
		obb1WorldVertex[i] = Matrix::Transform(obb1Vertex[i], obb1WorldMatrix);
		obb2WorldVertex[i] = Matrix::Transform(obb2Vertex[i], obb2WorldMatrix);
	}

	// 分離軸
	// 面法線
	Vector3 obb1NormalX = Matrix::TransformNormal({ obb1.size.x, 0.0f, 0.0f }, obb1WorldMatrix);
	Vector3 obb1NormalY = Matrix::TransformNormal({ 0.0f, obb1.size.y, 0.0f }, obb1WorldMatrix);
	Vector3 obb1NormalZ = Matrix::TransformNormal({ 0.0f, 0.0f, obb1.size.z }, obb1WorldMatrix);
	Vector3 obb2NormalX = Matrix::TransformNormal({ obb2.size.x, 0.0f, 0.0f }, obb2WorldMatrix);
	Vector3 obb2NormalY = Matrix::TransformNormal({ 0.0f, obb2.size.y, 0.0f }, obb2WorldMatrix);
	Vector3 obb2NormalZ = Matrix::TransformNormal({ 0.0f, 0.0f, obb2.size.z }, obb2WorldMatrix);
	// 各辺の組み合わせのクロス積
	Vector3 cross1 = Cross(obb1NormalX, obb2NormalX);
	Vector3 cross2 = Cross(obb1NormalX, obb2NormalY);
	Vector3 cross3 = Cross(obb1NormalX, obb2NormalZ);
	Vector3 cross4 = Cross(obb1NormalY, obb2NormalX);
	Vector3 cross5 = Cross(obb1NormalY, obb2NormalY);
	Vector3 cross6 = Cross(obb1NormalY, obb2NormalZ);
	Vector3 cross7 = Cross(obb1NormalZ, obb2NormalX);
	Vector3 cross8 = Cross(obb1NormalZ, obb2NormalY);
	Vector3 cross9 = Cross(obb1NormalZ, obb2NormalZ);

	// 各方向ベクトルを設定する
	Vector3 axisOfSeparation[15] = {
		obb1NormalX, obb1NormalY, obb1NormalZ,
		obb2NormalX, obb2NormalY, obb2NormalZ,
		cross1, cross2, cross3,
		cross4, cross5, cross6,
		cross7, cross8, cross9
	};
	// 正規化
	Vector3 NAxisOfSeparation[15];
	for (uint32_t i = 0; i < 15; i++)
	{
		NAxisOfSeparation[i] = Normalize(axisOfSeparation[i]);
	}

	// 変数の用意
	float obb1Projection[8];			// obb1の各頂点の射影した値
	float obb2Projection[8];			// obb2の各頂点の射影した値
	float min1 = 0.0f, max1 = 0.0f, L1 = 0.0f;		// obb1 のデータ
	float min2 = 0.0f, max2 = 0.0f, L2 = 0.0f;		// obb2 のデータ
	float sumSpan = 0.0f, longSpan = 0.0f;			// 比較用

	// 分離軸の分ループ
	for (uint32_t i = 0; i < 15; i++)
	{
		// 各頂点を分離軸で射影
		for (uint32_t l = 0; l < 8; l++)
		{
			obb1Projection[l] = Dot(NAxisOfSeparation[i], obb1WorldVertex[l]);
			obb2Projection[l] = Dot(NAxisOfSeparation[i], obb2WorldVertex[l]);
		}
		// 初期値をセット
		min1 = obb1Projection[0]; max1 = obb1Projection[0];
		min2 = obb2Projection[0]; max2 = obb2Projection[0];
		// 各頂点分比較
		for (uint32_t l = 1; l < 8; l++)
		{
			min1 = (std::min)(min1, obb1Projection[l]);
			max1 = (std::max)(max1, obb1Projection[l]);
			min2 = (std::min)(min2, obb2Projection[l]);
			max2 = (std::max)(max2, obb2Projection[l]);
		}
		// obb1 と obb2 各射影の長さ
		L1 = max1 - min1;
		L2 = max2 - min2;

		// obb1とobb2の合計の長さ
		sumSpan = L1 + L2;
		// 最大値と最低値の差分
		longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		// 分離軸が引けるかどうか
		// 引ける時、未衝突
		if (sumSpan < longSpan)
		{
			return false;
		}
	}
	// 全ての分離軸が引けない時、衝突判定
	return true;
}

/// 分離軸に投影された軸成分から投影線分長を算出
float MyTools::LenSegOnSeparateAxis(const Vector3* Seg, const Vector3* e1, const Vector3* e2, const Vector3* e3)
{
	float r1 = fabsf(Dot(*Seg, *e1));
	float r2 = fabsf(Dot(*Seg, *e2));
	float r3 = e3 ? (fabsf(Dot(*Seg, *e3))) : 0;

	return r1 + r2 + r3;
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
MyBase::Vector2 MyTools::Normalize(float x, float y)
{
	float length = Length(x, y);
	if (length != 0) {
		x = x / length;
		y = y / length;
	}
	return Vector2{ x,y };
}

/// 2次元ベクトルの方向を求める関数
MyBase::Vector2 MyTools::Direction(float x, float y)
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
MyBase::Vector3 MyTools::Add(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// 3次元ベクトルの減算を返す関数
MyBase::Vector3 MyTools::Subtract(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/// 3次元ベクトルのスカラー倍を返す関数
MyBase::Vector3 MyTools::Multiply(float scalar, const Vector3& v)
{
	return Vector3{ scalar * v.x, scalar * v.y, scalar * v.z };
}

/// 3次元ベクトルの内積を返す関数
float MyTools::Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// 3次元ベクトルのクロス積(外積)を返す関数
MyBase::Vector3 MyTools::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

/// 3次元ベクトル長さ(ノルム)を返す関数
float MyTools::Length(const Vector3& v)
{
	return sqrtf(Dot(v, v));
}

/// 3次元ベクトルの正規化した値を返す関数
MyBase::Vector3 MyTools::Normalize(const Vector3& v)
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

/// 3次元ベクトルを標準化して返す関数
MyBase::Vector3 MyTools::Standardization(const Vector3& v)
{
	float mean = 0.f;					// 平均
	float standardDeviation = 1.0f;		// 標準偏差

	float meanValue = (v.x + v.y + v.z) / 3.0f;	// 平均値

	// 答え
	Vector3 ans;

	// 標準化
	ans.x = (v.x - meanValue) / standardDeviation * standardDeviation + mean;
	ans.y = (v.y - meanValue) / standardDeviation * standardDeviation + mean;
	ans.z = (v.z - meanValue) / standardDeviation * standardDeviation + mean;

	return ans;
}

/// 線形補間
MyBase::Vector3 MyTools::Lerp(const Vector3& vector1, const Vector3& vector2, float t)
{
	return Add(vector1, Multiply(t, Subtract(vector2, vector1)));
}

/// 球面線形補間
MyBase::Vector3 MyTools::Slerp(const Vector3& vector1, const Vector3& vector2, float t)
{
	// 正規化ベクトルを求める
	Vector3 start = Normalize(vector1);
	Vector3 end = Normalize(vector2);

	// 内積を求める
	float dot = Dot(start, end);
	// 誤差により1.0fを超えるのを防ぐ
	dot = Clamp(dot, dot, 1.0f);

	// s－九コサインでθの角度を求める
	float theta = std::acosf(dot);

	// θの角度からsinθを求める
	float sinTheta = std::sin(theta);

	// サイン(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	// サインθtを求める
	float sinThetaTo = std::sin(t * theta);

	Vector3 normalizeVector;
	// ゼロ除算を防ぐ
	if (sinTheta < 1.0e-5)
	{
		normalizeVector = start;
	}
	else
	{
		// 球面線形補間したベクトル(単位ベクトル)
		normalizeVector = Add(Multiply(sinThetaFrom / sinTheta, start), Multiply(sinThetaTo / sinTheta, end));
	}

	// ベクトルの長さはstartとendの長さを線形補間
	float length1 = Length(start);
	float length2 = Length(end);
	// Lerpで補間ベクトルの長さを求める
	float length = Lerp(length1, length2, t);

	// 長さを反映
	return Multiply(length, normalizeVector);
}

/// CatmullRom補間
MyBase::Vector3 MyTools::CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	const float s = 0.5f;	// 数式に出てくる 1/2 のこと。

	float t2 = t * t;	// t の2乗
	float t3 = t2 * t;	// t の3乗


	Vector3 e3 = Multiply(-1.f, p0);
	e3 = Add(e3, Multiply(3.0f, p1));
	e3 = Subtract(e3, Multiply(3.0f, p2));
	e3 = Add(e3, p3);
	/*Subtract(Add(Multiply(-1.f, p0), Multiply(3.f, p1)), Add(Multiply(3.f, p2), p3));*/

	Vector3 e2 = Multiply(2.0f, p0);
	e2 = Subtract(e2, Multiply(5.0f, p1));
	e2 = Add(e2, Multiply(4.0f, p2));
	e2 = Subtract(e2, p3);
	/*Add(Subtract(Multiply(2.f, p0), Multiply(5.f, p1)), Subtract(Multiply(4.f, p2), p3));*/

	Vector3 e1 = Multiply(-1.0f, p0);
	e1 = Add(e1, p2);
	/*Add(Multiply(-1.f, p0), p2);*/

	Vector3 e0 = Multiply(2.0f, p1);

	Vector3 ans = Multiply(t3, e3);
	ans = Add(ans, Multiply(t2, e2));
	ans = Add(ans, Multiply(t, e1));
	ans = Add(ans, e0);

	return /*Multiply(s, Add(Add(Add(Multiply(t3, e3), Multiply(t2, e2)), Multiply(t, e1)), e0))*/ Multiply(s, ans);
}

/// CatmullRomスプライン曲線上の座標を得る
MyBase::Vector3 MyTools::CatmullRomPosition(const std::vector<Vector3>& points, float t)
{
	assert(points.size() >= 4 && "制御点は4点以上必要です");

	// 区間数は制御点の数-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	// 区間内の始点を0.0f、終点を1.0fとしたときの現在位置
	float t_2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)の範囲に収める
	t_2 = Clamp(t_2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = index + 1 >= points.size() ? points.size() - 2 : index;

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 最初の区間のp0はp1を重複使用する
	if (index == 0) {
		index0 = index1;
	}

	// 最後の区間のp3はp2を重複使用する
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points.at(index0);
	const Vector3& p1 = points.at(index1);
	const Vector3& p2 = points.at(index2);
	const Vector3& p3 = points.at(index3);

	// 4点を指定してCatmul-Rom補間
	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

/// 正射影ベクトル(ベクトル射影)を返す関数
MyBase::Vector3 MyTools::Project(const Vector3& v1, const Vector3& v2)
{
	return Multiply(Dot(v1, Normalize(v2)), Normalize(v2));
}

/// 最近接点を返す関数
MyBase::Vector3 MyTools::ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 ans;

	ans = Add(segment.origin, Project(Subtract(point, segment.origin), segment.diff));

	return ans;
}

/// 垂直なベクトルを求める関数
MyBase::Vector3 MyTools::Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y, vector.x, 0.0f };
	}

	return { 0.0f, -vector.z, vector.y };
}

/// 反射ベクトルを求める関数
MyBase::Vector3 MyTools::Reflect(const Vector3& input, const Vector3& normal)
{
	Vector3 r;
	float iDotn = Dot(input, normal);

	r.x = input.x - 2 * iDotn * normal.x;
	r.y = input.y - 2 * iDotn * normal.y;
	r.z = input.z - 2 * iDotn * normal.z;

	return r;
}

/// 
/// 3次元ベクトル ここまで
/// 

///
/// 描画関数 ここから
/// 

/// 2次元ベクトルの表示
//void MyTools::VectorScreenPrintf(int x, int y, const Vector2& vector, const char* label)
//{
//	
//}

/// 3次元ベクトルの表示
//void MyTools::VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
//{
//	
//}

/// 
/// 描画関数 ここまで
/// 

