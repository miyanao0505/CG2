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
	static float Dot(const Vector2& v1, const Vector2& v2);
	/// クロス積(外積)
	static float Cross(const Vector2& v1, const Vector2& v2);
	/// 長さ
	static float Length(float x, float y);
	/// 正規化
	static Vector2 Normalize(float x, float y);
	/// 方向を求める
	static Vector2 Direction(float x, float y);

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	/// 加算
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	/// 減算
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	/// スカラー倍
	static Vector3 Multiply(float scalar, const Vector3& v);
	/// 内積
	static float Dot(const Vector3& v1, const Vector3& v2);
	/// クロス積(外積)
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	/// 長さ(ノルム)
	static float Length(const Vector3& v);
	/// 正規化
	static Vector3 Normalize(const Vector3& v);

	/// <summary>
	/// 描画
	/// </summary>
	/// 2次元ベクトルの表示
	static void VectorScreenPrintf(int x, int y, const Vector2& vector, const char* label);
	/// 3次元ベクトルの表示
	static void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

};