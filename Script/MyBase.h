#pragma once

class MyBase
{
public:
	// 構造体
	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	typedef struct Vector2 {
		float x;
		float y;
	}Vec2;

	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	typedef struct Vector3 {
		float x;
		float y;
		float z;
	}Vec3;
	
	/// <summary>
	/// 4次元ベクトル
	/// </summary>
	typedef struct Vector4 {
		float x;
		float y;
		float z;
		float w;
	}Vec4;

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
		Vec3 scale;
		Vec3 rotate;
		Vec3 translate;
	};

	/// <summary>
	/// 円
	/// </summary>
	struct Ball {
		Vec2 pos;			// 位置
		Vec2 velocity;		// 速度
		Vec2 acceleration;	// 加速度
		float mass;			// 質量
		float radius;		// 半径
		unsigned int color;	// 色
	};

	/// <summary>
	/// 矩形
	/// </summary>
	struct Box {
		Vec2 pos;				// 位置
		Vec2 size;				// サイズ
		Vec2 velocity;			// 速度
		Vec2 accleration;		// 加速度
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
		Vec2 start;
		Vec2 end;
	};
};

