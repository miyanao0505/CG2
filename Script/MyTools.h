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
	/// マウスカーソルの座標を取得する関数
	/// </summary>
	/// <param name="kWindowWidth">ウィンドウの横幅</param>
	/// <param name="kWindowHeight">ウィンドウの縦幅</param>
	/// <param name="viewMatrix">ビュー行列</param>
	/// <param name="projectionMatrix">プロジェクション行列</param>
	/// <returns></returns>
	//static Vector3 GetMousePosition(const float& kWindowWidth, const float& kWindowHeight, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);
	
	/// <summary>
	/// 範囲内の値を返す関数
	/// </summary>
	/// <param name="num">確認する値</param>
	/// <param name="min">最低値</param>
	/// <param name="max">最大値</param>
	/// <returns>範囲内の値</returns>
	static float Clamp(const float& num, const float& min, const float& max);

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