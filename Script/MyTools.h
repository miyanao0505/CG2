#pragma once
#include <cmath>
#include "MyBase.h"

class MyTools :
	public MyBase
{
public:

	// メンバ変数
	static const int kColumnWidth = 60;			// 表示文字サイズ_横幅

	// メンバ関数

	/// <summary>
	/// ツール関数
	/// </summary>
	/// 範囲内
	static float Clamp(const float& num, const float& min, const float& max);

	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	/// 内積
	static float Dot(const Vec2& v1, const Vec2& v2);
	/// クロス積(外積)
	static float Cross(const Vec2& v1, const Vec2& v2);
	/// 長さ
	static float Length(float x, float y);
	/// 正規化
	static Vec2 Normalize(float x, float y);
	/// 方向を求める
	static Vec2 Direction(float x, float y);

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	/// 加算
	static Vec3 Add(const Vec3& v1, const Vec3& v2);
	/// 減算
	static Vec3 Subtract(const Vec3& v1, const Vec3& v2);
	/// スカラー倍
	static Vec3 Multiply(float scalar, const Vec3& v);
	/// 内積
	static float Dot(const Vec3& v1, const Vec3& v2);
	/// クロス積(外積)
	static Vec3 Cross(const Vec3& v1, const Vec3& v2);
	/// 長さ(ノルム)
	static float Length(const Vec3& v);
	/// 正規化
	static Vec3 Normalize(const Vec3& v);

	/// <summary>
	/// 描画
	/// </summary>
	/// 2次元ベクトルの表示
	static void VectorScreenPrintf(int x, int y, const Vec2& vector, const char* label);
	/// 3次元ベクトルの表示
	static void VectorScreenPrintf(int x, int y, const Vec3& vector, const char* label);

};