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
	/// 
	/// 2x2の行列 ここから
	/// 
	
	/// <summary>
	/// 2x2回転行列の作成関数
	/// </summary>
	/// <param name="theta">角度(radian)</param>
	static Matrix2x2 MakeRotateMatrix2x2(const float& theta);

	/// <summary>
	/// 2x2行列の加算を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix2x2 Add(const Matrix2x2& matrix1, const Matrix2x2& matrix2);
	
	/// <summary>
	/// 2x2行列の減算を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix2x2 Subtract(const Matrix2x2& matrix1, const Matrix2x2& matrix2);
	
	/// <summary>
	/// 2x2行列の積を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix2x2 Multiply(const Matrix2x2& matrix1, const Matrix2x2& matrix2);
	
	/// <summary>
	/// スカラーと2x2行列の積を返す関数
	/// </summary>
	/// <param name="scalar">スカラー値</param>
	/// <param name="matrix">行列</param>
	static Matrix2x2 Multiply(const float& scalar, const Matrix2x2& matrix);
	
	/// <summary>
	/// 2次元ベクトルと2x2行列の積を返す関数
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	static Vector2 Multiply(const Vector2& vector, const Matrix2x2& matrix);
	
	/// <summary>
	/// 2x2逆行列を取得する関数
	/// </summary>
	/// <param name="matrix">行列</param>
	static Matrix2x2 Inverse(const Matrix2x2& matrix);

	/// <summary>
	/// 2x2転置行列を取得する関数
	/// </summary>
	/// <param name="matrix">行列</param>
	static Matrix2x2 Transpose(const Matrix2x2& matrix);

	/// 
	/// 2x2の行列 ここまで
	/// 

	/// 
	/// 3x3の行列 ここから
	/// 
	
	/// <summary>
	/// 3x3拡縮行列の作成
	/// </summary>
	/// <param name="scale">拡縮の値</param>
	static Matrix3x3 MakeScaleMatrix(const Vector2& scale);

	/// <summary>
	/// 3x3回転行列の作成
	/// </summary>
	/// <param name="theta">角度(radian)</param>
	static Matrix3x3 MakeRotateMatrix3x3(const float& theta);

	/// <summary>
	/// 3x3平行移動行列の作成関数
	/// </summary>
	/// <param name="translate">移動値</param>
	static Matrix3x3 MakeTranslateMatrix(const Vector2& translate);

	/// <summary>
	/// 2次元アフィン変換行列の作成
	/// </summary>
	/// <param name="scale">拡縮のベクトル</param>
	/// <param name="rotate">角度のベクトル</param>
	/// <param name="translate">移動のベクトル</param>
	static Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float& rotate, const Vector2& translate);

	/// <summary>
	/// 3x3正射影行列の作成
	/// </summary>
	/// <param name="left">左辺 X値</param>
	/// <param name="top">上辺 Y値</param>
	/// <param name="right">右辺 X値</param>
	/// <param name="bottom">下辺 Y値</param>
	static Matrix3x3 MakeOrthographicMatrix(const float& left, const float& top, const float& right, const float& bottom);

	/// <summary>
	/// 3x3ビューポート行列の作成
	/// </summary>
	/// <param name="left">左辺 X値</param>
	/// <param name="top">上辺 Y値</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	static Matrix3x3 MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height);

	/// <summary>
	/// 3x3行列の加算を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix3x3 Add(const Matrix3x3& matrix1, Matrix3x3& matrix2);

	/// <summary>
	/// 3x3行列の減算を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix3x3 Subtract(const Matrix3x3& matrix1, Matrix3x3& matrix2);

	/// <summary>
	/// 3x3行列の積を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix3x3 Multiply(const Matrix3x3& matrix1, const Matrix3x3& matrix2);

	/// <summary>
	/// スカラーと3x3行列の積を返す関数
	/// </summary>
	/// <param name="scalar">スカラー値</param>
	/// <param name="matrix">行列</param>
	static Matrix3x3 Multipty(const float& scalar, const Matrix3x3& matrix);

	/// <summary>
	/// 2次元ベクトルを同次座標として変換する関数
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	static Vector2 Transform(const Vector2& vector, const Matrix3x3& matrix);

	/// <summary>
	/// 3x3逆行列の取得する関数
	/// </summary>
	/// <param name="matrix">行列</param>
	static Matrix3x3 Inverse(const Matrix3x3& matrix);

	/// <summary>
	/// 3x3転置行列の取得する関数
	/// </summary>
	/// <param name="matrix">行列</param>
	static Matrix3x3 Transpose(const Matrix3x3& matrix);

	/// 
	/// 3x3の行列 ここまで
	/// 

	/// 
	/// 4x4の行列 ここから
	/// 
	
	/// <summary>
	/// 4x4拡縮行列の作成
	/// </summary>
	/// <param name="scale">拡縮の値</param>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	/// <summary>
	/// X軸回転行列の作成
	/// </summary>
	/// <param name="radian">角度(radian)</param>
	static Matrix4x4 MakeRotateXMatrix4x4(const float& radian);

	/// <summary>
	/// Y軸回転行列の作成
	/// </summary>
	/// <param name="radian">角度(radian)</param>
	static Matrix4x4 MakeRotateYMatrix4x4(const float& radian);

	/// <summary>
	/// Z軸回転行列の作成
	/// </summary>
	/// <param name="radian">角度(radian)</param>
	static Matrix4x4 MakeRotateZMatrix4x4(const float& radian);

	/// <summary>
	/// 4x4回転行列の作成
	/// </summary>
	/// <param name="radianX">X軸周りのθ値(radian)</param>
	/// <param name="radianY">Y軸周りのθ値(radian)</param>
	/// <param name="radianZ">Z軸周りのθ値(radian)</param>
	static Matrix4x4 MakeRotateMatrix4x4(const float& radianX, const float& radianY, const float& radianZ);

	/// <summary>
	/// 4x4平行移動行列の作成
	/// </summary>
	/// <param name="translate">移動値</param>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	/// <summary>
	/// 3次元アフィン変換行列の作成
	/// </summary>
	/// <param name="scale">拡縮のベクトル</param>
	/// <param name="rotate">角度ベクトル</param>
	/// <param name="translate">移動のベクトル</param>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// 4x4透視投影行列の作成
	/// </summary>
	/// <param name="fovY">縦画角</param>
	/// <param name="asoectRatio">アスペクト比</param>
	/// <param name="nearClip">近平面への距離</param>
	/// <param name="farClip">遠平面への距離</param>
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	/// <summary>
	/// 4x4正射影行列の作成
	/// </summary>
	/// <param name="left">左辺 X値</param>
	/// <param name="top">上辺 Y値</param>
	/// <param name="right">右辺 X値</param>
	/// <param name="bottom">下辺 Y値</param>
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	/// <summary>
	/// 4x4ビューポート変換行列の作成
	/// </summary>
	/// <param name="left">左辺 X値</param>
	/// <param name="top">上辺 Y値</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="minDepth">最小深度値</param>
	/// <param name="maxDepth">最大深度値</param>
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	/// <summary>
	/// 4x4単位行列の作成
	/// </summary>
	static Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// 4x4行列の加算を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

	/// <summary>
	/// 4x4行列の減算を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

	/// <summary>
	/// 4x4行列の積を返す関数
	/// </summary>
	/// <param name="matrix1">行列A</param>
	/// <param name="matrix2">行列B</param>
	static Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

	/// <summary>
	/// スカラーと4x4行列の積を返す関数
	/// </summary>
	/// <param name="scalar">スカラー値</param>
	/// <param name="matrix">行列</param>
	static Matrix4x4 Multiply(const float& scalar, const Matrix4x4& matrix);

	/// <summary>
	/// 3次元ベクトルを同時座標として変換する
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	static Vector3 Transform(const Vector3& vector, const Matrix4x4 matrix);

	/// <summary>
	/// 4x4逆行列の作成
	/// </summary>
	/// <param name="matrix">行列</param>
	static Matrix4x4 Inverse(const Matrix4x4& matrix);

	/// <summary>
	/// 4x4行列式の作成
	/// </sumary>
	/// <param name="matrix">行列</param>
	static float Determinant(const Matrix4x4& matrix);

	/// <summary>
	/// 4x4転置行列の作成
	/// </summary>
	/// <param name="matrix">行列</param>
	static Matrix4x4 Transpose(const Matrix4x4& matrix);

	/// 
	/// 4x4の行列 ここから
	/// 

	/// 
	/// 描画 ここから
	/// 
	
	/// <summary>
	/// 2x2行列の表示
	/// </summary>
	/// <param name="x">表示するデバッグ用文字列の左上座標 X</param>
	/// <param name="y">表示するデバッグ用文字列の左上座標 Y</param>
	/// <param name="matrix">表示したい行列の変数</param>
	/// <param name="label">表示したい行列の名前</param>
	//static void MatrixScreenPrintf(int x, int y, const Matrix2x2& matrix, const char* label);

	/// <summary>
	/// 3x3行列の表示
	/// </summary>
	/// <param name="x">表示するデバッグ用文字列の左上座標 X</param>
	/// <param name="y">表示するデバッグ用文字列の左上座標 Y</param>
	/// <param name="matrix">表示したい行列の変数</param>
	/// <param name="label">表示したい行列の名前</param>
	//static void MatrixScreenPrintf(int x, int y, const Matrix3x3& matrix, const char* label);

	/// <summary>
	/// 4x4行列の表示
	/// </summary>
	/// <param name="x">表示するデバッグ用文字列の左上座標 X</param>
	/// <param name="y">表示するデバッグ用文字列の左上座標 Y</param>
	/// <param name="matrix">表示したい行列の変数</param>
	/// <param name="label">表示したい行列の名前</param>
	//static void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

	/// 
	/// 描画 ここまで
	/// 
};

