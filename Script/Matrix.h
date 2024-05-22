#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>
#include "MyBase.h"

class Matrix :
	public MyBase
{
public:
	/// 定数
	static const int kRowHeight = 20;		// 縦
	static const int kColumnWidth = 60;		// 横

	/// 関数
	/// <summary>
	/// 2x2の行列
	/// </summary>
	// 回転行列の作成
	static Matrix2x2 MakeRotateMatrix2x2(const float& theta);
	// 行列の加法
	static Matrix2x2 Add(const Matrix2x2& matrix1, const Matrix2x2& matrix2);
	// 行列の減法
	static Matrix2x2 Subtract(const Matrix2x2& matrix1, const Matrix2x2& matrix2);
	// 行列の積
	static Matrix2x2 Multiply(const Matrix2x2& matrix1, const Matrix2x2& matrix2);
	// スカラーと行列の積
	static Matrix2x2 Multiply(const float& scalar, const Matrix2x2& matrix);
	// ベクトルと行列の積
	static Vec2 Multiply(const Vec2& vector, const Matrix2x2& matrix);
	// 逆行列
	static Matrix2x2 Inverse(const Matrix2x2& matrix);
	// 転置行列
	static Matrix2x2 Transpose(const Matrix2x2& matrix);

	/// <summary>
	/// 3x3の行列
	/// </summary>
	// 拡縮行列の作成
	static Matrix3x3 MakeScaleMatrix(const Vec2& scale);
	// 回転行列の作成
	static Matrix3x3 MakeRotateMatrix3x3(const float& theta);
	// 平行移動行列の作成関数
	static Matrix3x3 MakeTranslateMatrix(const Vec2& translate);
	// 2次元アフィン変換行列の作成
	static Matrix3x3 MakeAffineMatrix(const Vec2& scale, const float& rotate, const Vec2& translate);
	// 正射影行列の作成
	static Matrix3x3 MakeOrthographicMatrix(const float& left, const float& top, const float& right, const float& bottom);
	// ビューポート行列の作成
	static Matrix3x3 MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height);
	// 行列の加法
	static Matrix3x3 Add(const Matrix3x3& matrix1, Matrix3x3& matrix2);
	// 行列の減法
	static Matrix3x3 Subtract(const Matrix3x3& matrix1, Matrix3x3& matrix2);
	// 行列の積
	static Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2);
	// スカラーと行列の積
	static Matrix3x3 Multipty(const float& scalar, const Matrix3x3& matrix);
	// 2次元ベクトルを同次座標として変換する
	static Vec2 Transform(const Vec2& vector, const Matrix3x3& matrix);
	// 逆行列
	static Matrix3x3 Inverse(const Matrix3x3& matrix);
	// 転置行列
	static Matrix3x3 Transpose(const Matrix3x3& matrix);

	/// <summary>
	/// 4x4の行列
	/// </summary>
	// 拡縮行列の作成
	static Matrix4x4 MakeScaleMatrix(const Vec3& scale);
	// X軸回転行列
	static Matrix4x4 MakeRotateXMatrix4x4(const float& radian);
	// Y軸回転行列
	static Matrix4x4 MakeRotateYMatrix4x4(const float& radian);
	// Z軸回転行列
	static Matrix4x4 MakeRotateZMatrix4x4(const float& radian);
	// 回転行列の作成
	static Matrix4x4 MakeRotateMatrix4x4(const float& thetaX, const float& thetaY, const float& thetaZ);
	// 平行移動行列の作成
	static Matrix4x4 MakeTranslateMatrix(const Vec3& translate);
	// 3次元アフィン変換行列の作成
	static Matrix4x4 MakeAffineMatrix(const Vec3& scale, const Vec3& rotate, const Vec3& translate);
	// 透視投影行列の作成
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	// 正射影行列の作成
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	// ビューポート変換行列の作成
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	// 単位行列の作成
	static Matrix4x4 MakeIdentity4x4();
	// 行列の加法
	static Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
	// 行列の減法
	static Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
	// 行列の積
	static Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
	// スカラーと行列の積
	static Matrix4x4 Multiply(const float& scalar, const Matrix4x4& matrix);
	// 3次元ベクトルを同時座標として変換する
	static Vec3 Transform(const Vec3& vector, const Matrix4x4 matrix);
	// 逆行列
	static Matrix4x4 Inverse(const Matrix4x4& matrix);
	// 行列式
	static float Determinant(const Matrix4x4& matrix);
	// 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& matrix);

	/// <summary>
	/// 描画
	/// </summary>
	// 2x2行列の表示
	static void MatrixScreenPrintf(int x, int y, const Matrix2x2& matrix, const char* label);
	// 3x3行列の表示
	static void MatrixScreenPrintf(int x, int y, const Matrix3x3& matrix, const char* label);
	// 4x4行列の表示
	static void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

};

