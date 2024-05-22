#pragma once
#include "MyTools.h"
#include "Matrix.h"

class Draw :
	public MyBase
{
public:

	/// 
	/// デバッグ用 ここから
	/// 

	/// <summary>
	/// グリッドの表示
	/// </summary>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	static void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	/// 
	/// デバッグ用 ここまで
	/// 

	/// 
	/// オブジェクト用 ここから
	/// 
	
	/// <summary>
	/// 直線の描画
	/// </summary>
	/// <param name="line">直線の情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawLine(const Line& line, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// 半直線の描画
	/// </summary>
	/// <param name="ray">半直線の情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawRay(const Ray& ray, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// 線分の描画
	/// </summary>
	/// <param name="segment">線分の情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// 球の描画
	/// </summary>
	/// <param name="sphere">球の情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// 平面の描画
	/// </summary>
	/// <param name="plane">平面の情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// 三角形の描画
	/// </summary>
	/// <param name="triangle">三角形の情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// <summary>
	/// AABBの描画
	/// </summary>
	/// <param name="aabb">aabbの情報</param>
	/// <param name="viewProjectionMatrix">ビュープロジェクション行列</param>
	/// <param name="viewportMatrix">ビューポート行列</param>
	/// <param name="color">色</param>
	static void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	/// 
	/// オブジェクト用 ここから
	/// 

};

