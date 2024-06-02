#pragma once

class MyBase
{
public:
	// 構造体
	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	struct Vector2 {
		float x;
		float y;
	};

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	struct Vector3 {
		float x;
		float y;
		float z;
	};
	
	/// <summary>
	/// 4次元ベクトル
	/// </summary>
	struct Vector4 {
		float x;
		float y;
		float z;
		float w;
	};

	/// <summary>
	/// 2x2の行列
	/// </summary>
	struct Matrix2x2 {
		float m[2][2];
	};
	
	/// <summary>
	/// 3x3の行列
	/// </summary>
	struct Matrix3x3 {
		float m[3][3];
	};

	/// <summary>
	/// 4x4の行列
	/// </summary>
	struct Matrix4x4 {
		float m[4][4];
	};

	/// <summary>
	/// Transform
	/// </summary>
	struct Transform {
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	/// <summary>
	/// 円
	/// </summary>
	struct Ball {
		Vector2 pos;			// 位置
		Vector2 velocity;		// 速度
		Vector2 acceleration;	// 加速度
		float mass;			// 質量
		float radius;		// 半径
		unsigned int color;	// 色
	};

	/// <summary>
	/// 矩形
	/// </summary>
	struct Box {
		Vector2 pos;				// 位置
		Vector2 size;				// サイズ
		Vector2 velocity;			// 速度
		Vector2 accleration;		// 加速度
		float mass;				// 質量
		unsigned int color;		// 色
	};

	/// <summary>
	/// 矩形の範囲
	/// </summary>
	struct Rect {
		float Top;
		float Bottom;
		float Left;
		float Right;
	};

	/// <summary>
	/// 線
	/// </summary>
	struct Line {
		Vector2 start;
		Vector2 end;
	};

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};
};

