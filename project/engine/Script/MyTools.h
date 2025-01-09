#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "MyBase.h"

class MyTools :
	public MyBase
{
public:

	// メンバ変数
	static const int kColumnWidth = 60;			// 表示文字サイズ_横幅
	static const int kRowHeight = 20;			// 表示文字サイズ_縦幅

	// メンバ関数

	/// 
	/// ツール関数 ここから
	/// 
	
	/// <summary>
	/// 範囲内の値を返す関数
	/// </summary>
	/// <param name="num">確認する値</param>
	/// <param name="min">最低値</param>
	/// <param name="max">最大値</param>
	/// <returns>範囲内の値</returns>
	static float Clamp(const float& num, const float& min, const float& max);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="num1">数値1</param>
	/// <param name="num2">数値2</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	static float Lerp(const float& num1, const float& num2, const float& t);

	/// <summary>
	/// 三角形の存在する平面情報を求める関数
	/// </summary>
	/// <param name="triangle"></param>
	/// <returns></returns>
	static Plane TriangleToPlane(const Triangle& triangle);

	/// <summary>
	/// 球と球の衝突判定を返す関数
	/// </summary>
	/// <param name="sphere1">球A</param>
	/// <param name="sphere2">球B</param>
	/// <returns></returns>
	static bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

	/// <summary>
	/// 球と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	static bool IsCollision(const Sphere& sphere, const Plane& plane);

	/// <summary>
	/// 直線と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="line">直線</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	static bool IsCollision(const Line& line, const Plane& plane);

	/// <summary>
	/// 半直線と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="ray">半直線</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	static bool IsCollision(const Ray& ray, const Plane& plane);

	/// <summary>
	/// 線分と平面の衝突判定を返す関数
	/// </summary>
	/// <param name="segment">線分</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	static bool IsCollision(const Segment& segment, const Plane& plane);

	/// <summary>
	/// 三角形と直線の衝突判定を返す関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="line">直線</param>
	/// <returns></returns>
	static bool IsCollision(const Triangle& triangle, const Line& line);

	/// <summary>
	/// 三角形と半直線の衝突判定を返す関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="ray">半直線</param>
	/// <returns></returns>
	static bool IsCollision(const Triangle& triangle, const Ray& ray);

	/// <summary>
	/// 三角形と線分の衝突判定を返す関数
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	static bool IsCollision(const Triangle& triangle, const Segment& segment);

	/// <summary>
	/// AABB同士の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb1">aabbA</param>
	/// <param name="aabb2">aabbB</param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	/// <summary>
	/// AABBと球の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="sphere">球</param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);

	/// <summary>
	/// AABBと直線の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="line">直線</param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Line& line);

	/// <summary>
	/// AABBと半直線の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="ray">半直線</param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Ray& ray);

	/// <summary>
	/// AABBと線分の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Segment& segment);

	/// <summary>
	/// AABBと点の衝突判定を返す関数
	/// </summary>
	/// <param name="aabb">aabb</param>
	/// <param name="point">点</param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Vector3& point);

	/// <summary>
	/// OBBと球の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="sphere">球</param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb, const Sphere& sphere);

	/// <summary>
	/// OBBと直線の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="line">直線</param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb, const Line& line);

	/// <summary>
	/// OBBと半直線の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="ray">半直線</param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb, const Ray& ray);

	/// <summary>
	/// OBBと線分の衝突判定を返す関数
	/// </summary>
	/// <param name="obb">OBB</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb, const Segment& segment);

	/// <summary>
	/// OBBとOBBの衝突判定を返す関数
	/// </summary>
	/// <param name="obb1">OBB1</param>
	/// <param name="obb2">OBB2</param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb1, const OBB& obb2);

	/// <summary>
	/// 分離軸に投影された軸成分から投影線分長を算出
	/// </summary>
	/// <param name="Seg"></param>
	/// <param name="e1"></param>
	/// <param name="e2"></param>
	/// <param name="e3"></param>
	/// <returns></returns>
	static float LenSegOnSeparateAxis(const Vector3* Seg, const Vector3* e1, const Vector3* e2, const Vector3* e3);

	/// 
	/// ツール関数 ここまで
	///

	/// 
	/// 2次元ベクトル ここから
	///
	
	/// <summary>
	/// 2次元ベクトルの内積を返す関数
	/// </suumary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static float Dot(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 2次元ベクトルのクロス積(外積)を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static float Cross(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 2次元ベクトルの長さ(ノルム)を返す関数
	/// </summary>
	/// <param name="x">ベクトルのX軸の値</param>
	/// <param name="y">ベクトルのY軸の値</param>
	static float Length(float x, float y);

	/// <summary>
	/// 2次元ベクトルの正規化した値を返す関数
	/// </summary>
	/// <param name="x">ベクトルのX軸の値</param>
	/// <param name="y">ベクトルのY軸の値</param>
	static Vector2 Normalize(float x, float y);

	/// <summary>
	/// 2次元ベクトルの方向を求める関数
	/// </summary>
	/// <param name="x">ベクトルのX軸の値</param>
	/// <param name="y">ベクトルのY軸の値</param>
	static Vector2 Direction(float x, float y);

	/// 
	/// 2次元ベクトル ここまで
	///

	/// 
	/// 3次元ベクトル ここから
	/// 
	
	/// <summary>
	/// 3次元ベクトルの加算を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 3次元ベクトルの減算を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 3次元ベクトルのスカラー倍
	/// </summary>
	/// <param name="scalar">スカラー値</param>
	/// <param name="v">ベクトル</param>
	static Vector3 Multiply(float scalar, const Vector3& v);

	/// <summary>
	/// 3次元ベクトルの内積を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 3次元ベクトルのクロス積(外積)を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 3次元ベクトル長さ(ノルム)を返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	static float Length(const Vector3& v);

	/// <summary>
	/// 3次元ベクトルの正規化した値を返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	static Vector3 Normalize(const Vector3& v);

	/// <summary>
	/// 3次元ベクトルを標準化して返す関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	static Vector3 Standardization(const Vector3& v);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="vector1">ベクトルA</param>
	/// <param name="vector2">ベクトルB</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	static Vector3 Lerp(const Vector3& vector1, const Vector3& vector2, float t);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="vector1">ベクトルA</param>
	/// <param name="vector2">ベクトルB</param>
	/// <param name="t">媒介変数</param>
	/// <returns></returns>
	static Vector3 Slerp(const Vector3& vector1, const Vector3& vector2, float t);

	/// <summary>
	/// CatmullRom補間
	/// </summary>
	/// <param name="p0">点0の座標</param>
	/// <param name="p1">点1の座標</param>
	/// <param name="p2">点2の座標</param>
	/// <param name="p3">点3の座標</param>
	/// <param name="t">点1を0.0f、点2を1.0fとした割合指定</param>
	/// <returns>点1と点2の間で指定された座標</returns>
	static Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	/// <summary>
	/// CatmullRomスプライン曲線上の座標を得る
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="t">スプラインの全区間の中での割合指定[0, 1]</param>
	/// <returns>座標</returns>
	static Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

	/// <summary>
	/// 正射影ベクトル(ベクトル射影)を返す関数
	/// </summary>
	/// <param name="v1">ベクトルA</param>
	/// <param name="v2">ベクトルB</param>
	static Vector3 Project(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 最近接点を返す関数
	/// </summary>
	/// <param name="point">始点</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	/// <summary>
	/// 垂直なベクトルを求める関数
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	static Vector3 Perpendicular(const Vector3& vector);

	/// <summary>
	/// 反射ベクトルを求める関数
	/// </summary>
	/// <param name="input">入射ベクトル</param>
	/// <param name="normal">法線</param>
	/// <returns></returns>
	static Vector3 Reflect(const Vector3& input, const Vector3& normal);

	/// 
	/// 3次元ベクトル ここまで
	/// 

	/// 
	/// 描画 ここから
	/// 
	
	/// <summary>
	/// 2次元ベクトルの表示
	/// </summary>
	/// <param name="x">表示するデバッグ用文字列の左上座標 X</param>
	/// <param name="y">表示するデバッグ用文字列の左上座標 Y</param>
	/// <param name="vector">表示したいベクトルの変数</param>
	/// <param name="label">表示したいベクトルの名前</param>
	//static void VectorScreenPrintf(int x, int y, const Vector2& vector, const char* label);

	/// <summary>
	/// 3次元ベクトルの表示
	/// </summary>
	/// <param name="x">表示するデバッグ用文字列の左上座標 X</param>
	/// <param name="y">表示するデバッグ用文字列の左上座標 Y</param>
	/// <param name="vector">表示したいベクトルの変数</param>
	/// <param name="label">表示したいベクトルの名前</param>
	//static void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

	/// 
	/// 描画 ここまで
	/// 
};